# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test actuator main engine

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
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest2(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest3(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest4(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest5(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
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
        assert(self.submarine.getMainEngine().getIntensity() == 0)
        self.setProgress(1)
        self.finished()


#let the sub move a forward
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
        self.position = self.submarine.getControlCenter().getGPS().getPosition()
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.submarine.getMainEngine().setIntensity(10)
        assert(self.submarine.getMainEngine().getIntensity() == 1)
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            p = self.position + self.forwardDirection*14
            assert((p - self.submarine.getControlCenter().getGPS().getPosition()).getLength() < 1.5)            
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            
            self.setProgress(1)
            self.finished()


#let the sub move a backward
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
        self.position = self.submarine.getControlCenter().getGPS().getPosition()
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.submarine.getMainEngine().setIntensity(-10)
        assert(self.submarine.getMainEngine().getIntensity() == -1)
        
    def step(self):
        if CSWTime.getTime() > self.time + 2:
            p = self.position - self.forwardDirection*14
            assert((p - self.submarine.getControlCenter().getGPS().getPosition()).getLength() < 1.5)
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)

            self.setProgress(1)
            self.finished()


#let the sub move a circle leftward
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
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.leftwardDirection = self.submarine.makeGlobalDirection(Vector3(1, 0, 0))
        self.submarine.getMainEngine().setDirection(-90, 0)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setDirection(90, 0)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setIntensity(1)
        assert(self.submarine.getMainEngine().getIntensity() == 1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 10.5 and CSWTime.getTime() < self.time + 11.5:
            assert(self.leftwardDirection.getAngleBetween(forwardDir) < 10)

        if CSWTime.getTime() > self.time + 41.6:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 3)
            
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            self.setProgress(1)
            self.finished()


#let the sub move a circle rightward
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
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.rightwardDirection = self.submarine.makeGlobalDirection(Vector3(-1, 0, 0))
        self.submarine.getMainEngine().setDirection(90, 0)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setDirection(-90, 0)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setIntensity(1)
        assert(self.submarine.getMainEngine().getIntensity() == 1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 10.5 and CSWTime.getTime() < self.time + 11.5:
            assert(self.rightwardDirection.getAngleBetween(forwardDir) < 10)

        if CSWTime.getTime() > self.time + 41.6:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 3)
            
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            self.setProgress(1)
            self.finished()


#let the sub move a circle downward
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
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.downwardDirection = self.submarine.makeGlobalDirection(Vector3(0, -1, 0))
        self.submarine.getMainEngine().setDirection(0, -90)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setDirection(0, 90)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setIntensity(1)
        assert(self.submarine.getMainEngine().getIntensity() == 1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 11.0 and CSWTime.getTime() < self.time + 12.0:
            assert(self.downwardDirection.getAngleBetween(forwardDir) < 10)

        if CSWTime.getTime() > self.time + 42.1:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 3)
            
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            self.setProgress(1)
            self.finished()


#let the sub move a circle upward
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
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.upwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 1, 0))
        self.submarine.getMainEngine().setDirection(0, 90)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setDirection(0, -90)
        assert(abs(self.submarine.getMainEngine().getDirection().getAngleBetween(self.forwardDirection) - 45.0) < 0.01)
        self.submarine.getMainEngine().setIntensity(1)
        assert(self.submarine.getMainEngine().getIntensity() == 1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 11.0 and CSWTime.getTime() < self.time + 12.0:
            assert(self.upwardDirection.getAngleBetween(forwardDir) < 10)

        if CSWTime.getTime() > self.time + 42.1:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 3)
            
            self.submarine.getMainEngine().setIntensity(0)
            assert(self.submarine.getMainEngine().getIntensity() == 0)
            self.setProgress(1)
            self.finished()
