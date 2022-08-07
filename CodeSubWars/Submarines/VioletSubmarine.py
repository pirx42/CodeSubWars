# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#the behavior of this submarine is very simple. it moves in a large circles. if it detects a submarine in front of them it will 
#immediately launches an ordinary torpedo. on collision it moves away from the collision point for awhile. on explosion it moves 
#forward for awhile.
#this shows that it is possible to implement a simple behavior without using commands. but it is not recommend to do that.
#its much more complex to reach an comparable behavior.

from CodeSubWars_Math import *
from CodeSubWars_Utilities import *

from CodeSubWars_Common import *

from CodeSubWars_Equipment import *
from CodeSubWars_Actuators import *
from CodeSubWars_Sensors import *
from CodeSubWars_Events import *
from CodeSubWars_Weapons import *
from CodeSubWars_Commands import *
from CodeSubWars_Submarine import *

#time period in seconds within no engine is changed
MAKE_NO_ENGINE_CHANGES_TIME = 15


class VioletSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.5, 0.1, 0.7, 1))

        #the time in seconds of the last collision detection
        self.lastDetectedEventTime = -9999

        #alway look in front direction        
        self.getControlCenter().getActiveSonar().setEnableAutomaticRotation(1)
        self.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        self.getControlCenter().getActiveSonar().setScanVelocityMode(ScanVelocityMode.FAST)
        self.getControlCenter().getActiveSonar().setScanRangeMode(ScanRangeMode.FAR_RANGE)
        self.getControlCenter().getActiveSonar().setScanDirection(Vector3(0, 0, 1))


    #this method must be overriden and is called every 10ms
    def update(self):
        #launch torpedo if submarine is in front and near to us
        angle = self.getControlCenter().getActiveSonar().getDirection().getAngleBetween(self.getControlCenter().getGyroCompass().getForwardDirection())
        if self.getControlCenter().getActiveSonar().hasTargetDetected() and \
           self.getControlCenter().getActiveSonar().isTargetSubmarine() and \
           self.getControlCenter().getActiveSonar().getDistance() < 500 and \
           angle < 10:

            self.getFrontLeftWeaponBattery().releaseNext(5)

        if CSWTime.getTime() - self.lastDetectedEventTime < MAKE_NO_ENGINE_CHANGES_TIME: return

        self.getMainEngine().setDirection(10, 0)
        self.getMainEngine().setIntensity(1)
      
        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):
        message = event.getMessage()
        if isinstance(message, CSWExplosionDetectedMessage):
            self.lastDetectedEventTime = CSWTime.getTime()

            #raise main engine to escape as fast as possible
            self.getMainEngine().setIntensity(1)

        elif isinstance(message, CSWCollisionDetectedMessage):
            self.lastDetectedEventTime = CSWTime.getTime()

            #main engine now points to forward direction
            self.getMainEngine().setDirection(0, 0)
            #determine the local position of the collision (local forward direction is along z axis)
            localPosition = self.makeLocalPosition(message.getPoint())
            if localPosition.z > 0:
                #collision point lies in front side: move backward 
                self.getMainEngine().setIntensity(-1)
            else:
                #collision point lies in back side: move forward
                self.getMainEngine().setIntensity(1)
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    
