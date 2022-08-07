# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#this is collection of specialized sample commands used in indigo submarine

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
sys.path.append('./pylib/commands')
import Commands



#this is a minimal command that does nothing. the only thing that must be done is call finished() in step().
#this indicates the command processor to finish this command.
class NOPCommand(CSWPyCommand):
    def __init__(self, param = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)

    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        pass
        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        self.setProgress(1)
        self.finished()

    #this method must be overriden and is called directly after finishing of execution or on canceling.
    def cleanup(self):
        pass
    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__




#this command lays mines if the velocity of the executing submarine is more than 9m/s and the distance to
#the last mine is more than the given distance in meter. the command finishes if no mine is available.
#@param param the submarine that should be moved weird (should be self).
#@param mineDistance the minimal distance between 2 mines
class LayMinesCommand(CSWPyCommand):
    def __init__(self, param, mineDistance = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            #copy all member variables
            self.submarine = param.submarine
            self.mineDistance = param.mineDistance
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.mineDistance = mineDistance


    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        self.lastMinePosition = Vector3(0, 0, 0)

        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        #lay a mine only if we move forward with more than 9m/s and if the distance to the last mine is more than the given distance
        if self.submarine.getControlCenter().getMovingPropertiesSensor().getForwardVelocity() > 9 and \
           (self.submarine.getControlCenter().getGPS().getPosition() - self.lastMinePosition).getLength() > self.mineDistance:
            
            #get the mine that will be released next
            weapon = self.submarine.getBackWeaponBattery().getNext()
            #arm weapon from now in 5 seconds
            weapon.arm(5)
            #if last weapon, fire 120 seconds from now
            if self.submarine.getBackWeaponBattery().getNumInserted() == 1: weapon.fire(120)
            #release mine
            self.submarine.getBackWeaponBattery().release(weapon)
            #store the position
            self.lastMinePosition = self.submarine.getControlCenter().getGPS().getPosition()
            #update progress state
            self.setProgress(float(self.submarine.getBackWeaponBattery().getSize() - self.submarine.getBackWeaponBattery().getNumInserted()) / 
                             self.submarine.getBackWeaponBattery().getSize())

        #if no mine is available finish itself
        if self.submarine.getBackWeaponBattery().isEmpty(): 
            self.setProgress(1)
            self.finished()

    
    #this method must be overriden and is called directly after finishing of execution or on canceling.
    def cleanup(self):
        pass

    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__

    
    #this method should be overriden and should return some interesting stuff about the current command state.
    def getDetails(self):
        return 'Lay mine ' + str(self.submarine.getBackWeaponBattery().getSize() - self.submarine.getBackWeaponBattery().getNumInserted() + 1) + \
               ' of ' + str(self.submarine.getBackWeaponBattery().getSize())




#this command makes the submarine move to randomly generated positions for ever. this command generates and executes new 
#move commands on each execution step. to avoid useless filling of the execution queue this command is not working on the 
#same execution context as the new generated command. this is done by using push and pop.
#@param param the submarine that should be moved weird (should be self).
class WeirdMovingCommand(CSWPyCommand):
    def __init__(self, param):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.submarine = param


    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        pass

        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        #create new random position
        position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)

        #copy itself onto the stack
        self.submarine.getCommandProcessor().push()
        #remove itself from current executing context
        self.submarine.getCommandProcessor().cleanup()
        #execute new command
        self.submarine.getCommandProcessor().execute(Commands.MoveCommand(self.submarine, position))
        #after execution of the new command restore itself from stack and continue executing itself
        self.submarine.getCommandProcessor().execute(PopCommand.create())

        
    #this method must be overriden and is called directly after finishing of execution or on canceling.
    def cleanup(self):
        pass

    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__
