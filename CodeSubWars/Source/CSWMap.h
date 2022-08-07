// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWEquipment.h"

namespace CodeSubWars
{

  class CSWMapVisualizer;
  class CSWMap3DView;
  class CSWMapElement;

  /**
   * This class encapsulates functionality of a dynamic map. This is the central for object management e.g. gives base information
   * for routing decisions. Object positions from object detectors were reported to the map. Positions with regarding information 
   * can also be stored or manipulated manually. Once a position is inserted it will exist for 20 seconds if no redetection is done.
   * This completely works in world coordinate system.
   */
  class CSWMap : public CSWEquipment
  {
    public:
      typedef std::shared_ptr<CSWMap> PtrType;

      /**
       * The level of danger of objects in the map.
       */
      enum DangerLevel
      {
        UNKNOWN = 0,  ///< Unknown level (object was not identified)
        NONE = 1,     ///< No danger (object that was identified as friend)
        LOW = 2,      ///< Low level (object that was identified as harmless like stupid rocks)
        MEDIUM = 4,   ///< Medium level (object identified as submarine)
        HIGH = 8      ///< High level (object identified as weapon)
      };

      static PtrType create(const std::string& strName);

      virtual ~CSWMap();

      //defined methods from CSWEquipment
      virtual QWidget* getInformationWidget(QWidget* pParent);

      //defined methods from IUpdateable
      virtual void update();


      /**
       * Inserts an element with the given position and danger level to the map. That means an element is inserted or 
       * replacing an existing near element.
       * @param vecPosition The position of the element in world coordinate system that should be inserted.
       * @param nLevel The danger level regarding to the element.
       * @param nBitMask The bitmask that is ANDed to the found elements level before ORing the given new nLevel.\n
       *                 foundElement.nLevel' = (foundElement.nLevel & nBitMask) | nLevel
       * @return The id of the inserted element. If 0 is returned the insertion failed.
       */
      unsigned long insertElement(const Vector3D& vecPosition, unsigned long nLevel, unsigned long nBitMask = 0xffffffff);

      /**
       * Inserts an element to the map. That means an element is inserted or 
       * replacing an existing near element. The set velocity of the given element is ignored.
       * @param element The element that should be inserted.
       * @param nBitMask The bitmask that is ANDed to the found elements level before ORing the given new nLevel.\n 
       *                 foundElement.nLevel' = (foundElement.nLevel & nBitMask) | element.nLevel
       * @return The id of the inserted element. If 0 is returned the insertion failed.
       */
      unsigned long insertElement(const CSWMapElement& element, unsigned long nBitMask = 0xffffffff);
    
      /**
       * Returns true if the map contains nothing.
       * @return Returns true if the map contains nothing.
       */
      bool isEmpty() const;

      /**
       * Clears the map.
       * @post isEmpty() is true.
       */
      void clear();

      /**
       * Finds the element in the map which has the given id.
       * @param nID The id of the element which should be found.
       * @return Returns the found element. If nothing was found the returned element is invalid.
       */
      CSWMapElement findElementByID(unsigned long nID);
    
      /**
       * Finds the nearest element in the map that is in radius to the given position and with the given danger level.
       * @param vecPosition The position to which the nearest element should be found.
       * @param fRadius The radius around the given position that should be checked.
       * @param nLevel The danger level that must full fill the found element.
       * @param nNotLevel The danger level that must not full fill the found element.
       * @return Returns the found element. If nothing was found the returned element is invalid.
       */
      CSWMapElement findNearestElement(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel = 0);

      /**
       * Finds the elements in the map that are in radius to the given position and with the given danger level.
       * @param vecPosition The position to which the elements should be found.
       * @param fRadius The radius around the given position that should be checked.
       * @param nLevel The danger level that must full fill the found elements.
       * @param nNotLevel The danger level that must not full fill the found element.
       * @return Returns the found elements. If nothing was found the container is empty.
       */
      std::vector<CSWMapElement> findElements(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel = 0);

      /**
       * Removes elements in the map with the given danger level.
       * @param nLevel The danger level that must full fill the deleted elements.
       * @param nNotLevel The danger level that must not full fill the deleted element.
       * @return Returns the number of deleted elements.
       */
      int removeElements(unsigned long nLevel, unsigned long nNotLevel = 0);

      /**
       * Removes elements in the map that are in radius to the given position and with the given danger level.
       * @param vecPosition The position to which the elements should be deleted.
       * @param fRadius The radius around the given position that should be checked.
       * @param nLevel The danger level that must full fill the deleted elements.
       * @param nNotLevel The danger level that must not full fill the deleted element.
       * @return Returns the number of deleted elements.
       */
      int removeElements(const Vector3D& vecPosition, double fRadius, unsigned long nLevel, unsigned long nNotLevel = 0);

     
      friend CSWMap3DView; 
    protected:  
      typedef std::pair<double, CSWMapElement> ElementHelper;
      typedef std::list<ElementHelper> MapElementContainer;

      CSWMap(const std::string& strName);

      MapElementContainer::iterator findFirstElement(const Vector3D& vecPosition, double fRadius, unsigned long nLevel);

      friend bool isElementNotValid(const ElementHelper& el);

      friend bool operator < (const ElementHelper& lhs, const ElementHelper& rhs);
      friend bool operator < (const CSWMap::ElementHelper& lhs, const double& rhs);
      friend bool operator < (const double& lhs, const CSWMap::ElementHelper& rhs);

      static const double EQUAL_POSITION_DISTANCE;
    
      MapElementContainer m_Map;
      CSWMapVisualizer* m_pVisualizer;
  };

  bool isElementNotValid(const CSWMap::ElementHelper& el);

  bool operator < (const CSWMap::ElementHelper& lhs, const CSWMap::ElementHelper& rhs);
  bool operator < (const CSWMap::ElementHelper& lhs, const double& rhs);
  bool operator < (const double& lhs, const CSWMap::ElementHelper& rhs);



  /**
   * This represents a single element of a CSWMap e.g. an object reported by sonar. 
   */
  class CSWMapElement
  {
    public:
      //a default contructed element is always invalid
      CSWMapElement() 
      : nID(0),
        vecWorldTPosition(0, 0, 0), 
        vecWorldTVelocity(0, 0, 0), 
        nLevel(0), 
        nUserData(0),
        fTime(-1000), 
        m_Velocities(5),
        bMarkAsDeleted(false)
      {
      }

      CSWMapElement(const CSWMapElement& other)
      : nID(other.nID),
        vecWorldTPosition(other.vecWorldTPosition), 
        vecWorldTVelocity(other.vecWorldTVelocity), 
        nLevel(other.nLevel), 
        nUserData(other.nUserData),
        fTime(other.fTime), 
        m_Velocities(other.m_Velocities),
        bMarkAsDeleted(other.bMarkAsDeleted)
      {
      } 

      CSWMapElement(const Vector3D& vecPos, const Vector3D& vecVel, unsigned int lvl, double tme) 
      : nID(0),
        vecWorldTPosition(vecPos), 
        vecWorldTVelocity(vecVel), 
        nLevel(lvl), 
        nUserData(0),
        fTime(tme), 
        m_Velocities(5),
        bMarkAsDeleted(false)
      {
      }
    
      /**
      * Returns if the element is valid.
      * @return Returns true if the element is valid otherwise false.
      */
      bool isValid() const
      {
        return ARSTD::Time::getTime() - fTime < 20 && !bMarkAsDeleted && nID;
      }

      unsigned long nID;              ///< The id of the element. It is ensured that this id is system wide unique. Only inserted elements
                                      ///< have a valid id.
      Vector3D vecWorldTPosition;     ///< The position regarding to the element in world coordinate system.
                                      ///<
      Vector3D vecWorldTVelocity;     ///< The approximate velocity regarding to the element in m/s in world coordinate system. This is calculated from 
                                      ///< the detection difference of the current and the last position and time.\n
                                      ///< vApprox = (vecCurrent - vecLast)/(timeCurrent - timeLast) 
      unsigned long nLevel;           ///< The danger level regarding to the element.
                                      ///<
      long nUserData;                 ///< Data that can be set by the user.
                                      ///<
      double fTime;
      boost::circular_buffer<Vector3D> m_Velocities;
      bool bMarkAsDeleted;
  };


  bool operator == (const CSWMapElement& lhs, const CSWMapElement& rhs);

}