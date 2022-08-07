# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to deal with threads. two threads are created that asynchronously tries to create
#and execute moving commands.

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
import time
from threading import *

import sys
sys.path.append('./pylib/commands')
import Commands


class PurpleSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.63, 0.13, 0.55, 1))
        
        #create and start the two threads
        self.t1 = myThread(self)
        self.t1.start()
        self.t2 = myThread(self)
        self.t2.start()


    #this method must be overriden and is called every 10ms
    def update(self):
        pass
                    

    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    

    #make some finalizations
    def finalize(self):
        #stop the threads
        self.t1.stop()
        self.t1.join()
        self.t1 = None
        self.t2.stop()
        self.t2.join()
        self.t2 = None



#protect the submarine "resource" to ensure that only one thread access to the submarine at the same time.
lockSubmarine = RLock()


#the specialized thread that generates moving commands
class myThread(Thread):
    def __init__ (self, submarine):
        Thread.__init__(self)
        self.submarine = submarine
        self.cancel = 0

    def __del__(self):
        self.stop()
        self.submarine = None

    def stop(self):
        #on stop, wait till the loop has finished
        if not self.is_alive(): return
        self.cancel = 1
        while self.is_alive():
            time.sleep(0.1)
   
    #overridden method that does the threads work
    def run(self):
        self.cancel = 0
        while not self.cancel:
            #try to lock "resource". on failure continue without blocking.
            if lockSubmarine.acquire(0):
                #if command processor is idle, execute a moving command.
                if not self.submarine.getCommandProcessor().isBusy():
                    position = Vector3(random.random()*2000 - 1000, random.random()*2000 - 1000, random.random()*2000 - 1000)
                    self.submarine.getCommandProcessor().execute(Commands.MoveCommand(self.submarine, position))
                #release lock
                lockSubmarine.release()
            #sleep to enable other threads get processor time
            time.sleep(0.25)
