# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test actuator bows jet oar engine

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
        self.getCommandProcessor().execute(CheckPreConditions(self))

        #test
        self.getCommandProcessor().execute(DoTest1(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest2(self))
        self.getCommandProcessor().execute(CSWWaitCommand.create(10))
        self.getCommandProcessor().execute(DoTest3(self))



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
        self.rightwardDirection = self.submarine.makeGlobalDirection(Vector3(-1, 0, 0))
        self.submarine.getBowsJetOar().setDirection(Vector3(1, 1, 1))
        assert(self.submarine.getBowsJetOar().getDirection().getAngleBetween(self.rightwardDirection) < 0.01)
        
    def step(self):
        self.setProgress(1)
        self.finished()



#let the sub rotate rightward
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
        self.time = CSWTime.getTime()
        self.forwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        self.rightwardDirection = self.submarine.makeGlobalDirection(Vector3(-1, 0, 0))
        self.submarine.getBowsJetOar().setIntensity(10)
        assert(self.submarine.getBowsJetOar().getIntensity() == 1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 136.0 and CSWTime.getTime() < self.time + 137.0:
            assert(self.rightwardDirection.getAngleBetween(forwardDir) < 2)

        if CSWTime.getTime() > self.time + 542.5:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 1)
            
            self.submarine.getBowsJetOar().setIntensity(0)
            assert(self.submarine.getBowsJetOar().getIntensity() == 0)
            self.setProgress(1)
            self.finished()



#let the sub rotate leftward
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
        self.submarine.getBowsJetOar().setIntensity(-10)
        assert(self.submarine.getBowsJetOar().getIntensity() == -1)
        
    def step(self):
        forwardDir = self.submarine.makeGlobalDirection(Vector3(0, 0, 1))
        if CSWTime.getTime() > self.time + 136.0 and CSWTime.getTime() < self.time + 137.0:
            assert(self.leftwardDirection.getAngleBetween(forwardDir) < 2)

        if CSWTime.getTime() > self.time + 542.5:
            assert(self.forwardDirection.getAngleBetween(forwardDir) < 1)
            
            self.submarine.getBowsJetOar().setIntensity(0)
            assert(self.submarine.getBowsJetOar().getIntensity() == 0)
            self.setProgress(1)
            self.finished()


