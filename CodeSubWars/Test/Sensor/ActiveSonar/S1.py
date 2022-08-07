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
        sonar = self.submarine.getControlCenter().getActiveSonar()
        sonar.setEnableAutomaticRotation(0)
        sonar.setScanDirectionMode(ScanDirectionMode.GLOBAL_POSITION)
        self.time = CSWTime.getTime()
        
    def step(self):
        sonar = self.submarine.getControlCenter().getActiveSonar()
        if CSWTime.getTime() > self.time + 5:
                        
            self.setProgress(1)
            self.finished()
