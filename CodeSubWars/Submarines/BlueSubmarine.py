# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine lays a little mine field consisting of 8 mines. it shows how you can create your own "predefined" 
#commands. 

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


class BlueSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 50)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.2, 0.2, 1, 1))
        self.getCommandProcessor().execute(layMineFieldAroundPosition(self, Vector3(0, 0, 0)))


    #this method must be overriden and is called every 10ms
    def update(self):
        pass

        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    



#creates a command that lays a mine field (8 mines) in the x-z plane around a position given in world coordinates
def layMineFieldAroundPosition(submarine, position):
    cmd = MacroCommand.create('LayMineField')
    
    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-200, 0, -200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-200, 0, 0)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(-200, 0, 200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(0, 0, 200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(200, 0, 200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(200, 0, 0)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(200, 0, -200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));

    cmd.attach(Commands.MoveCommand(submarine, position + Vector3(0, 0, -200)));
    cmd.attach(CSWFireCommand.create(submarine.getBackWeaponBattery()));
    return cmd
