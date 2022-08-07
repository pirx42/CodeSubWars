# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#scanning with priority
# 1. scanning for enemies
# 2. [far explosions detected -> try to detect submarines in that direction]


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
sys.path.append('./pylib/Common')
from Statemachine import *
from Utilities import *

sys.path.append('./submarines/RuleBasedSubmarineModules')
import Constants


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
        

#the state instances
State.scanFully = StateScanFully()
State.scanCloselyPassive = StateScanCloselyPassive()
State.scanCloselyActive = StateScanCloselyActive()



#######   conditions
###################################################

#returns true if a sound was detected and the passive sonar is currently not self adjusting
def somethingDetected(statemachinecontext):
    return statemachinecontext.submarine.getControlCenter().getPassiveSonar().getLevel() > 60 and \
           not statemachinecontext.submarine.getControlCenter().getPassiveSonar().isAdjusting()

#returns true if the passive sonar has done its self adjusting to the highest sound
def objectProbablyDetected(statemachinecontext):
    return statemachinecontext.submarine.getControlCenter().getPassiveSonar().hasAdjusted()

#returns true if 2 seconds in the current state
def hasScannedForAwhile(statemachinecontext):
    return statemachinecontext.lastStateChangedTime + 2 < CSWTime.getTime()

#returns true if a submarine was detected
def submarineDetected(statemachinecontext):
    return statemachinecontext.submarine.getControlCenter().getActiveSonar().hasTargetDetected() and \
           statemachinecontext.submarine.getControlCenter().getActiveSonar().isTargetSubmarine()



#######   statemachine instance
###################################################

scanController = StateMachine(State.scanFully, \
                              [(State.scanFully,          somethingDetected,      State.scanCloselyPassive),
                               (State.scanCloselyPassive, objectProbablyDetected, State.scanCloselyActive),
                               (State.scanCloselyActive,  hasScannedForAwhile,    State.scanFully),
                               (State.scanCloselyActive,  submarineDetected,      State.scanFully)], \
                              doOnStateTransition)
#the time in seconds of the last detection
scanController.context.lastDetectionTime = -999                                          
#the global direction of the last detection
scanController.context.vecLastScannedGlobalDirection = Vector3(0, 0, 1)
#the time in seconds of the last state change
scanController.context.lastStateChangedTime = -999



#######   methods for controlling on submarine
###################################################


#makes the active and passive sonar scanning the full range of 360 degrees.
def scanFully(submarine):
    submarine.getInclinationJetOar().setIntensity(0)
    submarine.getAxialInclinationJetOar().setIntensity(0)
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
