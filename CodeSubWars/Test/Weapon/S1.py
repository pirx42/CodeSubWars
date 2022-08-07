# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test recharge of weapon battery

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
from Utilities import *



class S1(CSWPySubmarine):
    def __init__(self): CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
    def update(self): pass
    def processEvent(self, event): return 1

    def initialize(self):
        #prepare for testing
        self.getControlCenter().getActiveSonar().setEnableAutomaticRotation(0)
        self.getControlCenter().getActiveSonar().setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        self.getControlCenter().getActiveSonar().pointToGlobalPosition(Vector3(0, 0, 0))
        
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontLeftWeaponBattery(), 44, 1.1))
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontRightWeaponBattery(), 22, 1.1))
        self.getCommandProcessor().execute(CSWCommands.setMainEngineIntensity(self, 0.1))
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getBackWeaponBattery(), 22, 1.1))

        position = self.getControlCenter().getGPS().getPosition()
        if position.getLength() > 200:
            targetPosition = position.normalize()*200.0
            self.getCommandProcessor().execute(CSWCommands.moveAbsolute(self, targetPosition, 0, 100))

        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))




class CheckPreConditions(CSWPyCommand):
    def __init__(self, param):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param

    def cleanup(self): pass    
    def getName(self): return self.__class__.__name__
        
    def initialize(self):
        #check if supply is at position (0, 0, 0)
        assert(self.submarine.getControlCenter().getActiveSonar().hasTargetDetected())
        assert(self.submarine.getControlCenter().getActiveSonar().getTargetPosition().getLength() < 200)
        
        assert(self.submarine.getFrontLeftWeaponBattery().isEmpty())
        assert(self.submarine.getFrontRightWeaponBattery().isEmpty())
        assert(self.submarine.getBackWeaponBattery().isEmpty())

        assert(self.submarine.getFrontLeftWeaponBattery().getFillLevel() == 0)
        assert(self.submarine.getFrontRightWeaponBattery().getFillLevel() == 0)
        assert(self.submarine.getBackWeaponBattery().getFillLevel() == 0)

        assert(self.submarine.getFrontLeftWeaponBattery().getRechargingIDs() == 0)
        assert(self.submarine.getFrontRightWeaponBattery().getRechargingIDs() == 0)
        assert(self.submarine.getBackWeaponBattery().getRechargingIDs() == 0)

        assert(self.submarine.getFrontLeftWeaponBattery().getNumInserted() == 0)
        assert(self.submarine.getFrontRightWeaponBattery().getNumInserted() == 0)
        assert(self.submarine.getBackWeaponBattery().getNumInserted() == 0)

    def step(self):
        self.setProgress(1)
        self.finished()



class DoTest1(CSWPyCommand):
    def __init__(self, param):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param

    def cleanup(self): pass    
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.time = CSWTime.getTime()
        assert(self.submarine.getFrontLeftWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.RED_TORPEDO_ID) == 0)
        assert(self.submarine.getFrontLeftWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.GREEN_MINE_ID) == 0)
        assert(self.submarine.getFrontLeftWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.RED_TORPEDO_ID | WeaponResource.GREEN_MINE_ID) == 0)
        assert(self.submarine.getFrontLeftWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.GREEN_TORPEDO_ID) == WeaponResource.GREEN_TORPEDO_ID)

        assert(self.submarine.getFrontRightWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.GREEN_MINE_ID) == 0)
        assert(self.submarine.getFrontRightWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.RED_TORPEDO_ID | WeaponResource.GREEN_TORPEDO_ID) == \
               WeaponResource.RED_TORPEDO_ID | WeaponResource.GREEN_TORPEDO_ID)

        assert(self.submarine.getBackWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.RED_TORPEDO_ID) == 0)
        assert(self.submarine.getBackWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.GREEN_TORPEDO_ID) == 0)
        assert(self.submarine.getBackWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.RED_TORPEDO_ID | WeaponResource.GREEN_TORPEDO_ID) == 0)
        assert(self.submarine.getBackWeaponBattery().tryRecharging("envWeaponSupply", WeaponResource.GREEN_MINE_ID | WeaponResource.GREEN_TORPEDO_ID) == WeaponResource.GREEN_MINE_ID)

        assert(self.submarine.getFrontLeftWeaponBattery().getRechargingIDs() == WeaponResource.GREEN_TORPEDO_ID)
        assert(self.submarine.getFrontRightWeaponBattery().getRechargingIDs() == WeaponResource.RED_TORPEDO_ID | WeaponResource.GREEN_TORPEDO_ID)
        assert(self.submarine.getBackWeaponBattery().getRechargingIDs() == WeaponResource.GREEN_MINE_ID)

    def step(self):
        if CSWTime.getTime() > self.time + 150:
            assert(self.submarine.getFrontLeftWeaponBattery().isFull())
            assert(self.submarine.getFrontRightWeaponBattery().isFull())
            assert(self.submarine.getBackWeaponBattery().isFull())
    
            assert(self.submarine.getFrontLeftWeaponBattery().getFillLevel() == 1)
            assert(self.submarine.getFrontRightWeaponBattery().getFillLevel() == 1)
            assert(self.submarine.getBackWeaponBattery().getFillLevel() == 1)
    
            assert(self.submarine.getFrontLeftWeaponBattery().getRechargingIDs() == 0)
            assert(self.submarine.getFrontRightWeaponBattery().getRechargingIDs() == 0)
            assert(self.submarine.getBackWeaponBattery().getRechargingIDs() == 0)
    
            assert(self.submarine.getFrontLeftWeaponBattery().getNumInserted() == self.submarine.getFrontLeftWeaponBattery().getSize())
            assert(self.submarine.getFrontRightWeaponBattery().getNumInserted() == self.submarine.getFrontRightWeaponBattery().getSize())
            assert(self.submarine.getBackWeaponBattery().getNumInserted() == self.submarine.getBackWeaponBattery().getSize())

            self.setProgress(1)
            self.finished()
        

