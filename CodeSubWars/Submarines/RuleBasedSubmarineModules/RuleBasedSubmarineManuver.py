# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#niedriger priorisierte bedingungen duerfen von hoeher priorisierten unternrochen werden. aber nicht andersherum.

# States:
# AvoidHit: darf nicht unterbrochen werden
# AvoidCollision: darf nicht unterbrochen werden
# AvoidFutureCollision: kann unterbrochen werden
# Escape: kann unterbrochen werden
# Recharge: kann unterbrochen werden
# Attack: kann unterbrochen werden
# KeepTrack: kann unterbrochen werden
# Search: kann unterbrochen werden
#
#
#manuvering with priority
# 1. hit -> (AvoidHit) store commands; move 200m forward as fast as possible to prevent further hits; resume previous command
# 2. collision -> (AvoidCollision) store commands; move 200m in opposite direction to collision position; resume previous command
# 3. object on own course -> (AvoidFutureCollision) store commands; move 200m perpendicular to current direction to prevent collision; resume previous command
# 4. no weapons:
#   4.1. enemy in near (<500m) -> (Escape) store commands; move 300m perpendicular to current direction to prevent attacking from this enemy; resume previous command
#   4.2. weaponSupply available -> (Recharge) move and recharge at nearest supply
# 5. weapons available:
#   5.1. enemy in near (<500m) -> (Attack) try to predict moving path using splines and attack at predicted position
#   5.2. enemy in far (>500m) -> (KeepTrack) try to predict moving path using splines and move to be less than 400m away from predicted position
#   5.3. no detected enemy -> (Search) search enemy
#

#transition
# * except AvoidHit, hit -> AvoidHit
# * except AvoidHit/AvoidCollision, collision -> AvoidCollision
# * except AvoidHit/AvoidCollision/AvoidFutureCollision, object between current and next moving position (on own course) -> AvoidFutureCollision
# * except AvoidHit/AvoidCollision/AvoidFutureCollision/Escape, no weapons and enemy in near -> Escape
# * except AvoidHit/AvoidCollision/AvoidFutureCollision/Escape/Recharge, no weapons -> Recharge
# * except AvoidHit/AvoidCollision/AvoidFutureCollision/Escape/Recharge/Attack, weapons available and enemy in near -> Attack
# * except AvoidHit/AvoidCollision/AvoidFutureCollision/Escape/Recharge/Attack/KeepTrack, weapons available and enemy in far -> KeepTrack
# * except AvoidHit/AvoidCollision/AvoidFutureCollision/Escape/Recharge/Attack/KeepTrack, weapons available and no enemy -> Search



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
sys.path.append('./pylib/Commands')
from Statemachine import *
import Utilities
import Commands

sys.path.append('./submarines/RuleBasedSubmarineModules')
import Constants
import RuleBasedSubmarineCommands


def updateAction(submarine):
    pass











#on each transition store the current time
def doOnStateTransition(statemachinecontext):
    statemachinecontext.lastStateChangedTime = CSWTime.getTime()
    

#######   states    
###################################################

#state class that describes: nothing detected, fully scan the environment
class AvoidHit(State):
    def enter(self, statemachinecontext):
        print('enter AvoidHit')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().push()
        sub.getCommandProcessor().cleanup()
    
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return
          
        sub.getCommandProcessor().execute(CSWCommands.moveForward(sub, 200, 0, 100))

    def exit(self, statemachinecontext):
        print('exit AvoidHit')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().pop()

class AvoidCollision(State):
    def enter(self, statemachinecontext):
        print('enter AvoidCollision')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().push()
        sub.getCommandProcessor().cleanup()

    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return

        localHitPos = sub.makeLocalPosition(statemachinecontext.vecWorldTLastHitPosition)
        if localHitPos.z < 0:
            sub.getCommandProcessor().execute(CSWCommands.moveForward(sub, 200, 0, 100))
        else:
            sub.getCommandProcessor().execute(CSWCommands.moveForward(sub, -200, 0, 100))
    
    def exit(self, statemachinecontext):
        print('exit AvoidCollision')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().pop()

class AvoidFutureCollision(State):
    def enter(self, statemachinecontext):
        print('enter AvoidFutureCollision')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().push()
        sub.getCommandProcessor().cleanup()
    
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return

        destinationPosition = sub.getControlCenter().getGyroCompass().getUpDirection()*300
        sub.getCommandProcessor().execute(Commands.MoveCommand(sub, destinationPosition))

    def exit(self, statemachinecontext):
        print('exit AvoidFutureCollision')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().pop()

class Escape(State):
    def enter(self, statemachinecontext):
        print('enter Escape')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().push()
        sub.getCommandProcessor().cleanup()
    
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return

        destinationPosition = sub.getControlCenter().getGPS().getPosition() + sub.getControlCenter().getGyroCompass().getUpDirection()*400
        sub.getCommandProcessor().execute(Commands.MoveCommand(sub, destinationPosition))

    def exit(self, statemachinecontext):
        print('exit Escape')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().pop()

class Recharge(State):
    def enter(self, statemachinecontext):
        print('enter Recharge')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().cleanup()
         
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return

        #if the distance to the supply is more than the recharge radius move to its near        
        directionToSupply = statemachinecontext.vecWorldTWeaponSupplyPosition - sub.getControlCenter().getGPS().getPosition()
        moveNearToSupply = directionToSupply.getLength() > 280
        if moveNearToSupply:
            targetPosition = statemachinecontext.vecWorldTWeaponSupplyPosition - directionToSupply.getNormalized()*200
            sub.getCommandProcessor().execute(Commands.MoveCommand(sub, targetPosition))
            
        sub.getCommandProcessor().execute(CSWRechargeWeaponBatteryCommand.create(sub.getFrontLeftWeaponBattery(), "envWeaponSupply", WeaponResource.GREEN_TORPEDO_ID))
        sub.getCommandProcessor().execute(CSWRechargeWeaponBatteryCommand.create(sub.getFrontRightWeaponBattery(), "envWeaponSupply", WeaponResource.RED_TORPEDO_ID))
        sub.getCommandProcessor().execute(CSWRechargeWeaponBatteryCommand.create(sub.getBackWeaponBattery(), "envWeaponSupply", WeaponResource.GREEN_MINE_ID))
        sub.getCommandProcessor().execute(CSWWaitCommand.create(15))
        
        if moveNearToSupply:
            #if moved to near to supply now move back to where come from
            sub.getCommandProcessor().execute(Commands.MoveCommand(sub, sub.getControlCenter().getGPS().getPosition()))

    def exit(self, statemachinecontext):
        print('exit Recharge')

class Attack(State):
    def enter(self, statemachinecontext):
        print('enter Attack')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().cleanup()
    
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return
          
        sub.getCommandProcessor().execute(CSWCommands.setOrientationAbsoluteTowardPosition(sub, statemachinecontext.vecWorldTEnemyPosition, 0.1, 100))
        sub.getCommandProcessor().execute(RuleBasedSubmarineCommands.FireCommand(sub, statemachinecontext.vecWorldTEnemyPosition))
        sub.getCommandProcessor().execute(CSWWaitCommand.create(1.1))
        sub.getCommandProcessor().execute(RuleBasedSubmarineCommands.FireCommand(sub, statemachinecontext.vecWorldTEnemyPosition))

    def exit(self, statemachinecontext):
        print('exit Attack')

class KeepTrack(State):
    def enter(self, statemachinecontext):
        print('enter KeepTrack')
        sub = statemachinecontext.submarine
        sub.getCommandProcessor().cleanup()
    
    def update(self, statemachinecontext):
        sub = statemachinecontext.submarine
        if sub.getCommandProcessor().isBusy(): return

        destinationDir = statemachinecontext.vecWorldTEnemyPosition - sub.getControlCenter().getGPS().getPosition()
        distance = destinationDir.getLength()
        destinationPos = sub.getControlCenter().getGPS().getPosition() + destinationDir*((distance - 400)/distance)
        sub.getCommandProcessor().execute(Commands.MoveCommand(sub, destinationPos))

    def exit(self, statemachinecontext):
        print('exit KeepTrack')

class Search(State):
    def enter(self, statemachinecontext):
        print('enter Search')
        sub = statemachinecontext.submarine
        sub.getAxialInclinationJetOar().setIntensity(1)
    
    def update(self, statemachinecontext):
        pass

    def exit(self, statemachinecontext):
        print('exit Search')
        sub = statemachinecontext.submarine
        sub.getAxialInclinationJetOar().setIntensity(0)


#the state instances
State.avoidHit = AvoidHit()
State.avoidCollision = AvoidCollision()
State.AvoidFutureCollision = AvoidFutureCollision()
State.escape = Escape()
State.recharge = Recharge()
State.attack = Attack()
State.keepTrack = KeepTrack()
State.search = Search()



#######   conditions
###################################################

def hitDetected(statemachinecontext):
    return statemachinecontext.wasHit


def collisionDetected(statemachinecontext):
    return statemachinecontext.wasCollided


def objectOnOwnCourseDetected(statemachinecontext):
    vecWorldTVelocity = statemachinecontext.submarine.getControlCenter().getMovingPropertiesSensor().getVelocity()
    vecWorldTCurrentPosition = statemachinecontext.submarine.getControlCenter().getGPS().getPosition()
    vecWorldTPositionIn5Seconds = vecWorldTCurrentPosition + vecWorldTVelocity*5
    fScanRadius = (vecWorldTCurrentPosition - vecWorldTPositionIn5Seconds).getLength()
    dangerLevel = DangerLevel.NONE | DangerLevel.LOW | DangerLevel.MEDIUM | DangerLevel.HIGH
    
    objectFound = statemachinecontext.submarine.getControlCenter().getMap().findNearestElement(vecWorldTPositionIn5Seconds, fScanRadius, dangerLevel, 0)
    if objectFound.isValid():
        statemachinecontext.vecWorldTObjectOnCoursePosition = objectFound.vecWorldTPosition
    return objectFound.isValid()


def hasWeapons(statemachinecontext):
    return not statemachinecontext.submarine.getFrontLeftWeaponBattery().isEmpty()


def weaponSupplyDetected(statemachinecontext):
    vecWorldTCurrentPosition = statemachinecontext.submarine.getControlCenter().getGPS().getPosition()
    
    nearestSupply = statemachinecontext.submarine.getControlCenter().getMap().findNearestElement(vecWorldTCurrentPosition, 3000, Constants.WEAPON_SUPPLY_ID, 0)
    if nearestSupply.isValid():
        statemachinecontext.vecWorldTWeaponSupplyPosition = nearestSupply.vecWorldTPosition
    return nearestSupply.isValid()


def nearEnemyDetected(statemachinecontext):
    vecWorldTCurrentPosition = statemachinecontext.submarine.getControlCenter().getGPS().getPosition()
    
    submarineFound = statemachinecontext.submarine.getControlCenter().getMap().findNearestElement(vecWorldTCurrentPosition, 500, DangerLevel.MEDIUM, Constants.WEAPON_SUPPLY_ID)
    if submarineFound.isValid():
        statemachinecontext.vecWorldTEnemyPosition = submarineFound.vecWorldTPosition
    return submarineFound.isValid()


def farEnemyDetected(statemachinecontext):
    vecWorldTCurrentPosition = statemachinecontext.submarine.getControlCenter().getGPS().getPosition()
    
    submarineFound = statemachinecontext.submarine.getControlCenter().getMap().findNearestElement(vecWorldTCurrentPosition, 3000, DangerLevel.MEDIUM, Constants.WEAPON_SUPPLY_ID)
    if submarineFound.isValid():
        statemachinecontext.vecWorldTEnemyPosition = submarineFound.vecWorldTPosition
    return submarineFound.isValid()


def noEnemyDetected(statemachinecontext):
    vecWorldTCurrentPosition = statemachinecontext.submarine.getControlCenter().getGPS().getPosition()
    
    submarineFound = statemachinecontext.submarine.getControlCenter().getMap().findNearestElement(vecWorldTCurrentPosition, 3000, DangerLevel.MEDIUM, Constants.WEAPON_SUPPLY_ID)
    return not submarineFound.isValid()


State.avoidHit = AvoidHit()
State.avoidCollision = AvoidCollision()
State.avoidFutureCollision = AvoidFutureCollision()
State.escape = Escape()
State.recharge = Recharge()
State.attack = Attack()
State.keepTrack = KeepTrack()
State.search = Search()


manuverController = StateMachine(State.search, 
                                 [(State.AvoidFutureCollision, hitDetected, State.avoidHit),
                                  (State.escape,                hitDetected, State.avoidHit),
                                  (State.recharge,              hitDetected, State.avoidHit),
                                  (State.attack,                hitDetected, State.avoidHit),
                                  (State.keepTrack,             hitDetected, State.avoidHit),
                                  (State.search,                hitDetected, State.avoidHit),
                                 
                                  (State.AvoidFutureCollision, collisionDetected, State.avoidCollision),
                                  (State.escape,                collisionDetected, State.avoidCollision),
                                  (State.recharge,              collisionDetected, State.avoidCollision),
                                  (State.attack,                collisionDetected, State.avoidCollision),
                                  (State.keepTrack,             collisionDetected, State.avoidCollision),
                                  (State.search,                collisionDetected, State.avoidCollision),
                                 
                                  (State.escape,                objectOnOwnCourseDetected, State.avoidFutureCollision),
                                  (State.recharge,              objectOnOwnCourseDetected, State.avoidFutureCollision),
                                  (State.attack,                objectOnOwnCourseDetected, State.avoidFutureCollision),
                                  (State.keepTrack,             objectOnOwnCourseDetected, State.avoidFutureCollision),
                                  (State.search,                objectOnOwnCourseDetected, State.avoidFutureCollision),

                                  (State.recharge,              Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), nearEnemyDetected), State.escape),
                                  (State.attack,                Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), nearEnemyDetected), State.escape),
                                  (State.keepTrack,             Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), nearEnemyDetected), State.escape),
                                  (State.search,                Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), nearEnemyDetected), State.escape),

                                  (State.attack,                Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), weaponSupplyDetected), State.recharge),
                                  (State.keepTrack,             Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), weaponSupplyDetected), State.recharge),
                                  (State.search,                Utilities.make_and_f2(Utilities.make_not_f1(hasWeapons), weaponSupplyDetected), State.recharge),

                                  (State.keepTrack,             Utilities.make_and_f2(hasWeapons, nearEnemyDetected), State.attack),
                                  (State.search,                Utilities.make_and_f2(hasWeapons, nearEnemyDetected), State.attack),

                                  (State.search,                Utilities.make_and_f2(hasWeapons, farEnemyDetected), State.keepTrack)],

#                                  (State.search,                Utilities.make_and_f2(hasWeapons, noEnemyDetected), State.search)],
                                 doOnStateTransition)
#the time in seconds of the last state change
manuverController.context.lastStateChangedTime = -999

manuverController.context.vecWorldTObjectOnCoursePosition = Vector3(0, 0, 0)
manuverController.context.vecWorldTEnemyPosition = Vector3(0, 0, 0)
manuverController.context.vecWorldTWeaponSupplyPosition = Vector3(0, 0, 0)