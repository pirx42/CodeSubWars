# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#this is collection of useful predefined special commands for black submarine

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



#creates a macro command that makes the submarine attack an object at the given global position
def attackAtPosition(submarine, globalPosition):
    cmd = MacroCommand.create('AttackAtPosition')
    #switch off all engines
    cmd.attach(CSWCommands.disableAllEngines(submarine))
    #orientates toward the attacking position
    cmd.attach(CSWCommands.setOrientationAbsoluteTowardPosition(submarine, globalPosition, 0.1, 100))
    #fire two ordinary torpedos
    cmd.attach(FireCommand(submarine, globalPosition))
    cmd.attach(CSWWaitCommand.create(1))
    cmd.attach(FireCommand(submarine, globalPosition))
    cmd.attach(CSWWaitCommand.create(1.5))
    return cmd





#Launch torpedo to destination. Setup the torpedo so that it will be fired at or near the target.
class FireCommand(CSWPyCommand):
    def __init__(self, param, globalPosition = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            #copy all member variables
            self.submarine = param.submarine
            self.globalPosition = param.globalPosition
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.globalPosition = globalPosition


    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        pass
        
        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        #if the target is to close or no weapons are available finish.
        distanceToTarget = (self.globalPosition - self.submarine.getControlCenter().getGPS().getPosition()).getLength()
        if (distanceToTarget < 50 or self.submarine.getFrontLeftWeaponBattery().isEmpty()):
            self.setProgress(1)
            self.finished()
            return
            
        #determine the time to flight
        velocityTorpedo = 35.6 #meter per second (measured)
        flightTimeTorpedo = distanceToTarget/velocityTorpedo

        #setup the weapon to fire after time to flight (near to target)
        weapon = self.submarine.getFrontLeftWeaponBattery().getNext()
        weapon.arm(5)
        weapon.fire(flightTimeTorpedo)
        self.submarine.getFrontLeftWeaponBattery().release(weapon)
        
        #thats it
        self.setProgress(1)
        self.finished()
        
    
    #this method must be overriden and is called directly after finishing of execution or on canceling.
    def cleanup(self):
        pass

    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__
