# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test green mine (back weapon battery)

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



class S3(CSWPySubmarine):
    def __init__(self): CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
    def update(self): pass
    def processEvent(self, event): return 1

    def initialize(self):
        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))
        self.getCommandProcessor().execute(DoTest2(self))
        self.getCommandProcessor().execute(DoTest3(self))
        self.getCommandProcessor().execute(DoTest4(self))
        self.getCommandProcessor().execute(DoTest5(self))
        self.getCommandProcessor().execute(DoTest6(self))
        self.getCommandProcessor().execute(DoTest7(self))




class CheckPreConditions(CSWPyCommand):
    def __init__(self, param):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param

    def initialize(self): pass
    def cleanup(self): pass    
    def getName(self): return self.__class__.__name__
        
    def step(self):
        assert(self.submarine.getBackWeaponBattery().isFull())
        
        assert(self.submarine.getHealth() == 1)

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
        self.health = self.submarine.getHealth()
        self.submarine.getMainEngine().setIntensity(-1)

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.arm(0))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() < self.health - 0.02)
            self.submarine.getMainEngine().setIntensity(0)
    
            self.setProgress(1)
            self.finished()



class DoTest2(CSWPyCommand):
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
        self.health = self.submarine.getHealth()
        self.submarine.getMainEngine().setIntensity(-1)

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.arm(1000))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() == self.health)
            self.submarine.getMainEngine().setIntensity(0)
    
            self.setProgress(1)
            self.finished()
        


class DoTest3(CSWPyCommand):
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
        self.health = self.submarine.getHealth()

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.arm(1000))
        assert(weapon.fire(1))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() < self.health - 0.02)
    
            self.setProgress(1)
            self.finished()



class DoTest4(CSWPyCommand):
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
        self.health = self.submarine.getHealth()
        self.submarine.getMainEngine().setIntensity(-1)

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.fire(1))
        assert(weapon.arm(1000))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() == self.health)
            self.submarine.getMainEngine().setIntensity(0)
    
            self.setProgress(1)
            self.finished()



class DoTest5(CSWPyCommand):
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
        self.health = self.submarine.getHealth()
        self.submarine.getMainEngine().setIntensity(1)

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.fire(8))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 9:
            assert(self.submarine.getHealth() == self.health)
            self.submarine.getMainEngine().setIntensity(0)
    
            self.setProgress(1)
            self.finished()



class DoTest6(CSWPyCommand):
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
        self.health = self.submarine.getHealth()

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.fire(1))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        assert(not weapon.arm(1000))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() < self.health - 0.02)
    
            self.setProgress(1)
            self.finished()



class DoTest7(CSWPyCommand):
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
        self.health = self.submarine.getHealth()

        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.arm(1000))
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        assert(not weapon.fire(1))
        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            assert(self.submarine.getHealth() == self.health)
    
            self.setProgress(1)
            self.finished()

