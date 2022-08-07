// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSoundVisualizer.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "glut.h"


namespace CodeSubWars
{

  CSWSoundVisualizer::PtrType CSWSoundVisualizer::create()
  {
    return PtrType(new CSWSoundVisualizer());
  }


  CSWSoundVisualizer::~CSWSoundVisualizer()
  {
  }


  void CSWSoundVisualizer::addSound(const Vector3D& vecPosition, const double& fIntensity, long nEmitterID)
  {
    //explosion can only be added in graphic mode
    if (!qApp)
      return;

    if (!(CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_SOUND_EMITTING))
      return;

    double fIntensityIndB = 20*ARSTD::log10(fIntensity);

    SoundEntry e;
    e.vecPosition = vecPosition;
    e.fStartIntensity = fIntensityIndB < 250 ? fIntensityIndB : 250;
    e.fStartTime = ARSTD::Time::getTime();

    SoundContainer::iterator itFound = m_Sounds.find(nEmitterID);
    if (itFound == m_Sounds.end())
    {
      SoundEmits emits;
      emits[e.fStartTime*2] = e;
      m_Sounds[nEmitterID] = emits;
    }
    else
    {
      itFound->second[e.fStartTime*2] = e;
    }
  }


  void CSWSoundVisualizer::clear()
  {
    m_Sounds.clear();
  }


  void CSWSoundVisualizer::draw()
  {
    if (m_Sounds.empty())
      return;
  
    SoundContainer newSounds;
  
    double fCurrentTime = ARSTD::Time::getTime();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glEnable(GL_DEPTH_TEST);
      glDepthMask(false);

      glShadeModel(GL_SMOOTH);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
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
      SoundContainer::iterator itSounds = m_Sounds.begin();
      for (; itSounds != m_Sounds.end(); ++itSounds)
      {
        SoundEmits newEmits;
        SoundEmits::iterator itEmits = itSounds->second.begin();
        for (; itEmits != itSounds->second.end(); ++itEmits)
        {
          double fElapsedTime = fCurrentTime - itEmits->second.fStartTime;
          double fRadius = 100*fElapsedTime;
          double fAlpha = 1*(1/(fElapsedTime + 1) - 0.15 - (0.5 - 0.5*itEmits->second.fStartIntensity/250));
          if (fAlpha > 0)
          {
            glColor4d(0.1, 0.2, 1.0, fAlpha);
            mat.getTranslation() = itEmits->second.vecPosition;
            glLoadMatrixd(mat.pData);

            gluSphere(pSphere, fRadius, 10, 10);
            newEmits.insert(*itEmits);
          }
        }
  
        if (!newEmits.empty())
        {
          itSounds->second = newEmits;
          newSounds.insert(*itSounds);
        }
      }

      glDepthMask(true);
      glPopMatrix();
      gluDeleteQuadric(pSphere);
    glPopAttrib();

    m_Sounds.swap(newSounds);
  }


  CSWSoundVisualizer::CSWSoundVisualizer()
  {
  }

}