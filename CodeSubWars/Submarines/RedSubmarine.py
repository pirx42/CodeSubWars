# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine will be constructed in different sizes. the first instance will be 30m,
#the second 60m and third 90m length.

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


class RedSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, getNumberOfCurrentInstance(self.__class__.__name__)*30)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')

    #make some initializations
    def initialize(self):
        self.setColor(Vector4(1, 0.1, 0, 1))

    
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
        decreaseNumInstances(self.__class__.__name__)



#returns the number of currently existing instances (including the instance of the caller)
def getNumberOfCurrentInstance(name):
    try:
        f = open(name + '.cfg', 'r')
        numInstances = int(f.readline()) + 1
        f.close()
        if numInstances > 3:
            numInstances = 3
        f = open(name + '.cfg', 'w')
        f.write(str(numInstances))
        f.close()
        return numInstances
    except:
        f = open(name + '.cfg', 'w')
        f.write(str(1))
        f.close()
        return 1


#decreases the number of currently existing instances
def decreaseNumInstances(name):
    try:
        f = open(name + '.cfg', 'r')
        numInstances = int(f.readline()) - 1
        f.close()
        if numInstances >= 0:
            f = open(name + '.cfg', 'w')
            f.write(str(numInstances))
            f.close()
    except:
        f = open(name + '.cfg', 'w')
        f.write(str(0))
        f.close()
        