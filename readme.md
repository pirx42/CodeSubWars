CodeSubWars
===================

A physics based three dimensional programming game.


Content of archive
------------------
The archive contains the following file structure (only important files/folders are 
described):
- root
  - CodeSubWars.exe   The main executable.
  - changes.md        The detailed list of changes.
  - preferences.cfg   The current stored preferences.
  - readme.md         This file.
  - doc
    - Manual.pdf      The common description of the codesubwars environment.
    - html
      - index.html    The main entry point for the html based reference documentation.
  - plugins           Qt plugins
  - pylib             The python library main folder containing useful common python 
                      code for implementing submarines.
  - submarines        This folder contains available subarmines that can be loaded on 
                      start of a battle.
  

Installation
------------
* get and install python 3.10 (available at http://www.python.org) 
  - make sure Path environment variable is updated
* extract the archive into a folder e.g. c:\program files\CodeSubWars 
* start the application e.g. located in c:\program files\CodeSubWars\CodeSubWars.exe


System requirements
-------------------
* Windows 10


Features
--------
* Easy to implement new submarines using python
* Easy to set up new behavior by using predefined commands
* Easy to implement completely new commands using python
* Three dimensional environment
* Physics based environment


Known bugs/issues
-----------------
* Restarting a new battle that contains submarines which uses graphical user interface 
  environment (tkinter) works not properly.
* Replaying long battle records takes very long loading time.
* When a battle is stopped, not all memory is freed.
* Cause of beeing able to create references to itself (e.g. by holding self in member 
  variable of submarine) circular references could be created. This leads to memory 
  leaks.
* Restarting a new battle prints out lots of warning messages in console window like 
  "RuntimeWarning: to-Python converter for XXX already registered; second conversion 
  method ignored."


History
-------
* Version 0.4.7b (2022/08/07)
  - new world with active rocks
  - switched to most current python (3.10.5)
  - built as 64 bit application
  - some bugs removed

* Version 0.4.6b (2007/01/02)
  - new example submarine
  - some bugs removed

* Version 0.4.5b (2006/09/24)
  - new weapons: magnetic mine, passive sonar guided torpedo
  - example submarines now uses faster rotation and move commands
  - threads can now be used within python
  - several major and minor bugs removed

* Version 0.4.0b (2006/01/21)
  - commands can be implemented in python
  - new example submarine 
  - reference documentation improved
  - several minor bugs removed

* Version 0.3.1b (2005/11/26)
  - serious bug regarding to collision detection fixed
  - pylib subfolder introduced that contains useful common python code

* Version 0.3.0b (2005/09/02)
  - new equipment added: passive sonar
  - visualization improved
  - new example submarines
  - performance improved
  - some bugs were fixed

* Version 0.2.0b (2005/07/29)
  - new equipment added: map 
  - new silent mode added which has no graphical output
  - replaying of battles is now possible
  - weapon batteries can now be recharged on weapon supplies
  - new basic commands are available: repeat, push, pop
  - performance improved
  - some bugs were fixed

* Version 0.1.0b (2005/05/22)
  - Initial release

See changes.md for more detailed description.


Copyright/License
-----------------
Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
Released under the MIT license. (see license.txt)


For bug reports contact bug@codesubwars.org.
Watch out http://www.codesubwars.org for upcoming releases.

If you have any suggestion please do not hesitate to contact pirx@codesubwars.org.
