# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#command of rule base submarine


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
sys.path.append('./submarines/RuleBasedSubmarineModules')
import Constants


#Launch torpedo to destination. Setup the torpedo so that it will be fired at or near the target.
class FireCommand(CSWPyCommand):
    def __init__(self, param, targetPosition = None):
        if isinstance(param, self.__class__): 
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.targetPosition = param.targetPosition
        else: 
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.targetPosition = targetPosition

    def initialize(self):
        pass
        
    def step(self):
        distance = (self.targetPosition - self.submarine.getControlCenter().getGPS().getPosition()).getLength()
        if (distance > 50 and not self.submarine.getFrontLeftWeaponBattery().isEmpty()):
            velocityTorpedo = 35.6 #meter per second (measured)
            flightTimeTorpedo = distance/velocityTorpedo
    
            weapon = self.submarine.getFrontLeftWeaponBattery().getNext()
            weapon.fire(flightTimeTorpedo)
            self.submarine.getFrontLeftWeaponBattery().release(weapon)
        
        self.setProgress(1)
        self.finished()
        
    def cleanup(self):
        pass

    def getName(self): return self.__class__.__name__
