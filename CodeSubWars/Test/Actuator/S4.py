# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#test submarine
#test actuator axial inclination jet oar engine

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
        self.submarine.getAxialInclinationJetOar().setDirection(Vector3(1, 1, 1))
        assert(self.submarine.getAxialInclinationJetOar().getDirection().getAngleBetween(self.rightwardDirection) < 0.01)
        
    def step(self):
        self.setProgress(1)
        self.finished()



#let the sub rotate rightward about its axis
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
        self.upwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 1, 0))
        self.rightwardDirection = self.submarine.makeGlobalDirection(Vector3(-1, 0, 0))
        self.submarine.getAxialInclinationJetOar().setIntensity(10)
        assert(self.submarine.getAxialInclinationJetOar().getIntensity() == 1)
        
    def step(self):
        upwardDir = self.submarine.makeGlobalDirection(Vector3(0, 1, 0))
        print str(CSWTime.getTime() - self.time) + ' ' + str(self.upwardDirection.getAngleBetween(upwardDir))
        if CSWTime.getTime() > self.time + 73.0 and CSWTime.getTime() < self.time + 74.0:
            assert(self.rightwardDirection.getAngleBetween(upwardDir) < 2)

        if CSWTime.getTime() > self.time + 288.5:
            assert(self.upwardDirection.getAngleBetween(upwardDir) < 1)
            
            self.submarine.getAxialInclinationJetOar().setIntensity(0)
            assert(self.submarine.getAxialInclinationJetOar().getIntensity() == 0)
            self.setProgress(1)
            self.finished()



#let the sub rotate leftward about its axis
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
        self.upwardDirection = self.submarine.makeGlobalDirection(Vector3(0, 1, 0))
        self.leftwardDirection = self.submarine.makeGlobalDirection(Vector3(1, 0, 0))
        self.submarine.getAxialInclinationJetOar().setIntensity(-10)
        assert(self.submarine.getAxialInclinationJetOar().getIntensity() == -1)
        
    def step(self):
        upwardDir = self.submarine.makeGlobalDirection(Vector3(0, 1, 0))
        if CSWTime.getTime() > self.time + 73.0 and CSWTime.getTime() < self.time + 74.0:
            assert(self.leftwardDirection.getAngleBetween(upwardDir) < 2)

        if CSWTime.getTime() > self.time + 288.5:
            assert(self.upwardDirection.getAngleBetween(upwardDir) < 1)
            
            self.submarine.getAxialInclinationJetOar().setIntensity(0)
            assert(self.submarine.getAxialInclinationJetOar().getIntensity() == 0)
            self.setProgress(1)
            self.finished()


