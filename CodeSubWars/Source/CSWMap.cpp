// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

  
#include "PrecompiledHeader.h"
#include "CSWMap.h"
#include "CSWMapVisualizer.h"


namespace CodeSubWars
{

  const double CSWMap::EQUAL_POSITION_DISTANCE = 100;


  bool operator == (const CSWMapElement& lhs, const CSWMapElement& rhs)
  {
    return lhs.vecWorldTPosition == rhs.vecWorldTPosition &&
           lhs.vecWorldTVelocity == rhs.vecWorldTVelocity &&
           lhs.nLevel == rhs.nLevel &&
           fabs(lhs.fTime - rhs.fTime) < EPSILON &&
           lhs.bMarkAsDeleted == rhs.bMarkAsDeleted;
  }


  CSWMap::PtrType CSWMap::create(const std::string& strName)
  {
    return PtrType(new CSWMap(strName));
  }


  CSWMap::~CSWMap()
  {
  }


  QWidget* CSWMap::getInformationWidget(QWidget* pParent)
  {
    if (!m_pVisualizer)
      m_pVisualizer = new CSWMapVisualizer(pParent, std::dynamic_pointer_cast<CSWMap>(getSharedThis()));
    return m_pVisualizer;
  }


  void CSWMap::update()
  {
    CSWEquipment::update();
    //validate map (remove entries that are not valid (redetected within 20 seconds))
    MapElementContainer::iterator itEnd = std::remove_if(m_Map.begin(), m_Map.end(), isElementNotValid);
    m_Map.erase(itEnd, m_Map.end());
  }


  unsigned long CSWMap::insertElement(const Vector3D& vecPosition, unsigned long nLevel, unsigned long nBitMask)
  {
    CSWMapElement el;
    el.vecWorldTPosition = vecPosition;
    el.nLevel = nLevel;
    return insertElement(el, nBitMask);
  }


  unsigned long CSWMap::insertElement(const CSWMapElement& element, unsigned long nBitMask)
  {
    if ((element.vecWorldTPosition - getWorldTransform().getTranslation()).getSquaredLength() < EQUAL_POSITION_DISTANCE*EQUAL_POSITION_DISTANCE)
      return 0;

    double fCurrentTime = ARSTD::Time::getTime();
    MapElementContainer::iterator itFound = findFirstElement(element.vecWorldTPosition, EQUAL_POSITION_DISTANCE, std::numeric_limits<unsigned long>::max());
    if (itFound != m_Map.end())
    {
      //calc averaged velocity
      double fElapsedTime = fCurrentTime - itFound->second.fTime;
      itFound->second.m_Velocities.push_back((element.vecWorldTPosition - itFound->second.vecWorldTPosition)/fElapsedTime);
      assert(!itFound->second.m_Velocities.empty());
      itFound->second.vecWorldTVelocity = std::accumulate(itFound->second.m_Velocities.begin(), itFound->second.m_Velocities.end(), 
                                                          Vector3D(0, 0, 0)) / static_cast<double>(itFound->second.m_Velocities.size());
    
      itFound->second.vecWorldTPosition = element.vecWorldTPosition;
      itFound->second.nLevel = (itFound->second.nLevel & nBitMask) | element.nLevel;
      itFound->second.nUserData = element.nUserData;
      itFound->second.fTime = fCurrentTime;
      unsigned long nID = itFound->second.nID;

      //keep the map sorted
      double fLength = element.vecWorldTPosition.getLength();
      MapElementContainer::iterator itPos = std::lower_bound(m_Map.begin(), m_Map.end(), fLength);
      m_Map.splice(itPos, m_Map, itFound);
      itFound->first = fLength;
    
      return nID;
    }

    static unsigned long nCnt = 0;
    CSWMapElement tmpEl(element);
    tmpEl.nID = ++nCnt;
    tmpEl.fTime = fCurrentTime;
    tmpEl.vecWorldTVelocity = Vector3D(0, 0, 0);
    tmpEl.bMarkAsDeleted = false;

    double fLength = element.vecWorldTPosition.getLength();
    MapElementContainer::iterator itPos = std::lower_bound(m_Map.begin(), m_Map.end(), fLength);
    m_Map.insert(itPos, std::make_pair(fLength, tmpEl));

    return tmpEl.nID;
  }


  bool CSWMap::isEmpty() const
  {
    return m_Map.empty();
  }


  void CSWMap::clear()
  {
    m_Map.clear();
  }


  CSWMapElement CSWMap::findElementByID(unsigned long nID)
  {
    MapElementContainer::const_iterator it = m_Map.begin();
    for (; it != m_Map.end(); ++it)
    { 
      if (!it->second.bMarkAsDeleted &&
          it->second.nID == nID)
      {
        return it->second;
      }
    }
    return CSWMapElement();
  }


  CSWMapElement CSWMap::findNearestElement(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel)
  {
    CSWMapElement nearestElement;
    if (nLevel & nNotLevel)
      return nearestElement;
    if (fRadius < 0)
      fRadius = 0;

    double fSquaredRadius = fRadius*fRadius;
    double fNearestSquaredDistance = std::numeric_limits<double>::max();
    double fLength = vecPosition.getLength();
    MapElementContainer::const_iterator lowerIt = std::lower_bound(m_Map.begin(), m_Map.end(), fLength - fRadius);
    MapElementContainer::const_iterator upperIt = std::upper_bound(m_Map.begin(), m_Map.end(), fLength + fRadius);
    MapElementContainer::const_iterator it = lowerIt;
    for (; it != upperIt; ++it)
    { 
      double fCurrentSquaredDistance = (vecPosition - it->second.vecWorldTPosition).getSquaredLength();
      if (!it->second.bMarkAsDeleted &&
          (nLevel & it->second.nLevel) &&
          !(nNotLevel & it->second.nLevel) &&
          fCurrentSquaredDistance < fNearestSquaredDistance &&
          fCurrentSquaredDistance < fSquaredRadius)
      {
        fNearestSquaredDistance = fCurrentSquaredDistance;
        nearestElement = it->second;
      }
    }
    return nearestElement;
  }


  std::vector<CSWMapElement> CSWMap::findElements(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel)
  {
    std::vector<CSWMapElement> elements;
    if (nLevel & nNotLevel)
      return elements;
    if (fRadius < 0)
      fRadius = 0;
    //first check if the position is near or outside the border 
  
    double fSquaredRadius = fRadius*fRadius;
    double fLength = vecPosition.getLength();
    MapElementContainer::const_iterator lowerIt = std::lower_bound(m_Map.begin(), m_Map.end(), fLength - fRadius);
    MapElementContainer::const_iterator upperIt = std::upper_bound(m_Map.begin(), m_Map.end(), fLength + fRadius);
    MapElementContainer::const_iterator it = lowerIt;
    for (; it != upperIt; ++it)
    {
      double fCurrentSquaredDistance = (vecPosition - it->second.vecWorldTPosition).getSquaredLength();
      if (!it->second.bMarkAsDeleted &&
          (nLevel & it->second.nLevel) &&
          !(nNotLevel & it->second.nLevel) &&
          fCurrentSquaredDistance < fSquaredRadius)
      {
        elements.push_back(it->second);
      }
    }
    return elements;
  }


  int CSWMap::removeElements(unsigned long nLevel, unsigned long nNotLevel)
  {
    if (nLevel & nNotLevel)
      return 0;
  
    int nNumDeleted = 0;
    MapElementContainer::iterator it = m_Map.begin();
    for (; it != m_Map.end(); ++it)
    {
      if (it->second.nLevel & nLevel && 
          !(it->second.nLevel & nNotLevel))
      {
        it->second.bMarkAsDeleted = true;
        ++nNumDeleted;
      }
    }
    return nNumDeleted;
  }


  int CSWMap::removeElements(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel)
  {
    if (nLevel & nNotLevel)
      return 0;
    if (fRadius < 0)
      fRadius = 0;

    int nNumDeleted = 0;
    double fSquaredRadius = fRadius*fRadius;
    double fLength = vecPosition.getLength();
    MapElementContainer::iterator lowerIt = std::lower_bound(m_Map.begin(), m_Map.end(), fLength - fRadius);
    MapElementContainer::iterator upperIt = std::upper_bound(m_Map.begin(), m_Map.end(), fLength + fRadius);
    MapElementContainer::iterator it = lowerIt;
    for (; it != upperIt; ++it)
    {
      if (!it->second.bMarkAsDeleted &&
          (it->second.nLevel & nLevel) &&
          !(it->second.nLevel & nNotLevel) &&
          (it->second.vecWorldTPosition - vecPosition).getSquaredLength() < fSquaredRadius)
      {
        it->second.bMarkAsDeleted = true;
        ++nNumDeleted;
      }
    }
    return nNumDeleted;
  }


  CSWMap::CSWMap(const std::string& strName)
  : CSWEquipment(strName, Matrix44D(), 0, 0),
    m_pVisualizer(NULL)
  {
  }


  CSWMap::MapElementContainer::iterator CSWMap::findFirstElement(const Vector3D& vecPosition, double fRadius, unsigned long nLevel)
  {
    if (fRadius < 0)
      fRadius = 0;
    //first check if the position is near or outside the border 
  
    double fSquaredRadius = fRadius*fRadius;
    double fLength = vecPosition.getLength();
    MapElementContainer::iterator lowerIt = std::lower_bound(m_Map.begin(), m_Map.end(), fLength - fRadius);
    MapElementContainer::iterator upperIt = std::upper_bound(m_Map.begin(), m_Map.end(), fLength + fRadius);
    MapElementContainer::iterator it = lowerIt;
    for (; it != upperIt; ++it)
    {
      if (!it->second.bMarkAsDeleted &&
          (nLevel & it->second.nLevel) &&
          (it->second.vecWorldTPosition - vecPosition).getSquaredLength() < fSquaredRadius)
      {
        return it;
      }
    }
    return m_Map.end();
  }


  bool isElementNotValid(const CSWMap::ElementHelper& el)
  {
    return !el.second.isValid();  
  }


  bool operator < (const CSWMap::ElementHelper& lhs, const CSWMap::ElementHelper& rhs)
  {
    return lhs.first < rhs.first;
  }


  bool operator < (const CSWMap::ElementHelper& lhs, const double& rhs)
  {
    return lhs.first < rhs;
  }


  bool operator < (const double& lhs, const CSWMap::ElementHelper& rhs)
  {
    return lhs < rhs.first;
  }

}