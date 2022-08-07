// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWExplosionVisualizer.h"
#include "glut.h"

namespace CodeSubWars
{

  CSWExplosionVisualizer::PtrType CSWExplosionVisualizer::create()
  {
    return PtrType(new CSWExplosionVisualizer());
  }


  CSWExplosionVisualizer::~CSWExplosionVisualizer()
  {
  }


  void CSWExplosionVisualizer::addExplosion(const Vector3D& vecPosition, const double& fRadius)
  {
    //explosion can only be added in graphic mode
    if (!qApp)
      return;

    ExplosionEntry e;
    e.vecPosition = vecPosition;
    e.fRadius = fRadius;
    e.fTimeToStop = ARSTD::Time::getTime() + fRadius/50.0*0.25;
    m_Explosions.push_back(e);
  }


  void CSWExplosionVisualizer::clear()
  {
    m_Explosions.clear();
  }


  void CSWExplosionVisualizer::draw()
  {
    if (m_Explosions.empty())
      return;
  
    ExplosionContainer newExplosions;
    newExplosions.reserve(m_Explosions.size());
  
    double fCurrentTime = ARSTD::Time::getTime();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glEnable(GL_DEPTH_TEST);
      glDepthMask(false);
      glDisable(GL_LIGHTING);
      glEnable(GL_COLOR_MATERIAL);

      glPolygonMode(GL_FRONT, GL_FILL);

      glCullFace(GL_BACK);    
      glEnable(GL_CULL_FACE);
    
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      GLUquadricObj* pSphere = gluNewQuadric();
      gluQuadricDrawStyle(pSphere, GLU_FILL);
      Matrix44D mat;
      ExplosionContainer::iterator it = m_Explosions.begin();
      for (; it != m_Explosions.end(); ++it)
      {
        if (fCurrentTime < it->fTimeToStop)
        {
          assert(50.0/it->fRadius*2*(it->fTimeToStop - fCurrentTime) <= 0.5);
          glColor4d(1.0, 0.5, 0.1, 50.0/it->fRadius*2*(it->fTimeToStop - fCurrentTime));
          mat.getTranslation() = it->vecPosition;
          glLoadMatrixd(mat.pData);

          gluSphere(pSphere, it->fRadius, 10, 10);
        
          it->bDrawn = true;
          newExplosions.push_back(*it);    
        }
        else if (!it->bDrawn)
        {
          //at least one 'flash' must be drawn
          glColor4d(1.0, 0.5, 0.1, 0.5);
          mat.getTranslation() = it->vecPosition;
          glLoadMatrixd(mat.pData);

          gluSphere(pSphere, it->fRadius, 10, 10);
        }
      }

      glDepthMask(true);
      glPopMatrix();
      gluDeleteQuadric(pSphere);
    glPopAttrib();

    m_Explosions.swap(newExplosions);
  }


  CSWExplosionVisualizer::CSWExplosionVisualizer()
  {
  }

}