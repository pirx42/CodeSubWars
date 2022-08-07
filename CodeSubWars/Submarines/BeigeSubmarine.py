# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to create specialized commands. a command is derived from another python command. 
#the submarine behavior is very simple: it looks for a submarine and tries to keep a distance of 300m to it. 

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

import sys
sys.path.append('./pylib/commands')
import Commands


class BeigeSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.96, 0.96, 0.86, 1))


    #this method must be overriden and is called every 10ms
    def update(self):
        #if commands are processed, do nothing new
        if self.getCommandProcessor().isBusy():
            return
                    
        mapElement = self.getControlCenter().getMap().findNearestElement(self.getControlCenter().getGPS().getPosition(), 2000, DangerLevel.MEDIUM, 0)
        if mapElement.isValid():
            localDestination = self.makeLocalPosition(mapElement.vecWorldTPosition)
            if localDestination.getLength() > 200: 
                self.getCommandProcessor().execute(FollowCommand(self, mapElement))

        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1




#this command specializes the move command. it makes the submarine moves and follows a given map element. 
#the possible moving of the map element is detected by the active sonar. the active sonar is directed to 
#the last known map element position.
#@param param the submarine that should be moved (should be self).
#@param mapElement the map element to which the submarine should move and follow.
class FollowCommand(Commands.MoveCommand):
    def __init__(self, param = None, mapElement = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            Commands.MoveCommand.__init__(self, param)
            self.mapElement = param.mapElement
        else: 
            #construct a new command. the position is not relevant at this point.
            Commands.MoveCommand.__init__(self, param, Vector3(0, 0, 0), 1)
            self.mapElement = mapElement


    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        #initialize base class stuff
        Commands.MoveCommand.initialize(self)

        #setup active sonar to scan closely to target position to track the target
        self.submarine.getControlCenter().getActiveSonar().setEnableAutomaticRotation(1)
        self.submarine.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        self.submarine.getControlCenter().getActiveSonar().setScanVelocityMode(ScanVelocityMode.SLOW)
        self.submarine.getControlCenter().getActiveSonar().setScanRangeMode(ScanRangeMode.FAR_RANGE)
        self.submarine.getControlCenter().getActiveSonar().pointToGlobalPosition(self.mapElement.vecWorldTPosition)

        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        #update the information of the map element.
        self.mapElement = self.submarine.getControlCenter().getMap().findElementByID(self.mapElement.nID)
        if not self.mapElement.isValid():
            self.setProgress(1)
            self.finished()
            return

        #if the distance to the map element is less than 200m finish itself
        distance = self.submarine.makeLocalPosition(self.mapElement.vecWorldTPosition).getLength()
        if distance < 150: 
            self.setProgress(1)
            self.finished()
            return

        #adjust active sonar to scan at new position
        self.submarine.getControlCenter().getActiveSonar().pointToGlobalPosition(self.mapElement.vecWorldTPosition)

        #if close to target, adjust main engine intensity to get a distance of 200m
        self.mainEngineIntensity = (distance - 200)*1/30
        if self.mainEngineIntensity > 1: self.mainEngineIntensity = 1
        elif self.mainEngineIntensity < -1: self.mainEngineIntensity = -1

        #set new destination position to MoveCommand through a member variable
        self.destinationPosition = self.mapElement.vecWorldTPosition

        #continue moving to new set destination position with new main engine intensity
        Commands.MoveCommand.step(self)
        

    #this method must be overriden and is called directly after finishing of execution or on breaking.
    def cleanup(self):
        #clean up base class stuff
        Commands.MoveCommand.cleanup(self)

        #reset active sonar to scan total range
        self.submarine.getControlCenter().getActiveSonar().setEnableAutomaticRotation(1)
        self.submarine.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.FULL)
        self.submarine.getControlCenter().getActiveSonar().setScanVelocityMode(ScanVelocityMode.FAST)
        self.submarine.getControlCenter().getActiveSonar().setScanRangeMode(ScanRangeMode.NEAR_RANGE)

    
    #this method should be overriden and should return some interesting stuff about the current command state.
    def getDetails(self):
        return 'Follow map id ' + str(self.mapElement.nID) + ', Position ' + str(self.mapElement.vecWorldTPosition) + '' + \
               ', current distance ' + str(self.currentDistance) 