how to build for windows 64bit with visual studio 2022 commnunity

1. install python (python.org)
2. install prebuild qt 5.12 (qt.io)
3. build boost.python
  1. add in environment: PYTHON_ROOT=<path to installed python root folder> (eg C:\Users\pirx\AppData\Local\Programs\Python\Python310)
  2. open x64 native tools command prompt from visual studio 2022
  3. go to boost root folder
  4. call 'bootstrap.bat'
  5. call 'b2.exe toolset=msvc-14.3 address-model=64 link=shared --with-python -j8'
4. open CodeSubWars solution and build