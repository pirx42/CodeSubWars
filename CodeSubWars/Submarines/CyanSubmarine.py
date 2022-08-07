# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows more advanced command functionality. it implements a simple static search "strategy". it moves along 
#a defined path. if a submarine has been detected, it stores the current commands using "push" and attacks the detected object. 
#after attacking it restores the previously stored commands using "pop".

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
sys.path.append('./pylib/commands')
import Commands


SEARCH_CUBE_EDGE_LENGTH = 1000


class CyanSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.0, 0.65, 0.92, 1))
        self.cmdAttack = None

        #construct and execute "search" route 
        self.getCommandProcessor().execute(searchOnPath(self, Vector3(0, 0, 0)))


    #this method must be overriden and is called every 10ms
    def update(self):
        #dont attack if no torpedos are available
        if self.getFrontLeftWeaponBattery().isEmpty():
            return

        #preventing from attacking if previous attack command was not completely done
        if self.cmdAttack != None:
            if not self.cmdAttack.wasExecuted(): 
                return
            
        #find the destination position that is nearest to the current orientation. this method of 
        #search may not be the best.
        positionInFront = self.getControlCenter().getGPS().getPosition() + \
                          self.getControlCenter().getGyroCompass().getForwardDirection()*400.0

        #find submarine
        nearestElement = self.getControlCenter().getMap().findNearestElement(positionInFront, 600, DangerLevel.MEDIUM, 0)
        if nearestElement.isValid():
            #keep current execution state in "mind"
            self.getCommandProcessor().push()
            self.getCommandProcessor().cleanup()
            #attack the object and restore previous command
            self.cmdAttack = attack(self, nearestElement.vecWorldTPosition)
            self.getCommandProcessor().execute(self.cmdAttack)
 
        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    

    #make some finalizations
    def finalize(self):
        self.cmdAttack = None


#creates a macro command that let the submarine move endless along a cube edge path around a position given in world coordinates
def searchOnPath(submarine, position):
    cmd = MacroCommand.create('SearchOnPath')    
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2,  SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3( SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2,  SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3( SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2)))

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-SEARCH_CUBE_EDGE_LENGTH/2, SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-SEARCH_CUBE_EDGE_LENGTH/2, SEARCH_CUBE_EDGE_LENGTH/2,  SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3( SEARCH_CUBE_EDGE_LENGTH/2, SEARCH_CUBE_EDGE_LENGTH/2,  SEARCH_CUBE_EDGE_LENGTH/2)))
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3( SEARCH_CUBE_EDGE_LENGTH/2, SEARCH_CUBE_EDGE_LENGTH/2, -SEARCH_CUBE_EDGE_LENGTH/2)))
    #repeat moving for ever
    cmd.attach(RepeatCommand.create(-1))
    return cmd



#creates a macro command that makes the submarine attack an object at the given position
def attack(submarine, position):
    cmd = MacroCommand.create('Attack')
    #switch off all engines
    cmd.attach(CSWCommands.disableAllEngines(submarine))
    #orientates toward the attacking position
    cmd.attach(Commands.rotateCommand(submarine, position))
    #fire one ordinary torpedo
    cmd.attach(CSWCommands.fireSalvo(submarine.getFrontLeftWeaponBattery(), 1, 1.0))
    cmd.attach(CSWWaitCommand.create(2))
    #after end of execution of attack command restore what was in "mind" before 
    cmd.attach(PopCommand.create())
    return cmd
