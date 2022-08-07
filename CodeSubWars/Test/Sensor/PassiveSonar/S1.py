# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test sensor passive sonar
#requires: test submarine must be alone in the world

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
        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(5))
        self.getCommandProcessor().execute(DoTest2(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(5))
        self.getCommandProcessor().execute(DoTest3(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(5))
        self.getCommandProcessor().execute(DoTest4(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(15))
        self.getCommandProcessor().execute(DoTest5(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(5))
        self.getCommandProcessor().execute(DoTest6(self))



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
        assert(self.submarine.getFrontLeftWeaponBattery().isFull())
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setAngleOfBeam(15)
        assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 5:
            assert(sonar.getLevel() > 25)
                        
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setAngleOfBeam(15)
        
        assert(not sonar.isAdjusting())
        assert(sonar.hasAdjusted())
        sonar.adjustDirectionToMaximum(3)
        assert(sonar.isAdjusting())
        assert(not sonar.hasAdjusted())
        
        assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 3.5:
            assert(not sonar.isAdjusting())
            assert(sonar.hasAdjusted())
            assert(sonar.getLevel() > 25)
            assert(sonar.getDirection().getAngleBetween(self.submarine.makeGlobalDirection(Vector3(0, 0, 1))) < 4)
                        
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setScanDirection(Vector3(0, 1, 0))
        sonar.setAngleOfBeam(-10)
        assert(sonar.getAngleOfBeam() == 0.0)
        sonar.setAngleOfBeam(200)
        assert(sonar.getAngleOfBeam() == 180.0)
        sonar.setAngleOfBeam(15)
        assert(sonar.getAngleOfBeam() == 15.0)

        assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 3:
            assert(sonar.getLevel() <= 0.01)

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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setScanDirection(Vector3(0, 1, 5).getNormalized())
        sonar.setAngleOfBeam(15)
        
        assert(not sonar.isAdjusting())
        assert(sonar.hasAdjusted())
        sonar.adjustDirectionToMaximum(0.1)
        assert(sonar.isAdjusting())
        assert(not sonar.hasAdjusted())
        
        assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 0.5:
            assert(not sonar.isAdjusting())
            assert(sonar.hasAdjusted())
            assert(sonar.getLevel() > 5)
                        
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setScanDirection(Vector3(0, 0, 1))
        sonar.setAngleOfBeam(15)
        
        weapon = self.submarine.getFrontLeftWeaponBattery().getNext()
        assert(weapon != None)
        assert(weapon.fire(50))
        assert(self.submarine.getFrontLeftWeaponBattery().release(weapon))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 51.0:
            assert(sonar.getLevel() < 0.01)
                        
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.LOCAL_DIRECTION)
        sonar.setScanDirection(Vector3(0, 0, 1))
        sonar.setAngleOfBeam(15)
        
        assert(self.submarine.getFrontLeftWeaponBattery().releaseNext(5))
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 61.0:
            assert(sonar.getLevel() < 0.01)
                        
            self.setProgress(1)
            self.finished()
