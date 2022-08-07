# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to deal with a map. it looks for the nearest object in the map and
#attack this object

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

sys.path.append('./pylib/common')
from Utilities import *



WEAPON_SUPPLY_ID = 1024

class TestSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.5, 0.5, 0.5, 1))

#        c1 = ChaineableCommand(None)
#        c2 = ChaineableCommand(None, c1)
#        c3 = ChaineableCommand(None, c2)
#        self.getCommandProcessor().execute(c1)
#        self.getCommandProcessor().execute(c2)
#        self.getCommandProcessor().execute(c3)

          
    def update(self):
        if self.getCommandProcessor().isBusy(): return

        if self.getFrontLeftWeaponBattery().isEmpty() and self.getFrontLeftWeaponBattery().getRechargingIDs() == 0:
            rechargeTorpedos(self)
            return
            
        direction = self.getControlCenter().getGPS().getPosition();
        direction.normalize()

        self.getCommandProcessor().cleanup()
        self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
        self.getCommandProcessor().execute(rotateFastTowardDirection(self, direction))
        self.getCommandProcessor().execute(CSWCommands.moveForward(self, 400, 0, 100))
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontLeftWeaponBattery(), 70, 1.1))
            
        
    def processEvent(self, event):        
        message = event.getMessage()
        if isinstance(message, CSWTextMessage):
            if message.getText().find('WeaponSupply') >= 0:
                #weapon supply is available extract position and maximal distance (format: "WeaponSupply position: x y z radius: d resources: r")
                try:
                    splitted = message.getText().split()
                    position = Vector3(float(splitted[2]), float(splitted[3]), float(splitted[4]))
                    self.getControlCenter().getMap().insertElement(position, WEAPON_SUPPLY_ID, 0)
                except:
                    pass
            
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    



#creates a special macro command that makes the submarine to rotate fast but less accurate by using its main engine too
def rotateFastTowardDirection(submarine, direction):
    position = submarine.getControlCenter().getGPS().getPosition() + direction*150
    return Commands.rotateCommand(submarine, position)



#makes the submarine move to the nearest weapon supply and recharge.
def rechargeTorpedos(submarine):
    nearestElement = submarine.getControlCenter().getMap().findNearestElement(submarine.getControlCenter().getGPS().getPosition(), 4000, WEAPON_SUPPLY_ID, 0)
    if not nearestElement.isValid(): return

    cmd = MacroCommand.create('RechargeTorpedos')
    directionToSupply = nearestElement.vecWorldTPosition - submarine.getControlCenter().getGPS().getPosition()
    if directionToSupply.getLength() > 300:
        directionToSupply.normalize()
        targetPosition = nearestElement.vecWorldTPosition - directionToSupply*240.0
        cmd.attach(Commands.MoveCommand(submarine, targetPosition))
        
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getFrontLeftWeaponBattery(), "envWeaponSupply", WeaponResource.GREEN_TORPEDO_ID))
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getFrontRightWeaponBattery(), "envWeaponSupply", WeaponResource.GREEN_MINE_ID))
    cmd.attach(CSWRechargeWeaponBatteryCommand.create(submarine.getBackWeaponBattery(), "envWeaponSupply", WeaponResource.GREEN_TORPEDO_ID))
    cmd.attach(CSWWaitCommand.create(20))
    submarine.getCommandProcessor().execute(cmd)






class ChaineableCommand(CSWPyCommand):
    def __init__(self, param = None, previousCommand = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            self.previousCommand = param.previousCommand
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.previousCommand = previousCommand

        if self.previousCommand != None:
            print(self.getName() + ' created with connection to ' + self.previousCommand.getName())
        self.previousResult = None
        self.a = 0

    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        if self.previousCommand != None:
            self.previousResult = self.previousCommand.getResult()
            print(self.getName() + ' started with data ' + str(self.previousResult) + ' from ' + self.previousCommand.getName())
        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        self.a = self.a + 1
        self.setProgress(self.a/400)
        if (self.a >= 400):
            self.setProgress(1)
            self.finished()

    #this method must be overriden and is called directly after finishing of execution or on breaking.
    def cleanup(self):
        print(self.getName() + ' finished')
    
    def getResult(self):
        return 42
    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__ + str(id(self))

    #this method should be overriden and should return some interesting stuff about the current command state.
    def getDetails(self):
        if self.previousCommand != None:
            return 'Connected with previous cmd: ' + self.previousCommand.getName()
        return 'not connected'
