# CodeSubWars - A physics based three dimensional programming game

This is a programming game. Here you can define the behaviour of autonomous submarines. The submarines fight against each other in teams or alone with torpedos and mines. The world is 3d and is based on simple physics. To define a submarine behavior the object oriented programming language python is used. Its up to you if they are smart enough to survive.  
  
The **goal** is to create a submarine that is alive at last.

<p float="left" align="center">
  <img src="https://github.com/pirx42/CodeSubWars/blob/main/CodeSubWars/Screenshots/screenshot.PNG" width="32%" align="top" alt="ui while battle running">
  <img src="https://github.com/pirx42/CodeSubWars/blob/main/CodeSubWars/Screenshots/screenshot4.PNG" width="32%" align="top" alt="passive sonar">
  <img src="https://github.com/pirx42/CodeSubWars/blob/main/CodeSubWars/Screenshots/screenshot7.png" width="32%" align="top" alt="submarine attacks">
</p>

## Features

* Easy to implement new submarines using python ([link](http://www.rose.st/codesubwars.org/doc/html/class_code_sub_wars_1_1_c_s_w_py_submarine.html))
* Easy to set up new behavior by using predefined commands ([link](http://www.rose.st/codesubwars.org/doc/html/class_code_sub_wars_1_1_c_s_w_commands.html))
* Easy to implement completely new commands using python ([link](http://www.rose.st/codesubwars.org/doc/html/class_code_sub_wars_1_1_c_s_w_py_command.html))
* Three dimensional environment
* Physics based environment

## Documentation

Detailed information about user interface, running modes and submarine creating can be found in [manual.pdf](https://github.com/pirx42/CodeSubWars/blob/main/CodeSubWars/doc/manual.pdf).  
The reference documentation is at [html documentation](http://www.rose.st/codesubwars.org/doc/html/index.html).

## Build

### How to build for Windows 10 64bit with Visual Studio 2022 Commnunity

1. install [python 3.10](https://www.python.org)
2. install prebuild [qt 5.12](https://www.qt.io)
3. build boost.python
    1. add in environment: `PYTHON_ROOT=<path to installed python root folder>` (eg `C:\Users\pirx\AppData\Local\Programs\Python\Python310`)
    2. open x64 native tools command prompt from Visual Studio 2022
    3. go to boost root folder in repository
    4. call `bootstrap.bat`
    5. call `b2.exe toolset=msvc-14.3 address-model=64 link=shared --with-python -j8`
4. open `CodeSubWars.sln` solution in Visual Studio 2022 and build

## History

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

See [changes.md](https://github.com/pirx42/CodeSubWars/blob/main/changes.md) for more detailed description.

## Known bugs/issues

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

## Copyright/License

Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
Released under the MIT license. (see [license.txt](https://github.com/pirx42/CodeSubWars/blob/main/LICENSE.txt))  
For detailed copyright notification see [CopyrightNotifications.txt](https://github.com/pirx42/CodeSubWars/blob/main/CopyrightNotifications.txt).
