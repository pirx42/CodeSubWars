# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#the behavior makes only sense in team mode. the teammates share information about its positions and 
#positions of enemies. each submarine attacks the nearest known enemy.
#very simple implementation of an identification, friend or foe (IFF).
#all submarine instances of this class sends its position and detected enemie positions to the other ones. 
#each submarine inserts the positions of its teammates and the known enenmies to its own map.
#a very simple and of course not robust implementation against message spoofing.

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

#the time in seconds between the messages to the teammates are sent
MESSAGE_SEND_DELAY = 2.0
#the period in seconds of which the map data will be cleared periodically to remove wrong detected or moved objects.
KEEP_MAP_DATA_TIME = 30.0
#the id that is inserted in the map to regarding positions of teammates.
TEAMMATE_ID = 1024


class PinkSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.96, 0.38, 1, 1))
        self.strMyID = str(id(self))
        self.strMyGroupID = '9132547913716345378714' + self.__class__.__name__ + '1148796103143009740'
        
        #time in seconds of the begin the last quiet period within no messages are sent
        self.lastQuietTime = -9999
        #time in seconds of sending the last message
        self.lastSendTime = -9999

        self.getAxialInclinationJetOar().setIntensity(1)                


    #this method must be overriden and is called every 10ms
    def update(self):
        currentTime = CSWTime.getTime()
        
        if currentTime - self.lastQuietTime > KEEP_MAP_DATA_TIME + MESSAGE_SEND_DELAY:
            self.lastQuietTime = currentTime
        elif currentTime - self.lastQuietTime > KEEP_MAP_DATA_TIME:
            #dont send or accept messages to/from teammates because possibly wrong detected or moved objects should be removed
            #from the map
            self.getControlCenter().getMap().clear()
            
        #broadcast an "encoded" message in period of about MESSAGE_SEND_DELAY seconds
        elif currentTime - self.lastSendTime > MESSAGE_SEND_DELAY:
            self.lastSendTime = currentTime
            
            #find all dangerous things but friends in radius of 2km around the current position
            enemyContainer = self.getControlCenter().getMap().findElements(self.getControlCenter().getGPS().getPosition(), 2000, \
                                                                           DangerLevel.NONE | DangerLevel.LOW | DangerLevel.MEDIUM | DangerLevel.HIGH, \
                                                                           TEAMMATE_ID)
            enemies = ''
            tmp = [' ' + str(e.vecWorldTPosition) for e in enemyContainer]
            enemies = ''.join(tmp)
 
            #send message consisting of own and known enemie positions
            self.sendMessage(str(self.getControlCenter().getGPS().getPosition()) + enemies)

        #attack nearest known enemie
        if not self.getCommandProcessor().isBusy() and not self.getFrontLeftWeaponBattery().isEmpty():

            #find the destination position that is nearest to the current orientation. this method of 
            #search may not be the best.
            positionInFront = self.getControlCenter().getGPS().getPosition() + \
                              self.getControlCenter().getGyroCompass().getForwardDirection()*200.0

            #find nearest submarine except teammates
            nearestElement = self.getControlCenter().getMap().findNearestElement(positionInFront, 2000, DangerLevel.MEDIUM, TEAMMATE_ID)
            if nearestElement.isValid():
                self.getCommandProcessor().cleanup()
                self.getCommandProcessor().execute(CSWCommands.disableAllEngines(self))
                self.getCommandProcessor().execute(Commands.rotateCommand(self, nearestElement.vecWorldTPosition))
                self.getCommandProcessor().execute(CSWFireCommand.create(self.getFrontLeftWeaponBattery()))
                self.getCommandProcessor().execute(CSWCommands.setAxialInclinationJetOar(self, 1))                
                self.getCommandProcessor().execute(CSWWaitCommand.create(5))


    #this method must be overriden and is called if an event has arrived
    def processEvent(self, event):        
        #dont process messages from other teammates within the quiet phase 
        message = event.getMessage()
        if isinstance(message, CSWTransceiverMessage) and self.isMessageValid(message) and \
           CSWTime.getTime() - self.lastQuietTime < KEEP_MAP_DATA_TIME:
            #message from another teammate was received
            #extract the position information                
            positions = toVector3List(extractInformation(message))

            #update known teammate position
            self.getControlCenter().getMap().insertElement(positions[0], TEAMMATE_ID, 0xffffffff)

            #update known enemie positions
            for i in range(1, len(positions)): 
                self.getControlCenter().getMap().insertElement(positions[i], DangerLevel.MEDIUM, 0xffffffff)

        #returning true means that the event was successfully processed. it can removed from event queue.
        #returning false will keep the event. the next time the event will be delivered to this object again.
        return 1


    #sends an "encoded" message. format is: <current time> <group id> <id> <message> 
    def sendMessage(self, message):
        self.getControlCenter().getTransceiver().send(str(CSWTime.getTime()) + ' ' + \
                                                      self.strMyGroupID + ' ' + \
                                                      self.strMyID + ' ' + \
                                                      message)
        
        
    #check if the given message is valid
    def isMessageValid(self, message):
        try:
            currentTime = CSWTime.getTime()
            stringList = message.getText().split()
            fMessageSendTime = float(stringList[0])
            strMessageSenderGroupID = stringList[1]
            strMessageSenderID = stringList[2]
            
            #a message is only valid if the send time is near to current time and if groupid and senderid is valid
            return fMessageSendTime > currentTime - 0.1 and fMessageSendTime <= currentTime and \
                   strMessageSenderID != self.strMyID and \
                   strMessageSenderGroupID == self.strMyGroupID
        except:
            return 0




def extractInformation(message):
    try:
        return message.getText().split(' ', 3)[3]
    except:
        return ''


#converts string into vector sequence
def toVector3List(string):
    try:
        splitted = string.split()
        return [Vector3(float(splitted[i + 0]), float(splitted[i + 1]), float(splitted[i + 2])) for i in range(0, len(splitted), 3)]
    except:
        return []

