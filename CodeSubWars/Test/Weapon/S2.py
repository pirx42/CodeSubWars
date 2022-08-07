# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test release of weapons

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



class S2(CSWPySubmarine):
    def __init__(self): CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
    def update(self): pass
    def processEvent(self, event): return 1

    def initialize(self):
        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions1(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))

        #check post conditions
        self.getCommandProcessor().execute(CheckPostConditions1(self))


        #prepare for next testing
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontLeftWeaponBattery(), 44, 1.1))
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontRightWeaponBattery(), 22, 1.1))
        self.getCommandProcessor().execute(CSWCommands.setMainEngineIntensity(self, 0.1))
        self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getBackWeaponBattery(), 22, 1.1))
        self.getCommandProcessor().execute(CSWCommands.setMainEngineIntensity(self, 0))


        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions2(self))

        #test
        self.getCommandProcessor().execute(DoTest2(self))



class CheckPreConditions1(CSWPyCommand):
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
        assert(self.submarine.getFrontLeftWeaponBattery().isFull())
        assert(self.submarine.getFrontRightWeaponBattery().isFull())
        assert(self.submarine.getBackWeaponBattery().isFull())

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
        self.submarine.getMainEngine().setIntensity(0.1)

        self.numFLInsertedWeapons = self.submarine.getFrontLeftWeaponBattery().getNumInserted()
        weapon = self.submarine.getFrontLeftWeaponBattery().getNext()
        assert(weapon != None)
        assert(not self.submarine.getFrontLeftWeaponBattery().release(None))
        assert(self.submarine.getFrontLeftWeaponBattery().getNumInserted() == self.numFLInsertedWeapons)
        assert(self.submarine.getFrontLeftWeaponBattery().release(weapon))
        assert(self.submarine.getFrontLeftWeaponBattery().getNumInserted() == self.numFLInsertedWeapons - 1)
        assert(not self.submarine.getFrontLeftWeaponBattery().releaseNext(5))

        self.numFRInsertedWeapons = self.submarine.getFrontRightWeaponBattery().getNumInserted()
        weapon = self.submarine.getFrontRightWeaponBattery().getNext()
        assert(weapon != None)
        assert(not self.submarine.getFrontRightWeaponBattery().release(None))
        assert(self.submarine.getFrontRightWeaponBattery().getNumInserted() == self.numFRInsertedWeapons)
        assert(self.submarine.getFrontRightWeaponBattery().release(weapon))
        assert(self.submarine.getFrontRightWeaponBattery().getNumInserted() == self.numFRInsertedWeapons - 1)
        assert(not self.submarine.getFrontRightWeaponBattery().releaseNext(5))

        self.numBInsertedWeapons = self.submarine.getBackWeaponBattery().getNumInserted()
        weapon = self.submarine.getBackWeaponBattery().getNext()
        assert(weapon != None)
        assert(not self.submarine.getBackWeaponBattery().release(None))
        assert(self.submarine.getBackWeaponBattery().getNumInserted() == self.numBInsertedWeapons)
        assert(self.submarine.getBackWeaponBattery().release(weapon))
        assert(self.submarine.getBackWeaponBattery().getNumInserted() == self.numBInsertedWeapons - 1)
        assert(not self.submarine.getBackWeaponBattery().releaseNext(5))

        self.time = CSWTime.getTime()
        
    def step(self):
        if CSWTime.getTime() > self.time + 1.1:
            assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
            assert(self.submarine.getFrontLeftWeaponBattery().getNumInserted() == self.numFLInsertedWeapons - 2)

            assert(self.submarine.getFrontRightWeaponBattery().releaseNext(5))
            assert(self.submarine.getFrontRightWeaponBattery().getNumInserted() == self.numFRInsertedWeapons - 2)

            assert(self.submarine.getBackWeaponBattery().releaseNext(5))
            assert(self.submarine.getBackWeaponBattery().getNumInserted() == self.numBInsertedWeapons - 2)

            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            self.setProgress(1)
            self.finished()
                


class CheckPostConditions1(CSWPyCommand):
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
        assert(not self.submarine.getFrontLeftWeaponBattery().isFull())
        assert(not self.submarine.getFrontRightWeaponBattery().isFull())
        assert(not self.submarine.getBackWeaponBattery().isFull())

    def step(self):
        self.setProgress(1)
        self.finished()



class CheckPreConditions2(CSWPyCommand):
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
        assert(self.submarine.getFrontLeftWeaponBattery().isEmpty())
        assert(self.submarine.getFrontRightWeaponBattery().isEmpty())
        assert(self.submarine.getBackWeaponBattery().isEmpty())

    def step(self):
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
        assert(self.submarine.getFrontLeftWeaponBattery().getNext() == None)
        assert(not self.submarine.getFrontLeftWeaponBattery().release(None))
        assert(not self.submarine.getFrontLeftWeaponBattery().releaseNext(5))

        assert(self.submarine.getFrontRightWeaponBattery().getNext() == None)
        assert(not self.submarine.getFrontRightWeaponBattery().release(None))
        assert(not self.submarine.getFrontRightWeaponBattery().releaseNext(5))

        assert(self.submarine.getBackWeaponBattery().getNext() == None)
        assert(not self.submarine.getBackWeaponBattery().release(None))
        assert(not self.submarine.getBackWeaponBattery().releaseNext(5))

    def step(self):
        self.setProgress(1)
        self.finished()
        
