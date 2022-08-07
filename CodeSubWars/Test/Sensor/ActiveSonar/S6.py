# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test sensor passive sonar, global position scanning mode


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

import random
import sys
sys.path.append('./pylib/common')
from Utilities import *



class S6(CSWPySubmarine):
    def __init__(self): CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
    def update(self): pass
    def processEvent(self, event): return 1

    def initialize(self):
        #check pre conditions
        self.getCommandProcessor().execute(CheckPreConditions(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest2(self))
        self.getCommandProcessor().execute(DoTest3(self))
        self.getCommandProcessor().execute(DoTest4(self))
        self.getCommandProcessor().execute(DoTest5(self))



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
        assert(not sonar.isAutomaticRotationEnabled())
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        assert(sonar.getScanDirectionMode() == ScanDirectionMode.GLOBAL_POSITION)
        sonar.setScanRangeMode(ScanRangeMode.NEAR_RANGE)
        assert(sonar.getScanRangeMode() == ScanRangeMode.NEAR_RANGE)
        sonar.setScanVelocityMode(ScanVelocityMode.FAST)
        assert(sonar.getScanVelocityMode() == ScanVelocityMode.FAST)
        self.position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
        sonar.pointToGlobalPosition(self.position)
        direction = (self.position - self.submarine.getControlCenter().getGPS().getPosition()).getNormalized()
        assert(direction.getAngleBetween(sonar.getDirection()) < 5)
        self.submarine.getMainEngine().setDirection(Vector3(1, 0, 0))
        self.submarine.getMainEngine().setIntensity(1)
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 8:
            direction = (self.position - self.submarine.getControlCenter().getGPS().getPosition()).getNormalized()
            assert(direction.getAngleBetween(sonar.getDirection()) < 5)
            
            self.submarine.getMainEngine().setDirection(Vector3(0, 0, 1))
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
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        sonar.setEnableAutomaticRotation(1)
        assert(sonar.isAutomaticRotationEnabled())
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        assert(sonar.getScanDirectionMode() == ScanDirectionMode.GLOBAL_POSITION)
        sonar.setScanRangeMode(ScanRangeMode.NEAR_RANGE)
        assert(sonar.getScanRangeMode() == ScanRangeMode.NEAR_RANGE)
        sonar.setScanVelocityMode(ScanVelocityMode.FAST)
        assert(sonar.getScanVelocityMode() == ScanVelocityMode.FAST)
        self.position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
        sonar.pointToGlobalPosition(self.position)
        self.direction = sonar.getDirection()
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 2/1.5:
            assert(self.direction.getAngleBetween(sonar.getDirection()) < 5)
            
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
        sonar.setEnableAutomaticRotation(1)
        assert(sonar.isAutomaticRotationEnabled())
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        assert(sonar.getScanDirectionMode() == ScanDirectionMode.GLOBAL_POSITION)
        sonar.setScanRangeMode(ScanRangeMode.FAR_RANGE)
        assert(sonar.getScanRangeMode() == ScanRangeMode.FAR_RANGE)
        sonar.setScanVelocityMode(ScanVelocityMode.FAST)
        assert(sonar.getScanVelocityMode() == ScanVelocityMode.FAST)
        self.position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
        sonar.pointToGlobalPosition(self.position)
        self.direction = sonar.getDirection()
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 2/1.5:
            assert(self.direction.getAngleBetween(sonar.getDirection()) < 5)
            
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
        sonar.setEnableAutomaticRotation(1)
        assert(sonar.isAutomaticRotationEnabled())
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        assert(sonar.getScanDirectionMode() == ScanDirectionMode.GLOBAL_POSITION)
        sonar.setScanRangeMode(ScanRangeMode.NEAR_RANGE)
        assert(sonar.getScanRangeMode() == ScanRangeMode.NEAR_RANGE)
        sonar.setScanVelocityMode(ScanVelocityMode.SLOW)
        assert(sonar.getScanVelocityMode() == ScanVelocityMode.SLOW)
        self.position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
        sonar.pointToGlobalPosition(self.position)
        self.direction = sonar.getDirection()
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 9/1.5:
            assert(self.direction.getAngleBetween(sonar.getDirection()) < 5)
            
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
        sonar.setEnableAutomaticRotation(1)
        assert(sonar.isAutomaticRotationEnabled())
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        assert(sonar.getScanDirectionMode() == ScanDirectionMode.GLOBAL_POSITION)
        sonar.setScanRangeMode(ScanRangeMode.FAR_RANGE)
        assert(sonar.getScanRangeMode() == ScanRangeMode.FAR_RANGE)
        sonar.setScanVelocityMode(ScanVelocityMode.SLOW)
        assert(sonar.getScanVelocityMode() == ScanVelocityMode.SLOW)
        self.position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
        sonar.pointToGlobalPosition(self.position)
        self.direction = sonar.getDirection()
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getPassiveSonar()
        if CSWTime.getTime() > self.time + 9/1.5:
            assert(self.direction.getAngleBetween(sonar.getDirection()) < 5)
            
            self.setProgress(1)
            self.finished()
