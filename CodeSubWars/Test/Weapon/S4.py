# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test green and red torpedo (front left/right weapon battery)

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



class S4(CSWPySubmarine):
    def __init__(self): CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
    def update(self): pass
    def processEvent(self, event): return 1

    def initialize(self):
        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions(self, self.getFrontLeftWeaponBattery()))
        self.getCommandProcessor().execute(CheckPreConditions(self, self.getFrontRightWeaponBattery()))

        #test
        self.getCommandProcessor().execute(DoTest1(self, self.getFrontLeftWeaponBattery()))
        self.getCommandProcessor().execute(DoTest2(self, self.getFrontLeftWeaponBattery()))
        self.getCommandProcessor().execute(DoTest3(self, self.getFrontLeftWeaponBattery()))
        self.getCommandProcessor().execute(DoTest4(self, self.getFrontLeftWeaponBattery()))
        self.getCommandProcessor().execute(DoTest5(self, self.getFrontLeftWeaponBattery()))

        self.getCommandProcessor().execute(DoTest1(self, self.getFrontRightWeaponBattery()))
        self.getCommandProcessor().execute(DoTest2(self, self.getFrontRightWeaponBattery()))
        self.getCommandProcessor().execute(DoTest3(self, self.getFrontRightWeaponBattery()))
        self.getCommandProcessor().execute(DoTest4(self, self.getFrontRightWeaponBattery()))
        self.getCommandProcessor().execute(DoTest5(self, self.getFrontRightWeaponBattery()))



class CheckPreConditions(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def initialize(self): pass
    def cleanup(self): pass    
    def getName(self): return self.__class__.__name__
        
    def step(self):
        assert(self.battery.isFull())
        
        assert(self.submarine.getHealth() == 1)

        self.setProgress(1)
        self.finished()



class DoTest1(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def cleanup(self): pass
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.health = self.submarine.getHealth()

        weapon = self.battery.getNext()
        assert(weapon != None)
        assert(weapon.arm(1000))
        assert(weapon.fire(0.2))
        assert(self.battery.release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() < self.health - 0.02)
    
            self.setProgress(1)
            self.finished()



class DoTest2(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def cleanup(self): pass
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.health = self.submarine.getHealth()

        weapon = self.battery.getNext()
        assert(weapon != None)
        assert(weapon.fire(0.2))
        assert(weapon.arm(1000))
        assert(self.battery.release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() == self.health)
    
            self.setProgress(1)
            self.finished()



class DoTest3(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def cleanup(self): pass
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.health = self.submarine.getHealth()

        weapon = self.battery.getNext()
        assert(weapon != None)
        assert(weapon.arm(1000))
        assert(self.battery.release(weapon))
        assert(not weapon.fire(0.2))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() == self.health)
    
            self.setProgress(1)
            self.finished()



class DoTest4(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def cleanup(self): pass
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.health = self.submarine.getHealth()

        weapon = self.battery.getNext()
        assert(weapon != None)
        assert(weapon.fire(0.2))
        assert(self.battery.release(weapon))
        assert(not weapon.arm(1000))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() < self.health - 0.02)
    
            self.setProgress(1)
            self.finished()



class DoTest5(CSWPyCommand):
    def __init__(self, param, battery = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.battery = param.battery
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.battery = battery

    def cleanup(self): pass
    def getName(self): return self.__class__.__name__

    def initialize(self):
        self.health = self.submarine.getHealth()

        weapon = self.battery.getNext()
        assert(weapon != None)
        assert(weapon.fire(3.5))
        assert(self.battery.release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 5:
            assert(self.submarine.getHealth() == self.health)
    
            self.setProgress(1)
            self.finished()
