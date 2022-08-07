# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#the behavior is simple: attack every detected object except other weapons if torpedos are available.
#the behavior of this submarine bases on a little state machine. on each state transition a command stack is
#generated and executed. this generally shows how to deal with commands.
#its behavior bases on a not very nice implemented state machine.

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


class YellowSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(1, 0.8, 0, 1))
        self.states = ['SLEEPING', 'ATTACKING', 'ESCAPING']
        self.currentState = 'UNKNOWN'
        self.changeToState(self.states[0])
        
        #the last global position where an object or an explosion was detected
        self.vecLastInterestedGlobalPosition = Vector3(0.0, 0.0, 0.0)
        self.bTargetDetected = 0

        
    #this method must be overriden and is called every 10ms
    def update(self):
        #check whether a state transition could be done
        self.checkForStateTransitions()

        #if the state has not changed do nothing
        if self.currentState == self.newState: return

        #make the state changed
        self.currentState = self.newState
        
        #state has changed: execute stuff on enter of the new state
        #sleeping
        if self.currentState == self.states[0]:
            self.getCommandProcessor().cleanup()
            #power off all engines
            self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
            #full stop
            self.getCommandProcessor().execute(CSWCommands.stopAll(self))
            #rotate about forward axis to scan larger region
            self.getCommandProcessor().execute(CSWCommands.setAxialInclinationJetOar(self, 1))                
            
        #attacking
        elif self.currentState == self.states[1]:
            self.getCommandProcessor().cleanup()
            #power off all engines
            self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
            #rotate as fast as possible to be oriented toward detected object
            self.getCommandProcessor().execute(Commands.rotateCommand(self, self.vecLastInterestedGlobalPosition))
            #fire 2 ordinary (green) torpedos
            self.getCommandProcessor().execute(CSWCommands.fireSalvo(self.getFrontLeftWeaponBattery(), 2, 2.5))
            #move to shooting distance of 400m, the end velocity is not important. with this distance and angle the rotation 
            #resolution is 0.7m
            fDistance = (self.getControlCenter().getGPS().getPosition() - self.vecLastInterestedGlobalPosition).getLength()
            self.getCommandProcessor().execute(CSWCommands.moveForward(self, -(400 - fDistance), 0, 100))
            #fire a ordinary torpedo
            self.getCommandProcessor().execute(CSWFireCommand.create(self.getFrontLeftWeaponBattery()))
            #return to sleep state
            self.getCommandProcessor().execute(ChangeToStateCommand(self, self.states[0]))

        #escaping
        elif self.currentState == self.states[2]: 
            vecDirection = self.getControlCenter().getGPS().getPosition() - self.vecLastInterestedGlobalPosition
            if vecDirection.getLength() < 400:
                self.getCommandProcessor().cleanup()
                #power off all engines
                self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
                #set main engine to 20% forward
                self.getCommandProcessor().execute(CSWCommands.setMainEngineIntensity(self, 0.2))
                #rotate as fast as possible to be contra oriented toward detected object and move 200 meter
                vecPosition = self.getControlCenter().getGPS().getPosition() + vecDirection.getNormalized()*200;
                self.getCommandProcessor().execute(Commands.MoveCommand(self, vecPosition))
                #return to sleep state
                self.getCommandProcessor().execute(ChangeToStateCommand(self, self.states[0]))
        
        #on any other state
        else:
            self.getCommandProcessor().cleanup()
            #power off all engines
            self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
            #return to sleep state
            self.getCommandProcessor().execute(ChangeToStateCommand(self, self.states[0]))
                     
                        
    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):
        message = event.getMessage()
        if isinstance(message, CSWExplosionDetectedMessage):
            self.vecLastInterestedGlobalPosition = message.getPoint()
            self.changeToState(self.states[2])

        elif isinstance(message, CSWCollisionDetectedMessage):
            self.vecLastInterestedGlobalPosition = message.getPoint()
            self.changeToState(self.states[2])
            
        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1
    
    
    #do a state transition to attacking or excaping if a submarine was found
    def checkForStateTransitions(self):
        if self.currentState != self.states[2] and \
           self.getControlCenter().getActiveSonar().hasTargetDetected() and \
           self.getControlCenter().getActiveSonar().isTargetDynamic() and \
           not self.getControlCenter().getActiveSonar().isTargetWeapon():

            self.vecLastInterestedGlobalPosition = self.getControlCenter().getActiveSonar().getTargetPosition()
           
            #submarine was found. if weapons are available go attacking else escape
            if not self.getFrontLeftWeaponBattery().isEmpty():
                self.changeToState(self.states[1])
            else:
                self.changeToState(self.states[2])

    #change to new state. is also called through the ChangeToStateCommand.
    def changeToState(self, newState):
        self.newState = newState



        
        
#this command makes the submarine change the internal state.
class ChangeToStateCommand(CSWPyCommand):
    def __init__(self, param, newState = None):
        if isinstance(param, self.__class__): 
            #construct a new command from the given one (copy construction).
            CSWPyCommand.__init__(self, param)
            self.submarine = param.submarine
            self.newState = param.newState
        else: 
            #construct a new command.
            CSWPyCommand.__init__(self)
            self.submarine = param
            self.newState = newState

    #this method must be overriden and is called directly before the first step() is called.
    def initialize(self):
        pass
        
    #this method must be overriden and is called periodically about every 10ms while executing.
    def step(self):
        self.submarine.changeToState(self.newState)
        self.setProgress(1)
        self.finished()

    #this method must be overriden and is called directly after finishing of execution or on canceling.
    def cleanup(self):
        pass
    
    #this method must be overriden and returns the name of the command.
    def getName(self):
        return self.__class__.__name__
        
