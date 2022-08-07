# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#sample submarine
#this submarine shows how to deal with graphical output (in this case tkinter). a control panel is created that allows
#the user to control very little submarine functionality remotely. because submarines should be autonomously, this should 
#used for debug purposes only.

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

import time

from threading import *
from tkinter import *

import sys
sys.path.append('./pylib/misc')
import TkinterSupport


class OliveSubmarine(CSWPySubmarine):
    #on construction set length of submarine
    def __init__(self):
        CSWPySubmarine.__init__(self, self.__class__.__name__, 110)
        if not getVersion() == Version('0.4'):
            raise(Exception, 'incompatible version of csw framework')
        self.envControlPanel = None


    #make some initializations
    def initialize(self):
        self.setColor(Vector4(0.42, 0.56, 0.14, 1))

        #construct a thread that encapsulates the graphical output
        self.envControlPanel = DialogEnvironment(self)
        #create the gui elements and start the mainloop of the gui within the previously created thread
        self.envControlPanel.start()

          
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
        #stop and destruct gui
        self.envControlPanel.stop()
        self.envControlPanel.join()
        self.envControlPanel = None



#the control panel frame consisting of button and state information widgets
class ControlPanel(Frame):
    def __init__(self, master, submarine):
        Frame.__init__(self, master)
        Grid.config(self)
        self.submarine = submarine

        Label(self, text=self.submarine.getName()).grid(row = 0, columnspan = 3, sticky = W+E)

        #create button widgets with bindings to regarding commands
        self.btnFireLeft = Button(self, text='fireLeft', foreground='black', command=self.doFireLeft)
        self.btnFireLeft.grid(row = 1, column = 0, sticky = W+E)
        Button(self, text='forward', foreground='black', command=self.doForward).grid(row = 1, column = 1, sticky = W+E)
        self.btnFireRight = Button(self, text='fireRight', foreground='black', command=self.doFireRight)
        self.btnFireRight.grid(row = 1, column = 2, sticky = W+E)
        Button(self, text='left', foreground='black', command=self.doLeft).grid(row = 2, column = 0, sticky = W+E)
        Button(self, text='stop', foreground='black', command=self.doStop).grid(row = 2, column = 1, sticky = W+E)
        Button(self, text='right', foreground='black', command=self.doRight).grid(row = 2, column = 2, sticky = W+E)
        Button(self, text='backward', foreground='black', command=self.doBackward).grid(row = 3, column = 1, sticky = W+E)
        
        #create state information displaying widgets
        Label(self, text='state:').grid(row = 4, column = 0, sticky = W+E)
        self.labelState = Label(self, text='stopped')
        self.labelState.grid(row = 4, column = 1, sticky = W+E)
        
        #start periodically refreshing of state information widgets
        self.refresh()

    def refresh(self):
        #update displayed state information
        if self.submarine.getControlCenter().getMovingPropertiesSensor().getVelocity().getLength() < 0.01 and \
           self.submarine.getControlCenter().getMovingPropertiesSensor().getAngularVelocity().getLength() < 0.01:
            self.labelState['text'] = 'stopped'
        else:
            self.labelState['text'] = 'moving'
        
        #disable left fire button if no weapons are available    
        if self.submarine.getFrontLeftWeaponBattery().isEmpty():
            self.btnFireLeft.config(state=DISABLED)          
        else:
            self.btnFireLeft.config(state=NORMAL)

        #disable right fire button if no weapons are available    
        if self.submarine.getFrontRightWeaponBattery().isEmpty():
            self.btnFireRight.config(state=DISABLED)          
        else:
            self.btnFireRight.config(state=NORMAL)
          
        #call refresh again after at least 100ms
        self.after(100, self.refresh)

    def doFireLeft(self):
        self.submarine.getFrontLeftWeaponBattery().releaseNext(0)

    def doFireRight(self):
        self.submarine.getFrontRightWeaponBattery().releaseNext(0)

    def doForward(self):
        self.submarine.getBowsJetOar().setIntensity(0)
        self.submarine.getMainEngine().setDirection(0, 0)
        self.submarine.getMainEngine().setIntensity(1)

    def doBackward(self):
        self.submarine.getBowsJetOar().setIntensity(0)
        self.submarine.getMainEngine().setDirection(0, 0)
        self.submarine.getMainEngine().setIntensity(-1)

    def doLeft(self):
        self.submarine.getBowsJetOar().setIntensity(-1)
        self.submarine.getMainEngine().setDirection(45, 0)
        self.submarine.getMainEngine().setIntensity(1)

    def doRight(self):
        self.submarine.getBowsJetOar().setIntensity(1)
        self.submarine.getMainEngine().setDirection(-45, 0)
        self.submarine.getMainEngine().setIntensity(1)

    def doStop(self):
        self.submarine.getBowsJetOar().setIntensity(0)
        self.submarine.getMainEngine().setDirection(0, 0)
        self.submarine.getMainEngine().setIntensity(0)



#the threaded environment that enables starting of gui elements containing an evenloop
class DialogEnvironment(Thread):
    def __init__ (self, submarine):
        Thread.__init__(self)
        self.submarine = submarine
        self.quitter = None

    def __del__(self):
        self.stop()
        self.submarine = None
        self.quitter = None

    def stop(self):
        if not self.is_alive(): return
        if self.quitter == None: return
        self.quitter()
        while self.is_alive():
            time.sleep(0.05)
   
    def run(self):
        master = TkinterSupport.TkSupport()
        dlg = ControlPanel(master, self.submarine)
        self.quitter = dlg.quit
        master.mainloop()
