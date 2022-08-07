# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to deal with its map. it looks for the nearest object in the map and
#attacks this object. if the weapon batteries are empty it tries to recharge them on a weapon
#supply (if available).

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


WEAPON_SUPPLY_ID = 1024



class GreenSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0, 1, 0.1, 1))
        
        #dictionary that holds additional information to id's of objects stored in the map
        self.dictObjectInformation = {}
        
        
    #this method must be overriden and is called every 10ms
    def update(self):
        #if command processor is busy do nothing
        if self.getCommandProcessor().isBusy():
            return

        #if front left weapon battery is empty try to recharge
        if self.getFrontLeftWeaponBattery().isEmpty():
            rechargeWeaponBatteries(self)
            return

        #find the destination position that is nearest to the current orientation. this method of 
        #search may not be the best.
        positionInFront = self.getControlCenter().getGPS().getPosition() + \
                          self.getControlCenter().getGyroCompass().getForwardDirection()*400.0

        #find submarine but no supply
        nearestSubmarine = self.getControlCenter().getMap().findNearestElement(positionInFront, 2000, DangerLevel.MEDIUM, WEAPON_SUPPLY_ID)
        if nearestSubmarine.isValid():
            #move on and attack object
            self.getCommandProcessor().execute(moveAndAttack(self, nearestSubmarine.vecWorldTPosition))
            
        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        message = event.getMessage()
        if isinstance(message, CSWTextMessage):

            #if the text message contains WeaponSupply assume that this is a message from weapon supply
            if message.getText().find('WeaponSupply') >= 0:

                #weapon supply is available. so extract information
                #message format is: "<WeaponSupplyName> position: <x> <y> <z> radius: <d> resources: <r>"
                try:
                    splitted = message.getText().split()
                    position = Vector3(float(splitted[2]), float(splitted[3]), float(splitted[4]))
                    #insert the position of the supply into the map and store additional information 
                    #regarding to the object id
                    id = self.getControlCenter().getMap().insertElement(position, WEAPON_SUPPLY_ID, 0)
                    self.dictObjectInformation[id] = SupplyInformation(splitted[0], float(splitted[6]), int(splitted[8]))
                except:
                    pass
            
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    




#creates a special macro command that makes the submarine move and attack an object at the given position
def moveAndAttack(submarine, position):
    cmd = MacroCommand.create('MoveAndAttack')
    #switch off all engines
    cmd.attach(CSWCommands.disableAllEngines(submarine))
    #orientates/move toward the attacking position
    directionToTarget = position - submarine.getControlCenter().getGPS().getPosition()
    distanceToTarget = directionToTarget.getLength()

    #rotate toward target only if it is close to target. on large distances the moving works fine.
    if distanceToTarget < 400:
        cmd.attach(Commands.rotateCommand(submarine, position))
    
    #move to get a distance of 300m; if to close move backward to target
    targetPosition = position - directionToTarget*(300/distanceToTarget)
    moveForwardToTarget = distanceToTarget > 300
    cmd.attach(Commands.MoveCommand(submarine, targetPosition, moveForwardToTarget))
        
    #fire one ordinary torpedo
    cmd.attach(CSWCommands.fireSalvo(submarine.getFrontLeftWeaponBattery(), 1, 0))
    cmd.attach(CSWWaitCommand.create(1))
    #scan environment
    cmd.attach(CSWCommands.setAxialInclinationJetOar(submarine, 1))                
    return cmd


#makes the submarine move to the nearest known weapon supply and recharge.
#if no weapon supply exists nothing is done. this function is specialized to 
#working with GreenSubmarine only.
def rechargeWeaponBatteries(submarine):
    #find position of the next weapon supply 
    nearestSupply = submarine.getControlCenter().getMap().findNearestElement(submarine.getControlCenter().getGPS().getPosition(), 4000, WEAPON_SUPPLY_ID, 0)	  
    if not nearestSupply.isValid(): return      
    if not nearestSupply.nID in submarine.dictObjectInformation: return
            
    supplyInfo = submarine.dictObjectInformation[nearestSupply.nID]
    #create macro
    cmd = MacroCommand.create('RechargeWeaponBatteries')
    cmd.attach(CSWCommands.disableAllEngines(submarine))

    #if the distance to the supply is more than the recharge radius move to its near        
    directionToSupply = nearestSupply.vecWorldTPosition - submarine.getControlCenter().getGPS().getPosition()
    moveNearToSupply = directionToSupply.getLength() > supplyInfo.getRadius()*0.8
    if moveNearToSupply:
        directionToSupply.normalize()
        targetPosition = nearestSupply.vecWorldTPosition - directionToSupply*(supplyInfo.getRadius()*0.8)
        cmd.attach(Commands.MoveCommand(submarine, targetPosition))
        
    #try to recharge front left weapon battery with ordinary torpedos (green torpedos)
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getFrontLeftWeaponBattery(), supplyInfo.getName(), WeaponResource.GREEN_TORPEDO_ID))
    #try to recharge front right weapon battery with homing torpedos (red torpedos)
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getFrontRightWeaponBattery(), supplyInfo.getName(), WeaponResource.RED_TORPEDO_ID))
    #try to recharge back weapon battery with ordinary torpedos (green torpedos) and ordinary mines (green mines)
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getBackWeaponBattery(), supplyInfo.getName(), WeaponResource.GREEN_TORPEDO_ID | WeaponResource.GREEN_MINE_ID))
    #wait little time to be recharged
    cmd.attach(CSWWaitCommand.create(30))
    
    if moveNearToSupply:
        #if moved to near to supply now move back to where come from
        cmd.attach(Commands.MoveCommand(submarine, submarine.getControlCenter().getGPS().getPosition()));
        
    #scan environment
    cmd.attach(CSWCommands.setAxialInclinationJetOar(submarine, 1))                
    #execute built command
    submarine.getCommandProcessor().execute(cmd)



#base class of information regarding to object id's in a map
class ObjectInformation:
    def __init__(self, name):
        self.name = name
        
    def getName(self):
        return self.name

    def __str__(self):
        return self.name


#specialized class that holds information ragarding to supply objects in the map
class SupplyInformation(ObjectInformation):
    def __init__(self, name, radius, resources):
        ObjectInformation.__init__(self, name)
        self.radius = radius
        self.resources = resources
        
    def getRadius(self):
        return self.radius

    def getResources(self):
        return self.resources

    def __str__(self):
        return ObjectInformation.__str__(self) + ' ' + str(self.radius) + ' ' + str(self.resources)
