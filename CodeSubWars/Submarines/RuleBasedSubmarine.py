# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine

#rules:
#entkoppeln von scanning und manuvering kram durch map. (scanning schreibt in map. manuvering liest nur aus map.)
#different state machines for scanning, manuvering/attacking

#todo:
#es sollte vor dem merken vom weaponsupply zumindest geprueft werden ob an der stelle wirklich ein grosser klotz ist ...                    


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
import RuleBasedSubmarineScanner
import RuleBasedSubmarineManuver
import Constants



class RuleBasedSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(1.0, 1.0, 1.0, 1))

        #create the controller that implements the behavior of scanning
        self.scanController = RuleBasedSubmarineScanner.scanController
        #the reference to the submarine itself                                           
        self.scanController.context.submarine = self                                           

        #create the controller that implements the behavior of scanning
        self.manuverController = RuleBasedSubmarineManuver.manuverController
        #the reference to the submarine itself                                           
        self.manuverController.context.submarine = self                                           
        self.manuverController.context.wasHit = 0
        self.manuverController.context.vecWorldTLastHitPosition = Vector3(0, 0, 0)
        self.manuverController.context.wasCollided = 0
        self.manuverController.context.vecWorldTLastCollisionPosition = Vector3(0, 0, 0)


    #this method must be overriden and is called every 10ms
    def update(self):
        #updates the controller to do available transitions depend on the given transition table
        self.scanController.updateStates()
        self.manuverController.updateStates()

        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):    
        message = event.getMessage()
        if isinstance(message, CSWTextMessage):
            #if the text message contains WeaponSupply assume that this is a message from weapon supply
            if message.getText().find('WeaponSupply') >= 0:

                #weapon supply is available. so extract information
                try:
                    splitted = message.getText().split()
                    position = Vector3(float(splitted[2]), float(splitted[3]), float(splitted[4]))
                    self.getControlCenter().getMap().insertElement(position, Constants.WEAPON_SUPPLY_ID, 0)
                except:
                    pass
            
        elif isinstance(message, CSWExplosionDetectedMessage):
            self.manuverController.context.wasHit = 1
            self.manuverController.context.vecWorldTLastHitPosition = message.getPoint()

        elif isinstance(message, CSWCollisionDetectedMessage):
            self.manuverController.context.wasCollided = 1
            self.manuverController.context.vecWorldTLastCollisionPosition = message.getPoint()
            
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1


    #make some finalizations
    def finalize(self):
        self.scanController.context.submarine = None
        self.manuverController.context.submarine = None
