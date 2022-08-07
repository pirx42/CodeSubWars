# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to deal with active and passive sonar. it extends the behavior of the white submarine.
#the passive sonar looks for high sound levels. once a high level sound is detected the active sonar scans closely 
#to this direction. if something was found the object will be scanned directly by the active sonar. if the object
#moves out of scanning direction the active sonar scans close to the last detected direction. if nothing was found
#the passive sonar resumes scanning.
#if an object is currently scanned directly it determines an approximate hit position based on target velocity, 
#change its direction to the hit position and fires two torpedos.
#its behavior bases on a table driven state machine.
#
#this also shows how code can be moved to seperate modules.

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
sys.path.append('./pylib/common')
from Statemachine import *
import Utilities

sys.path.append('./submarines/BlackSubmarineModules')
import BlackSubmarineUtilities
import BlackSubmarineCommands


class BlackSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.3, 0.3, 0.3, 1))

        #create the controller that implements the behavior of scanning
        self.scanController = StateMachine(State.scanFully, \
                                           [(State.scanFully,          somethingDetected,                         State.scanCloselyPassive),
                                            (State.scanCloselyPassive, objectProbablyDetected,                    State.scanCloselyActive),
                                            (State.scanCloselyActive,  hasTargetLostForAwhile,                    State.scanFully),
                                            (State.scanCloselyActive,  submarineDetected,                         State.trackObject),
                                            (State.trackObject,        Utilities.make_not_f1(submarineDetected),  State.scanCloselyActive)], \
                                           doOnStateTransition)
        #the reference to the submarine itself. this causes building a circular reference. leads to not deleting this submarine.
        #to avoid not deleting, in method finalize the reference is explicit cleared
        self.scanController.context.submarine = self
        #the time in seconds of the last detection
        self.scanController.context.lastDetectionTime = -999
        #the global direction of the last detection
        self.scanController.context.vecLastScannedGlobalDirection = Vector3(0, 0, 1)
        #the time in seconds of the last state change
        self.scanController.context.lastStateChangedTime = -999


    #this method must be overriden and is called every 10ms
    def update(self):
        #updates the controller to do available transitions depend on the given transition table
        self.scanController.updateStates()
        
        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    

    #make some finalizations
    def finalize(self):
        self.scanController.context.submarine = None
        
        

#on each transition store the current time
def doOnStateTransition(statemachinecontext):
    statemachinecontext.lastStateChangedTime = CSWTime.getTime()
    
    
#######   states    
###################################################

#state class that describes: nothing detected, fully scan the environment
class StateScanFully(State):
    def enter(self, statemachinecontext):
        scanFully(statemachinecontext.submarine)
        

#state class that describes: a little sound source was detected, try to detect the maximum direction of this source
class StateScanCloselyPassive(State):
    def enter(self, statemachinecontext):
        statemachinecontext.submarine.getControlCenter().getPassiveSonar().adjustDirectionToMaximum(1.5)
        
    def exit(self, statemachinecontext):
        statemachinecontext.vecLastScannedGlobalDirection = statemachinecontext.submarine.getControlCenter().getPassiveSonar().getDirection()


#state class that describes: the maximum direction of a sound source was detected, determine the distance by 
#using the active sonar
class StateScanCloselyActive(State):
    def enter(self, statemachinecontext):
        scanCloselyActive(statemachinecontext.submarine, statemachinecontext.vecLastScannedGlobalDirection)
        

#state class that describes: a submarine was detected, try to track its position and attack it
class StateTrackObject(State):
    def __init__(self): 
        self.timeEntered = -999
    
    def enter(self, statemachinecontext):
        self.timeEntered = CSWTime.getTime()
        trackObject(statemachinecontext.submarine, statemachinecontext.submarine.getControlCenter().getActiveSonar().getTargetPosition())

    def update(self, statemachinecontext):
        #if tracking object for at least 1 second try to attack it
        if CSWTime.getTime() - self.timeEntered > 1 and \
           not statemachinecontext.submarine.getFrontLeftWeaponBattery().isEmpty() and \
           not statemachinecontext.submarine.getCommandProcessor().isBusy():
            (resultValid, position) = BlackSubmarineUtilities.calculateHitPosition(statemachinecontext.submarine, statemachinecontext.submarine.getControlCenter().getActiveSonar().getTargetPosition())
            if resultValid:
                statemachinecontext.submarine.getCommandProcessor().execute(BlackSubmarineCommands.attackAtPosition(statemachinecontext.submarine, position))
        
    def exit(self, statemachinecontext):
        statemachinecontext.vecLastScannedGlobalDirection = statemachinecontext.submarine.getControlCenter().getActiveSonar().getDirection()


#the state instances
State.scanFully = StateScanFully()
State.scanCloselyPassive = StateScanCloselyPassive()
State.scanCloselyActive = StateScanCloselyActive()
State.trackObject = StateTrackObject()



#######   conditions
###################################################

#returns true if a sound was detected and the passive sonar is currently not self adjusting
def somethingDetected(statemachinecontext):
    return statemachinecontext.submarine.getControlCenter().getPassiveSonar().getLevel() > 60 and \
           not statemachinecontext.submarine.getControlCenter().getPassiveSonar().isAdjusting()

#returns true if the passive sonar has done its self adjusting to the highest sound
def objectProbablyDetected(statemachinecontext):
    return statemachinecontext.submarine.getControlCenter().getPassiveSonar().hasAdjusted()

#returns true if no target has been detected for time period of 2 seconds in the current state
def hasTargetLostForAwhile(statemachinecontext):
    return statemachinecontext.lastDetectionTime + 2 < CSWTime.getTime() and \
           statemachinecontext.lastStateChangedTime + 2 < CSWTime.getTime()

#returns true if a submarine was detected and stores the detection time
def submarineDetected(statemachinecontext):
    targetDetected = statemachinecontext.submarine.getControlCenter().getActiveSonar().hasTargetDetected() and \
                     statemachinecontext.submarine.getControlCenter().getActiveSonar().isTargetSubmarine()
    if targetDetected: statemachinecontext.lastDetectionTime = CSWTime.getTime()
    return targetDetected

    


#makes the active and passive sonar scanning the full range of 360 degrees.
#sets the axial inclination jet oar to rotate the submarine about main axis
def scanFully(submarine):
    submarine.getInclinationJetOar().setIntensity(0)
    submarine.getAxialInclinationJetOar().setIntensity(1)
    submarine.getBowsJetOar().setIntensity(0)

    submarine.getControlCenter().getPassiveSonar().setEnableAutomaticRotation(1)
    submarine.getControlCenter().getPassiveSonar().setScanDirectionMode(ScanDirectionMode.FULL)
    submarine.getControlCenter().getPassiveSonar().setScanVelocityMode(ScanVelocityMode.FAST)
    submarine.getControlCenter().getPassiveSonar().setScanRangeMode(ScanRangeMode.NEAR_RANGE)
    submarine.getControlCenter().getPassiveSonar().setAngleOfBeam(25)
    
    submarine.getControlCenter().getActiveSonar().setEnableAutomaticRotation(1)
    submarine.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.FULL)
    submarine.getControlCenter().getActiveSonar().setScanVelocityMode(ScanVelocityMode.FAST)
    submarine.getControlCenter().getActiveSonar().setScanRangeMode(ScanRangeMode.NEAR_RANGE)


#makes the active sonar scanning close to the given global direction
def scanCloselyActive(submarine, globalDirection):
    localDirection = submarine.makeLocalDirection(globalDirection)
    submarine.getControlCenter().getActiveSonar().setEnableAutomaticRotation(1)
    submarine.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
    submarine.getControlCenter().getActiveSonar().setScanVelocityMode(ScanVelocityMode.FAST)
    submarine.getControlCenter().getActiveSonar().setScanRangeMode(ScanRangeMode.FAR_RANGE)
    submarine.getControlCenter().getActiveSonar().setScanDirection(localDirection)

    
#makes the active sonar scanning directly to the given global position
def trackObject(submarine, globalPosition):
    submarine.getControlCenter().getActiveSonar().setEnableAutomaticRotation(0)
    submarine.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
    submarine.getControlCenter().getActiveSonar().pointToGlobalPosition(globalPosition)

