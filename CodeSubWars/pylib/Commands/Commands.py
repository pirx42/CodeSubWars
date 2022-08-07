# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#common commands

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



#selects the best rotation command toward a global position depend on rotation angle.
#@param submarine the submarine that should be rotated (should be self).
#@param position the global position where the submarine should rotate toward.
def rotateCommand(submarine, position):
    destinationDir = position - submarine.getControlCenter().getGPS().getPosition()
    if destinationDir.getAngleBetween(submarine.getControlCenter().getGyroCompass().getForwardDirection()) > 4:
        return FastRotateCommand(submarine, position)
    else:
        return CSWCommands.setOrientationAbsoluteTowardPosition(submarine, position, 0.1, 10)


#this command makes the submarine move to given positions. for rotation the main engine, bows jet oar and 
#inclination jet oar is used. this rotates faster than the original given command.
#@param submarine the submarine that should be moved (should be self).
#@param destinationPosition the global position where the submarine should move to.
#@param moveForwardToDestination indicates whether the moving should be forward (default) or backward to destination
#@warning this command can not handle moving to positions that are near (<90m) to the initial submarine 
#         position.
class MoveCommand(CSWPyCommand):
    def __init__(self, param = None, destinationPosition = None, moveForwardToDestination = 1):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.destinationPosition = param.destinationPosition
            self.moveForwardToDestination = param.moveForwardToDestination
            self.currentDistance = param.currentDistance
            self.mainEngineIntensity = param.mainEngineIntensity
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.destinationPosition = destinationPosition
            self.moveForwardToDestination = moveForwardToDestination
            self.currentDistance = 0
            self.mainEngineIntensity = 1

    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        self.currentDistance = 0
        self.mainEngineIntensity = 1
        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        #we didn't know the correct progress, so approximate with 50% 
        self.setProgress(0.5)
        #transform the destination position into the local (submarine) coordinate system
        localDestination = self.submarine.makeLocalPosition(self.destinationPosition)
        localDestinationDirection = localDestination.getNormalized()

        isMovingForward = self.mainEngineIntensity > 0
        if isMovingForward:
            if localDestination.z > 0:
                #target is in front, the main engine must be directed to opposite direction of the target
                localEngineDirection = Vector3(-localDestinationDirection.x, -localDestinationDirection.y, localDestinationDirection.z)
                mainEngineIntensity = self.mainEngineIntensity
            else:
                #target is in back, the main engine must be directed to same direction as the target
                localEngineDirection = Vector3(localDestinationDirection.x, localDestinationDirection.y, localDestinationDirection.z)
                mainEngineIntensity = -self.mainEngineIntensity
        else:
            if localDestination.z < 0:
                #target is in back, the main engine must be directed to opposite direction of the target
                localEngineDirection = Vector3(-localDestinationDirection.x, -localDestinationDirection.y, localDestinationDirection.z)
                mainEngineIntensity = -self.mainEngineIntensity
            else:
                #target is in front, the main engine must be directed to same direction as the target
                localEngineDirection = Vector3(localDestinationDirection.x, localDestinationDirection.y, localDestinationDirection.z)
                mainEngineIntensity = self.mainEngineIntensity
        
        #invert directions on moving backward to destination
        if self.moveForwardToDestination == 0:
            localDestinationDirection = localDestinationDirection*-1
            localEngineDirection = localEngineDirection*-1
                
        #set the new direction and intensity
        self.submarine.getMainEngine().setDirection(localEngineDirection)
        self.submarine.getMainEngine().setIntensity(mainEngineIntensity)
        #set the new intensity of the jet oars
        self.submarine.getBowsJetOar().setIntensity(-localDestinationDirection.x*5)
        self.submarine.getInclinationJetOar().setIntensity(localDestinationDirection.y*5)

        #if the distance to the target is below 90m finish itself 
        self.currentDistance = localDestination.getLength()
        if self.currentDistance < 90: 
            self.setProgress(1)
            self.finished()

    #this method must be overriden and is called directly after finishing of execution or on breaking.
    def cleanup(self):
        self.submarine.getBowsJetOar().setIntensity(0)
        self.submarine.getInclinationJetOar().setIntensity(0)
        self.submarine.getMainEngine().setIntensity(0)
        self.submarine.getMainEngine().setDirection(Vector3(0, 0, 1))

    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__

    #this method should be overriden and should return some interesting stuff about the current command state.
    def getDetails(self):
        return 'Move to ' + str(self.destinationPosition.x) + ' ' + str(self.destinationPosition.y) + ' ' + str(self.destinationPosition.z) + \
               ', current distance ' + str(self.currentDistance) 


#this command makes the submarine rotates fast toward a given global position. this rotation uses main 
#engine and jet oars.
#@param submarine the submarine that should be rotated (should be self).
#@param destinationPosition the global position where the submarine should rotate toward.
#@warning this command is slow if a rotation below 2 degree must be done.
class FastRotateCommand(CSWPyCommand):
    def __init__(self, param = None, destinationPosition = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.destinationPosition = param.destinationPosition
            self.currentAngle = param.currentAngle
            self.flipper = param.flipper
            self.startTime = param.startTime
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            #avoid destination position near current position
            destinationDir = destinationPosition - self.submarine.getControlCenter().getGPS().getPosition()
            self.destinationPosition = destinationPosition + destinationDir.getNormalized()*200
            self.currentAngle = 0
            self.flipper = 1
            self.startTime = CSWTime.getTime()

    def initialize(self):
        self.currentAngle = 0
        self.flipper = 1
        self.startTime = CSWTime.getTime()
        
    def step(self):
        self.setProgress(0.5)
        localDestination = self.submarine.makeLocalPosition(self.destinationPosition)
        localDestinationDirection = localDestination.getNormalized()

        if self.flipper > 0:
            localEngineDirection = Vector3(-localDestinationDirection.x, -localDestinationDirection.y, localDestinationDirection.z)
            mainEngineIntensity = 1
        else:
            localEngineDirection = Vector3(localDestinationDirection.x, localDestinationDirection.y, localDestinationDirection.z)
            mainEngineIntensity = -1
        
        self.submarine.getMainEngine().setDirection(localEngineDirection)
        self.submarine.getMainEngine().setIntensity(mainEngineIntensity)
        self.submarine.getBowsJetOar().setIntensity(-localDestinationDirection.x*5)
        self.submarine.getInclinationJetOar().setIntensity(localDestinationDirection.y*5)

        if self.startTime + 5 < CSWTime.getTime():
            self.flipper = -self.flipper
            self.startTime = CSWTime.getTime()
        
        self.currentAngle = localDestinationDirection.getAngleBetween(Vector3(0, 0, 1))
        if self.currentAngle < 0.3:
            self.setProgress(1)
            self.finished() 

    def cleanup(self):
        self.submarine.getBowsJetOar().setIntensity(0)
        self.submarine.getInclinationJetOar().setIntensity(0)
        self.submarine.getMainEngine().setIntensity(0)
        self.submarine.getMainEngine().setDirection(Vector3(0, 0, 1))

    def getName(self): return self.__class__.__name__

    def getDetails(self):
        return 'Rotate toward ' + str(self.destinationPosition.x) + ' ' + str(self.destinationPosition.y) + ' ' + str(self.destinationPosition.z) + \
               ', current angle ' + str(self.currentAngle) 
