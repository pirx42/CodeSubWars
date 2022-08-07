# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to create specialized commands. a command is defined that lay mines if some conditions 
#are full filled. the command finishes if no mine is available. After laying the mines a comand is executed that 
#lets the submarine move weird through the deep "space" for ever. these commands are created and executed at the 
#beginning by the submarine.

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
sys.path.append('./submarines/IndigoSubmarineModules')
from IndigoSubmarineCommands import *


class IndigoSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.5, 0.0, 1.0, 1))
  
        #move in large circles
        self.getMainEngine().setDirection(15, 0)
        self.getMainEngine().setIntensity(1)

        #build execution queue:
        # 1. lay the mines
        # 2. move weird around
        self.getCommandProcessor().execute(LayMinesCommand(self, 55))
        self.getCommandProcessor().execute(WeirdMovingCommand(self))


    #this method must be overriden and is called every 10ms
    def update(self):
        pass        

        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
