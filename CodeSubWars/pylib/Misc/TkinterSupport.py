# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#tkinter wrapper to support multiple instances per process.
import os
from tkinter import *

class TkSupport(Tk):
    def __init__(self, screenName=None, baseName=None, className='Tk',
                 useTk=1, sync=0, use=None):
        Tk.__init__(self, screenName, baseName, className, useTk, sync, use)
        
    def mainloop(self):
        #due to the possibility that multiple submarines use dialogs the first 
        #caller must be determined. the mainloop must be called only once per 
        #process. 
        lockFileName = 'tksupport_' + str(os.getpid()) + '.lck'
        if not os.path.exists(lockFileName): 
            open(lockFileName, 'w')
            Tk.mainloop(self)
            os.remove(lockFileName)
  