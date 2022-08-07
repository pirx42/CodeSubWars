// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorld.h"
#include "CSWLog.h"
#include "CSWPyObjectLoader.h"
#include "CSWSettings.h"
#include "CSWWorldGuard.h"
#include "CSWBattleStatistics.h"
#include "CSWExplosionVisualizer.h"
#include "CSWSoundVisualizer.h"

#include "CSWMessageInitializeObjects.h"
#include "CSWMessageCollectObjects.h"
#include "CSWMessageCollisionObjects.h"
#include "CSWMessageUpdateCollisionObjects.h"
#include "CSWMessageUpdateDamageObjects.h"
#include "CSWMessageEmitSoundObjects.h"
#include "CSWMessageRecalculateObjects.h"
#include "CSWMessageUpdateProcessEventObjects.h"
#include "CSWMessageStoreObjects.h"
#include "CSWMessageDrawObjects.h"
#include "CSWMessageDrawOverlayObjects.h"

#include "CSWObject.h"
#include "CSWIDynamic.h"

#include "CSWEvent.h"
#include "CSWEventManager.h"
#include "CSWCollisionDetectedMessage.h"

#include "CSWUtilities.h"

#include "CSWDummy.h"
#include "CSWCamera.h"

#include "CSWIPythonable.h"
#include "PythonContext.h"

#include "CSWSubmarine.h"
#include "Border.h"
#include "Wall.h"
#include "Rock.h"
#include "ActiveRock.h"
#include "BlackHole.h"
#include "Magnet.h"
#include "GreenTorpedo.h"
#include "GreenMine.h"
#include "WeaponSupply.h"

namespace CodeSubWars
{

  const Size3D CSWWorld::CUBE_SIZE(5000, 5000, 5000);
  const double CSWWorld::CUBE_MARGIN = 0.001;
  const double CSWWorld::CUBE_THICKNESS = 100;


  CSWWorld::PtrType CSWWorld::getInstance()
  {
    static PtrType pInstance = PtrType(new CSWWorld());
    return pInstance;
  }


  CSWWorld::~CSWWorld()
  {
  }


  DT_SceneHandle CSWWorld::getScene()
  {
    return m_hDTScene;
  }


  DT_RespTableHandle CSWWorld::getResponseTable()
  {
    return m_hDTRespTable;
  }


  DT_ResponseClass CSWWorld::getResponseClass()
  {
    return m_DTResponseClass;
  }


  void CSWWorld::newWorld(WorldType type)
  {
    QMutexLocker lckRecalc(&m_mtxRecalc);
    QMutexLocker lckDraw(&m_mtxDraw);

    finalizeWorld(); 

    m_hDTScene = DT_CreateScene();
    m_hDTRespTable = DT_CreateRespTable();
    m_DTResponseClass = DT_GenResponseClass(m_hDTRespTable);
  
  
    ARSTD::Time::reset();
    ARSTD::Time::setTimeRatio(0);

    if (type == DEFAULT_1)
      setupEnvironment1();
    else if (type == DEFAULT_2)
      setupEnvironment2();
    else if (type == DEFAULT_3)
      setupEnvironment3();
    else if (type == DEFAULT_4)
      setupEnvironment4();
    else if (type == DEFAULT_5)
      setupEnvironment5();
    else if (type == LOAD_WORLD)
    {
    }
    assert(m_pObjectTree);
  
    calcWorldTransform();
  
    CSWMessageInitializeCollisionObjects initializeCollisionMessage;
    ARSTD::Message<CSWObject>::broadcastMessage(m_pObjectTree, initializeCollisionMessage);
  
    m_bWorldInitialized = true;  
  
    if (type == DEFAULT_1)
      CSWLog::getInstance()->log("Default 1 environment successfully created");
    else if (type == DEFAULT_2)
      CSWLog::getInstance()->log("Default 2 environment successfully created");
    else if (type == DEFAULT_3)
      CSWLog::getInstance()->log("Default 3 environment successfully created");
  }


  void CSWWorld::finalizeWorld()
  {
    if (!m_pObjectTree)
      return;

    QMutexLocker lckRecalc(&m_mtxRecalc);
    QMutexLocker lckDraw(&m_mtxDraw);
    
    finalizeBattle();

    m_bWorldInitialized = false;

    CSWMessageDeleteCollisionObjects deleteCollisionMessage;
    ARSTD::Message<CSWObject>::broadcastMessage(m_pObjectTree, deleteCollisionMessage);

    ARSTD::Node::ChildRange range = m_pObjectTree->getChildRange();
    ARSTD::Node::ChildIterator itChild = range.first;
    for (; itChild != range.second; ++itChild)
    {
      CSWObject::PtrType pObj = std::dynamic_pointer_cast<CSWObject>(*itChild);
      assert(pObj);
      pObj->finalize();
    }
  
    m_pObjectTree.reset();

    DT_DestroyRespTable(m_hDTRespTable);
    DT_DestroyScene(m_hDTScene);
   
    CSWLog::getInstance()->log("environment successfully removed");
  }


  void CSWWorld::newBattle(const CSWUtilities::SubmarineFileContainer& submarines, 
                           BattleType type, 
                           int nTeamSize,
                           ARSTD::Time::Mode timeMode)
  {
    if (!m_bWorldInitialized ||
        submarines.empty())
      return;

    QMutexLocker lckRecalc(&m_mtxRecalc);
    QMutexLocker lckDraw(&m_mtxDraw);

    //remove all subs
    //insert subs
    //recalculate world
    //init collision stuff
    //reset time
    finalizeBattle();


    Py_Initialize();
    //gil held
    if (!Py_IsInitialized())
    {
      CSWLog::getInstance()->log("failed to initialize python");
    }
   
  //PyEval_SetTrace()

    m_pyMainState = PyThreadState_Get();

    ARSTD::Time::reset(timeMode);

    QString logPath;
    if (qApp)
      logPath = qApp->applicationDirPath() + "/records";
    else logPath = "records";
    useNewRecordFileInPath(logPath.toStdString());

    m_pBattleStatistics->initialize();

    m_BattleType = type;
    m_LoadedSubmarines.clear();
    loadSubmarines(submarines, nTeamSize);

    srand((unsigned)time(NULL));
    std::vector<CSWObject::PtrType >::iterator it = m_LoadedSubmarines.begin();
    for (; it != m_LoadedSubmarines.end(); ++it)
    {
      CSWDamDynSolCol::PtrType pObj = std::dynamic_pointer_cast<CSWDamDynSolCol>((*it));
      assert(pObj);

      Matrix44D mat = CSWUtilities::getRandomOrientation();
      if (getSettings()->getVariousProperties() & CSWSettings::INITIALZE_SUBS_IN_PLANE)
      {
        Matrix44D mat2;
        mat2.getZAxis() = mat.getZAxis();
        mat2.getZAxis().y = 0;
        mat2.getZAxis().normalize();
        mat2.getYAxis() = Vector3D(0, 1, 0);
        mat2.getXAxis() = mat2.getYAxis() % mat2.getZAxis();
        mat = mat2;
        mat.makeHomogeneous(Matrix44D::XAXIS);
      }

      if (m_BattleType == TEAM)
        mat.getTranslation() = CSWUtilities::getRandomPosition()*2500.0;
      else
        mat.getTranslation() = CSWUtilities::getRandomPosition()*1500.0;
    
      if (getSettings()->getVariousProperties() & CSWSettings::INITIALZE_SUBS_IN_PLANE)
      {
        mat.getTranslation().y = 0;
      }
      
      pObj->initializeDynamic(mat, Vector3D(0, 0, 0), Vector3D(0, 0, 0));
      attachObject(pObj);
      CSWLog::getInstance()->log(pObj->getName() + " added");
    }

    calcWorldTransform();
  
    it = m_LoadedSubmarines.begin();
    for (; it != m_LoadedSubmarines.end(); ++it)
    {
      if (!CSWMessageInitializeCollisionObjects::initializeCollision(*it)) 
      {
        std::cout << "CSWWorld::newBattle(): could not initialize collision for " << (*it)->getName() << "\n";
        finalizeBattle();
        return;
      }
    }

    rebuildReceiverContainer();

    CSWMessageInitializeObjects<CSWObject> initializeObjects;
    CSWMessageInitializeObjects<CSWObject>::broadcastMessage(m_pObjectTree, initializeObjects);
  
    m_bBattleInitialized = true;
    std::stringstream ss;
    ss << "battle successfully started (with " << m_LoadedSubmarines.size() << " submarines)";
    CSWLog::getInstance()->log(ss.str());
    m_LoadedSubmarines.clear();
  
    m_fLastStoredTime = -std::numeric_limits<double>::max();

    m_CalculateTimes.clear();
    m_TransformCalculateTimes.clear();
    m_RecalcTimes.clear();
    m_UpdateProcessEventCalculateTimes.clear();
    m_CollisionCalculateTimes.clear();
  }


  void CSWWorld::finalizeBattle()
  {
    if (!m_bBattleInitialized)
      return;

    QMutexLocker lckRecalc(&m_mtxRecalc);
    QMutexLocker lckDraw(&m_mtxDraw);

    m_bBattleInitialized = false;

    m_DynamicObjectContainer.clear();
    m_SoundReceiverObjectContainer.clear();
    getExplosionVisualizer()->clear();
    getSoundVisualizer()->clear();

    std::vector<CSWObject::PtrType> objectsToRemove;
    ARSTD::Node::ChildRange range = m_pObjectTree->getChildRange();
    ARSTD::Node::ChildIterator itChild = range.first;
    for (; itChild != range.second; ++itChild)
    {
      if ((*itChild)->getName().length() < 3 ||
          ((*itChild)->getName().length() >= 3 && (*itChild)->getName().substr(0, 3) != "env"))
      {
        CSWObject::PtrType pObj = std::dynamic_pointer_cast<CSWObject>(*itChild);
        assert(pObj);
        objectsToRemove.push_back(pObj);
      }
    }

    CSWLog::getInstance()->log("battle statistics:");
    m_pBattleStatistics->write();
    CSWLog::getInstance()->log("clean up environment");


    m_pWorldGuard->clear();


    std::vector<CSWObject::PtrType>::iterator it = objectsToRemove.begin();
    for (; it != objectsToRemove.end(); ++it)
    {
      bool bResult = CSWMessageDeleteCollisionObjects::deleteCollision(*it);  
      (*it)->finalize();
      bResult &= getObjectTree()->detach(*it);
      if (bResult)
        CSWLog::getInstance()->log((*it)->getName() + " removed");
      else
        CSWLog::getInstance()->log((*it)->getName() + " could not correctly removed");
  
      //PyThreadState* state = NULL;
      PythonContext::PtrType pyContext;
      if (CSWIPythonable::PtrType pPyObj = std::dynamic_pointer_cast<CSWIPythonable>(*it))
      {
        //state = pPyObj->getThreadState();
        pyContext = pPyObj->getPythonContext();
      }
      if (pyContext)
      {
        auto lck = pyContext->makeCurrent();
        it->reset();
      }

      //if (state)
      //{
      //  //gil must not be held
      //  PyEval_AcquireThread(state);
      //  //gil held

      //  it->reset();

      //  Py_EndInterpreter(state);
      //  //gil held

      //  //gil must be held
      //  PyThreadState_Swap(m_pyMainState);

      //  //gil must be held
      //  PyEval_ReleaseThread(m_pyMainState);
      //  //gil not held
      //}    
    }
    objectsToRemove.clear();

    //close attached sink
    m_RecordStream.strict_sync();
    m_RecordStream.reset();

    PythonContext::destroyAllRemaining();

    if (m_pyMainState)
    {  
      //gil must be held
      PyThreadState_Swap(m_pyMainState);
      //gil held
    }

    Py_FinalizeEx();

    ARSTD::Time::reset();
    ARSTD::Time::setTimeRatio(0);
   
    CSWLog::getInstance()->log("battle successfully stopped");
  }


  PyThreadState* CSWWorld::getPyMainState() const
  {
    return m_pyMainState;
  }


  CSWObject::PtrType CSWWorld::getObjectTree()
  {
    return m_pObjectTree;
  }


  const CSWObject::PtrType CSWWorld::getObjectTree() const
  {
    return m_pObjectTree;
  }


  void CSWWorld::setObjectTree(const CSWObject::PtrType pObjectTree)
  {
    if (m_pObjectTree == pObjectTree)
      return;
    m_pObjectTree = pObjectTree;
  }


  bool CSWWorld::isOutside(const std::shared_ptr<CSWObject> pObject) const
  {
    return fabs(pObject->getTransform().getTranslation().x) > CUBE_SIZE.getWidth()*0.5 ||
           fabs(pObject->getTransform().getTranslation().y) > CUBE_SIZE.getHeight()*0.5 ||
           fabs(pObject->getTransform().getTranslation().z) > CUBE_SIZE.getDepth()*0.5;
  }


  CSWObject::PtrType CSWWorld::getObject(const Vector3D& vecWorldTPosition, const double& fDistance) const
  {
    return CSWObject::findObject<CSWISolid>(m_pObjectTree, vecWorldTPosition, fDistance);
  }


  bool CSWWorld::hasChanged(bool bResetModification)
  {
    return m_pObjectTree ? m_pObjectTree->getModifications(bResetModification) != CSWObject::Nothing : false;
  }


  bool CSWWorld::isBattleRunning() const
  {
    return m_bBattleInitialized;
  }


  void CSWWorld::draw()
  {
    if (!m_bWorldInitialized)
      return;

    QMutexLocker lckDraw(&m_mtxDraw);

    CSWMessageDrawObjects<CSWObject> drawObjectsMessage;
    CSWMessageDrawObjects<CSWObject>::broadcastMessage(m_pObjectTree, drawObjectsMessage, 1);

    //draw world cube
    glPushAttrib(GL_ALL_ATTRIB_BITS);

      glDisable(GL_CULL_FACE);
    
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT, GL_DIFFUSE);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glShadeModel(GL_FLAT);
      glDisable(GL_LIGHTING);
      glColor4d(0, 0, 1, 0.2);

      glEnable(GL_DEPTH_TEST);
      glDepthMask(false);

      BoundingBoxD bbox(Matrix44D(Vector3D(-CUBE_SIZE.getDepth()/2 + CUBE_THICKNESS, 
                                           -CUBE_SIZE.getHeight()/2 + CUBE_THICKNESS, 
                                           -CUBE_SIZE.getDepth()/2 + CUBE_THICKNESS)), 
                        Size3D(CUBE_SIZE.getWidth() - CUBE_THICKNESS*2, 
                               CUBE_SIZE.getHeight() - CUBE_THICKNESS*2, 
                               CUBE_SIZE.getDepth() - CUBE_THICKNESS*2));

      glBegin(GL_QUADS);
        for (int iCnt = 0; iCnt < 6; ++iCnt) 
        {
          glVertex3dv(bbox.getRectangle(iCnt).getPoint0().pData);
          glVertex3dv(bbox.getRectangle(iCnt).getPoint1().pData);
          glVertex3dv(bbox.getRectangle(iCnt).getPoint2().pData);
          glVertex3dv(bbox.getRectangle(iCnt).getPoint3().pData);
        } 
      glEnd();    

      glDisable(GL_LIGHTING);
      glColor3d(0, 0, 1);
      glLineWidth(2);
      glBegin(GL_LINES);
        for (int iCnt = 0; iCnt < 12; ++iCnt) 
        {
          glVertex3dv(bbox.getSegment(iCnt).getPoint1().pData);
          glVertex3dv(bbox.getSegment(iCnt).getPoint2().pData);
        } 
      glEnd();
      glDepthMask(true);
    glPopAttrib();


    CSWMessageDrawOverlayObjects<CSWObject> drawOverlayObjectsMessage;
    CSWMessageDrawOverlayObjects<CSWObject>::broadcastMessage(m_pObjectTree, drawOverlayObjectsMessage, 1);
  
    m_pExplosionVisualizer->draw();
    m_pSoundVisualizer->draw();
  }


  CSWWorld::CSWWorld()
  : m_pSettings(CSWSettings::create()),
    m_pWorldGuard(CSWWorldGuard::create()),
    m_pBattleStatistics(CSWBattleStatistics::create()),
    m_pExplosionVisualizer(CSWExplosionVisualizer::create()),
    m_pSoundVisualizer(CSWSoundVisualizer::create()),
    m_bWorldInitialized(false),
    m_bBattleInitialized(false),
    m_mtxRecalc(QMutex::Recursive),
    m_mtxDraw(QMutex::Recursive),
    m_pyMainState(NULL),
    m_CalculateTimes(200),
    m_TransformCalculateTimes(200),
    m_RecalcTimes(200),
    m_UpdateProcessEventCalculateTimes(200),
    m_CollisionCalculateTimes(200)
  {
    m_Load.push_back(boost::tuples::make_tuple("Total", 0, 0));
    m_Load.push_back(boost::tuples::make_tuple("Transform", 0, 0));
    m_Load.push_back(boost::tuples::make_tuple("Recalc", 0, 0));
    m_Load.push_back(boost::tuples::make_tuple("Update/Events", 0, 0));
    m_Load.push_back(boost::tuples::make_tuple("Collision", 0, 0));
  }


  void CSWWorld::setupEnvironment1()
  {
    m_pObjectTree = CSWDummy::create("Root");

    CSWBorder::PtrType pBorder;
    pBorder = Border::create("envBorder1", Matrix44D(Vector3D(0, 0, CUBE_SIZE.getDepth()/2 - CUBE_THICKNESS/2)), 
                             Size3D(CUBE_SIZE.getWidth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN, CUBE_SIZE.getHeight(), CUBE_THICKNESS));
    attachObject(pBorder, false);
    pBorder = Border::create("envBorder2", Matrix44D(Vector3D(0, 0, -CUBE_SIZE.getDepth()/2 + CUBE_THICKNESS/2)), 
                             Size3D(CUBE_SIZE.getWidth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN, CUBE_SIZE.getHeight(), CUBE_THICKNESS));
    attachObject(pBorder, false);

    pBorder = Border::create("envBorder3", Matrix44D(Vector3D(CUBE_SIZE.getWidth()/2 - CUBE_THICKNESS/2, 0, 0)), 
                             Size3D(CUBE_THICKNESS, CUBE_SIZE.getHeight(), CUBE_SIZE.getDepth()));
    attachObject(pBorder, false);
    pBorder = Border::create("envBorder4", Matrix44D(Vector3D(-CUBE_SIZE.getWidth()/2 + CUBE_THICKNESS/2, 0, 0)), 
                             Size3D(CUBE_THICKNESS, CUBE_SIZE.getHeight(), CUBE_SIZE.getDepth()));
    attachObject(pBorder, false);

    pBorder = Border::create("envBorder5", Matrix44D(Vector3D(0, CUBE_SIZE.getHeight()/2 - CUBE_THICKNESS/2, 0)), 
                             Size3D(CUBE_SIZE.getWidth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN, CUBE_THICKNESS, CUBE_SIZE.getDepth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN));
    attachObject(pBorder, false);
    pBorder = Border::create("envBorder6", Matrix44D(Vector3D(0, -CUBE_SIZE.getHeight()/2 + CUBE_THICKNESS/2, 0)), 
                             Size3D(CUBE_SIZE.getWidth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN, CUBE_THICKNESS, CUBE_SIZE.getDepth() - 2*CUBE_THICKNESS - 2*CUBE_MARGIN));
    attachObject(pBorder, false);


    attachObject(CSWCamera::create("envMainCamera", Matrix44D(Vector3D(1, 0, 0),
                                                              Vector3D(0, 0, 1),
                                                              Vector3D(0, -1, 0),
                                                              Vector3D(0, 2300, 0))));
  }


  void CSWWorld::setupEnvironment2()
  {
    //only not damageable and not dynamic object can be environment.
    //the objects state must not change over time.
    setupEnvironment1();
  
  //  m_pObjectTree->attach(BlackHole::create("envBlackHole",
  //                                          Matrix44D(Vector3D(0, 0, 0)), 
  //                                          Size3D(3000, 3000, 3000)));

  //  m_pObjectTree->attach(Rock::create("envRock1",
  //                                     Matrix44D(Vector3D(200, 150, 400)), 
  //                                     Size3D(50, 50, 50)));
  //
  //m_pObjectTree->attach(Rock::create("envRock2",
  //                                    Matrix44D(Vector3D(0, 0, 600)), 
  //                                    Size3D(50, 50, 50)));

  //  attachObject(Wall::create("envWall1",
  //                            Matrix44D(Vector3D(700, 20, 800)), 
  //                            Size3D(400, 200, 50)));

  //  m_pObjectTree->attach(Magnet::create("envMagnetA",
  //                                       Matrix44D(Vector3D(-300, 0, 550)), 
  //                                       Size3D(10, 10, 10)));

    for (int i = 0; i < 20; ++i)
    {
      attachObject(Rock::create(std::string("envRock") + ARSTD::Utilities::toString(i),
                                Matrix44D(Vector3D(rand()%1500 - 750, rand()%1500 - 750, rand()%1500 - 750)), 
                                Size3D(rand()%100 + 50, rand()%100 + 50, rand()%100 + 50)));
    }
  }


  void CSWWorld::setupEnvironment3()
  {
    //only not damageable and not dynamic object can be environment.
    //the objects state must not change over time.
    setupEnvironment1();
  
    attachObject(WeaponSupply::create("envWeaponSupply", 
                                       Matrix44D(Vector3D(0, 0, 0)), 
                                       Size3D(100, 100, 100)));
  }


  void CSWWorld::setupEnvironment4()
  {
    //only not damageable and not dynamic object can be environment.
    //the objects state must not change over time.  
    setupEnvironment1();
  
    attachObject(BlackHole::create("envBlackHole",
                                   Matrix44D(Vector3D(0, 0, 0)), 
                                   Size3D(110, 110, 110)));

  }


  void CSWWorld::setupEnvironment5()
  {
    //only not damageable and not dynamic object can be environment.
    //the objects state must not change over time.
    setupEnvironment1();
  
    for (int i = 0; i < 5; ++i)
    {
      Matrix44D mat = CSWUtilities::getRandomOrientation();
      mat.getTranslation() = Vector3D(rand()%1500 - 750, rand()%1500 - 750, rand()%1500 - 750);
      attachObject(ActiveRock::create(std::string("envActiveRock") + ARSTD::Utilities::toString(i),
                                      mat, 
                                      Size3D(rand()%100 + 50, rand()%100 + 50, rand()%100 + 50)));
    }

  }


  void CSWWorld::attachObject(CSWObject::PtrType pObject, bool bChangePosition)
  {
    //check name and rename if name not unique
    std::string strOrgName = pObject->getName();
    std::string strNewName = strOrgName;
    int nCnt = 2;
    while (CSWObject::findElement(m_pObjectTree, strNewName))
    {
      strNewName = strOrgName + ARSTD::Utilities::toString(nCnt);
      ++nCnt;
    }
    pObject->rename(strOrgName, strNewName, true);

  
    //check for existing objects on insert position
    if (bChangePosition)
    {
      if (CSWISolid::PtrType pSolid = std::dynamic_pointer_cast<CSWISolid>(pObject))
      {
        nCnt = 0;
        Vector3D vecNewPosition = pObject->getTransform().getTranslation();
        double fObjectRadius = pSolid->getSurfaceBoundingBox().getDiagonalLength()*0.5;
        CSWISolid::PtrType pSolidExistingObject = std::dynamic_pointer_cast<CSWISolid>(getObject(vecNewPosition, fObjectRadius*2));
        while (pSolidExistingObject && nCnt < 10)
        {
          Vector3D vecRandomDir = CSWUtilities::getRandomDirection();
          if (std::dynamic_pointer_cast<CSWSubmarine>(pObject) && 
              (getSettings()->getVariousProperties() & CSWSettings::INITIALZE_SUBS_IN_PLANE))
          {
            vecRandomDir.y = 0;
          }
          vecNewPosition = pObject->getTransform().getTranslation() + vecRandomDir * fObjectRadius*2.5 * (nCnt + 2.0)*0.5;
          pSolidExistingObject = std::dynamic_pointer_cast<CSWISolid>(getObject(vecNewPosition, fObjectRadius*2));
          ++nCnt;
        }
        if (pSolidExistingObject && nCnt == 10)
        {
          throw std::runtime_error("could not find free space for placing object " + pObject->getName());
        }
       
        Matrix44D mat(pObject->getTransform());
        mat.getTranslation() = vecNewPosition;

        pObject->setTransform(mat);
        //all childs must be recaclulate its positions immediatly
        pObject->calcWorldTransforms();
        if (CSWIDynamic::PtrType pDynamic = std::dynamic_pointer_cast<CSWIDynamic>(pObject))
        {
          pDynamic->setNewTransform(mat);
        }
      }
    }

    m_pObjectTree->attach(pObject);
  }


  void CSWWorld::recalculate()
  {
    if (!m_bWorldInitialized || !m_bBattleInitialized)
      return;

    bool bBattleRunning = false;
    {
      QMutexLocker lckRecalc(&m_mtxRecalc);
    
      //store world 
      double fCurrentTime = ARSTD::Time::getTime();
      if (fCurrentTime - m_fLastStoredTime > 2)
      {
        store();
        m_fLastStoredTime = fCurrentTime;
      }
    
      double fCalculateTime = ARSTD::Time::getRealTime();

      double fRecalcTime = ARSTD::Time::getRealTime();
      CSWEventManager::getInstance()->deliverAllEvents();
    
      rebuildReceiverContainer();
    
      //clear sound receiver
      std::vector<std::shared_ptr<CSWObject> >::iterator it = m_SoundReceiverObjectContainer.begin();    
      for (; it != m_SoundReceiverObjectContainer.end(); ++it)
      {
        CSWISoundReceiver::PtrType pSoundReceiver = std::dynamic_pointer_cast<CSWISoundReceiver>(*it);
        assert(pSoundReceiver);
        pSoundReceiver->reset();
      }

      CSWMessageEmitSoundObjects<CSWObject> emitSoundMessage(m_SoundReceiverObjectContainer);
      CSWMessageEmitSoundObjects<CSWObject>::broadcastMessage(m_pObjectTree, emitSoundMessage, 2);
    
      CSWMessageRecalculateObjects<CSWObject> recalculateMessage(m_DynamicObjectContainer);
      CSWMessageRecalculateObjects<CSWObject>::broadcastMessage(m_pObjectTree, recalculateMessage, 1);
      m_RecalcTimes.push_back(ARSTD::Time::getRealTime() - fRecalcTime);



      //calculates the absolute position of every object
      double fTransformCalculateTime = ARSTD::Time::getRealTime();
      calcWorldTransform();
      m_TransformCalculateTimes.push_back(ARSTD::Time::getRealTime() - fTransformCalculateTime);



      //now all object positions are up to date -> update and processEvent are now processed
      double fUpdateProcessEventCalculateTime = ARSTD::Time::getRealTime();
      CSWMessageUpdateProcessEventObjects<CSWObject> updateProcessEventMessage;
      CSWMessageUpdateProcessEventObjects<CSWObject>::broadcastMessage(m_pObjectTree, updateProcessEventMessage);
      m_UpdateProcessEventCalculateTimes.push_back(ARSTD::Time::getRealTime() - fUpdateProcessEventCalculateTime);

      
      
      //collisionen (betrifft solid)
      double fCollisionCalculateTime = ARSTD::Time::getRealTime();
      //due to calling of dtTest() on destruction of the message this is encapsulated in its own scope
      {
        CSWMessageUpdateCollisionObjects<CSWObject> updateCollisionMessage;
        CSWMessageUpdateCollisionObjects<CSWObject>::broadcastMessage(m_pObjectTree, updateCollisionMessage);
      }
      m_CollisionCalculateTimes.push_back(ARSTD::Time::getRealTime() - fCollisionCalculateTime);


      //clear container to ensure that only the tree contain the objects
      m_DynamicObjectContainer.clear();
      m_SoundReceiverObjectContainer.clear();


      //remove dead objects
      CSWMessageUpdateDamageObjects<CSWObject>::ObjectContainer collectedObjects;
      {
        CSWMessageUpdateDamageObjects<CSWObject> updateDamageableMessage;
        CSWMessageUpdateDamageObjects<CSWObject>::broadcastMessage(m_pObjectTree, updateDamageableMessage, 1);
        collectedObjects = updateDamageableMessage.getCollectedObjects();
      }
      CSWMessageUpdateDamageObjects<CSWObject>::ObjectContainer::iterator itDeadObject = collectedObjects.begin();
      CSWMessageUpdateDamageObjects<CSWObject>::ObjectContainer::const_iterator itEnd = collectedObjects.end();
      for (; itDeadObject != itEnd; ++itDeadObject)
      {
        bool bResult = CSWMessageDeleteCollisionObjects::deleteCollision(*itDeadObject);  
        (*itDeadObject)->finalize();
        bResult &= getObjectTree()->detach(*itDeadObject);
        if (!bResult)
          throw std::runtime_error(std::string((*itDeadObject)->getName()) + ": is dead and could not be correctly removed from world");

        //PyThreadState* state = NULL;
        PythonContext::PtrType pyContext;
        if (CSWIPythonable::PtrType pPyObj = std::dynamic_pointer_cast<CSWIPythonable>(*itDeadObject))
        {
          //state = pPyObj->getThreadState();
          pyContext = pPyObj->getPythonContext();
        }
        if (pyContext)
        {
          auto lck = pyContext->makeCurrent();
          itDeadObject->reset();
        }
        //if (state)
        //{
        //  //gil must not be held
        //  PyEval_AcquireThread(state);
        //  //gil held

        //  itDeadObject->reset();

        //  Py_EndInterpreter(state);
        //  //gil held

        //  PyThreadState_Swap(m_pyMainState);
        //  //gil must be held
        //  PyEval_ReleaseThread(m_pyMainState);
        //  //gil not held
        //}
      }
      collectedObjects.clear();

      //check world for inconsistencies
      m_pWorldGuard->update();
      bBattleRunning = m_pWorldGuard->isBattleRunning();
    
      m_CalculateTimes.push_back(ARSTD::Time::getRealTime() - fCalculateTime);


      std::pair<double, double> result = calcStatistics(m_CalculateTimes.rbegin(), m_CalculateTimes.rend());
      m_Load[0] = boost::tuples::make_tuple("Total", result.first*1000, result.second*1000);
      result = calcStatistics(m_TransformCalculateTimes.rbegin(), m_TransformCalculateTimes.rend());
      m_Load[1] = boost::tuples::make_tuple("Transform", result.first*1000, result.second*1000);
      result = calcStatistics(m_RecalcTimes.rbegin(), m_RecalcTimes.rend());
      m_Load[2] = boost::tuples::make_tuple("Recalc", result.first*1000, result.second*1000);
      result = calcStatistics(m_UpdateProcessEventCalculateTimes.rbegin(), m_UpdateProcessEventCalculateTimes.rend());
      m_Load[3] = boost::tuples::make_tuple("Update/Events", result.first*1000, result.second*1000);
      result = calcStatistics(m_CollisionCalculateTimes.rbegin(), m_CollisionCalculateTimes.rend());
      m_Load[4] = boost::tuples::make_tuple("Collision", result.first*1000, result.second*1000);
    }  
  
    if ((getSettings()->getVariousProperties() & CSWSettings::AUTOMATIC_BATTLE_STOP) && !bBattleRunning)
    {
      CSWLog::getInstance()->log("one submarine/team leftover -> stopping battle");
      finalizeBattle();
    }
  }


  void CSWWorld::store()
  {
    if (getSettings()->getVariousProperties() & CSWSettings::STORE_WORLD_PERIODICALLY)
      store(m_RecordStream);
  }


  void CSWWorld::useNewRecordFileInPath(const std::string& strPath)
  {
    if (!(getSettings()->getVariousProperties() & CSWSettings::STORE_WORLD_PERIODICALLY))
      return;

    QDir().mkdir(strPath.c_str());

    //close attached sink
    m_RecordStream.strict_sync();
    m_RecordStream.reset();

    std::string strFileName = strPath + "/" + CSWUtilities::getUniqueFilename(strPath.c_str()).toStdString() + ".cbr";
    boost::iostreams::file_sink fs(strFileName, std::ios::out | std::ios::binary);
  
    //on first call the header must be written. (total header size: 6 + 3 byte)
    fs.write("CSW.BR", 6);
    fs.write(reinterpret_cast<const char*>(&Constants::getVersion().nMainVersion), 1);
    fs.write(reinterpret_cast<const char*>(&Constants::getVersion().nSubVersion), 1);
    fs.write(reinterpret_cast<const char*>(&Constants::getVersion().nSubSubVersion), 1);

    //configure filtered output stream with zlib compressor (max compression) and file sink
    //m_RecordStream.push(boost::iostreams::zlib_compressor(9));
    m_RecordStream.push(fs); 
  }


  void CSWWorld::store(boost::iostreams::filtering_ostream& os)
  {
    if (!os.is_complete())
      return;
  
    float fTime = ARSTD::Time::getTime();
    os.write(reinterpret_cast<const char*>(&fTime), 4);
  
    //write number of childs (only solid ones).
    ARSTD::uint16 nNumObjects = m_pObjectTree->getChildNumber<CSWISolid>();
    os.write(reinterpret_cast<const char*>(&nNumObjects), 2);
  
    CSWMessageStoreObjects<CSWObject> storeMessage(os);
    CSWMessageStoreObjects<CSWObject>::broadcastMessage(m_pObjectTree, storeMessage, 1);
  }


  CSWSettings::PtrType CSWWorld::getSettings()
  {
    return m_pSettings;
  }


  CSWWorldGuard::PtrType CSWWorld::getWorldGuard()
  {
    return m_pWorldGuard;
  }


  CSWBattleStatistics::PtrType CSWWorld::getBattleStatistics()
  {
    return m_pBattleStatistics;
  }


  CSWExplosionVisualizer::PtrType CSWWorld::getExplosionVisualizer()
  {
    return m_pExplosionVisualizer;
  }


  CSWSoundVisualizer::PtrType CSWWorld::getSoundVisualizer()
  {
    return m_pSoundVisualizer;
  }


  const std::vector<boost::tuples::tuple<std::string, double, double> >& CSWWorld::getLoad() const
  {
    return m_Load;
  }


  void CSWWorld::calcWorldTransform()
  {
    m_pObjectTree->calcWorldTransforms();
  }


  void CSWWorld::loadSubmarines(const CSWUtilities::SubmarineFileContainer& submarines, int nTeamSize)
  {
    int nUniqueTeamID = 0;
    int nNumSubmarinesToInstanciate = m_BattleType == SINGLE ? 1 : nTeamSize;
    CSWUtilities::SubmarineFileContainer::const_iterator it = submarines.begin();
    for (; it != submarines.end(); ++it)
    {
      for (int i = 0; i < nNumSubmarinesToInstanciate; ++i)
      {
        CSWObject::PtrType pObj = CSWPyObjectLoader::createFromPyScript(it->second, it->first);
        if (!pObj)
        {
          CSWLog::getInstance()->log(it->first + " could not be loaded -> skipped");
          break;
        }
      
        CSWSubmarine::PtrType pSubmarine = std::dynamic_pointer_cast<CSWSubmarine>(pObj);
        assert(pSubmarine);
        pSubmarine->setTeamID(nUniqueTeamID);
      
        m_LoadedSubmarines.push_back(pObj);
      }
      ++nUniqueTeamID;
    }
  }


  void CSWWorld::rebuildReceiverContainer()
  {
    typedef CSWMessageCollectObjects2<CSWObject, CSWIDynamic, CSWISoundReceiver> MessageCollectDynSndRec;
    MessageCollectDynSndRec collectMessage;
    MessageCollectDynSndRec::broadcastMessage(getObjectTree(), collectMessage);
    m_DynamicObjectContainer = collectMessage.getCollectedObjects()[0];
    m_SoundReceiverObjectContainer = collectMessage.getCollectedObjects()[1];
  }

}