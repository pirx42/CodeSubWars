* Version 0.4.7b (2022/08/07)
  - added:
     - new world default 5 available, which contains five active rocks.

  - changed:
    - build as win64-bit application
    - python 3.10.5 is now used.
    - several libraries updated eg qt 5.12, boost 1.79

  - fixed:
    - silent mode: total simulated time at end of battle was not determined correctly.


* Version 0.4.6b (2007/01/02)
  - added:
    - new example OliveSubmarine, that creates and uses a dialog as its own remote controller.
  
  - fixed:
    - direction changes of active/passive sonar and engines does not work properly.
  

* Version 0.4.5b (2006/09/24)
  - added:
    - new weapons: magnetic mine, passive sonar guided torpedo.
    - new example BeigeSubmarine, that follows detected submarines.
    - new example PurpleSubmarine, that handles with threads.
    - on starting a battle now a dialog occurs to select submarines that takes part on battle.
    - a running battle will now stopped if only one submarine/team is leftover. this can 
      be disabled.
    - threads can now be used within python.
    - sound emitting can be visualized optionally.
    - submarines can now override method finalize to clean up something before destruction.
     
  - changed:
    - dying submarines now explode.
    - the size of battle record files (*.cbr) are reduced.
    - menu restructured a bit.
    - Map now displays also element ids.
    - WeaponSupply now only provides GreenMines and GreenTorpedos.
    - team size can be selected from range [3, 5, 10].
    - Collision- and ExplosionDetectedMessage now contain the caused damage value to itself.
    - on collision the applied damage is now depend on difference velocity in collision
      direction.
    - angular velocity damping is now more realistically.
    - object collision shapes are now capsules (submarine, torpedo), spheres (mine) and boxes 
      (WeaponSupply, Rocks, Walls).
    - python 2.5 is now used.
    - removed some unused predefined macro commands.
    - minor improvements of example submarines.
    - reference documentation improved.
       
  - fixed:
    - submarines uses now faster rotation and move commands.
    - interpenetration of objects is no more possible. this regards to improvements in 
      collision detection by switching to library SOLID 3.5.4 and physics calculations. thus 
      objects are no more able to move outside the world cube.
    - objects are no more able to move and/or rotate very fast (e.g. caused by explosions).
    - objects, that are allowed to change its direction, are now restricted to a maximal 
      angular velocity (active/passiv sonar: 180 degree/second; main engine: 22.5 degree/second) 
    - application icon is now visible.
    

* Version 0.4.0b (2006/01/21)
  - added:
    - commands can now be implemented in python fully compatible to existing ones.
    - new example IndigoSubmarine added that uses simple self defined commands.
    - now weapons can additionally be fired delayed.
    - new world default 4 added that contains a "black hole" at position (0, 0, 0).
     
  - changed:
    - initializations can now be done in separate method initialize().
    - emitted impulses, damage and radius of weapons are now depend on distance and volume
      of weapon.
    - weapons contained by submarine are now no more depend on the submarines length. 
      each submarine as the same kind of weapons. only the weapon battery capacity is depend 
      on the length of the submarine. 
    - pylib: statemachine has now a context that can store useful information.
    - Black/Whitesubmarine improved.
    - boost 1.33.1 is now used.
    - reference documentation improved.
       
  - fixed:
    - help and manual menu entry now works correctly.
    - recharge of weapons has broken although it has not been finished.
    - calculation of getAngularVelocity(Vector3 p) corrected.
    - several minor bugs removed.


* Version 0.3.1b (2005/11/26)
  - added:
    - pylib subfolder was added that contains useful common python code. the first piece of 
      code is a table driven state machine.
     
  - changed:
    - code of example submarines (White/BlackSubmarine) improved.
    - python 2.4.2 is now used.
       
  - fixed:
    - visualized results of active and passive sonar now correspond to accessible results.
    - after long running times the application could sometimes crash. this regards to 
      accessing to invalid collision shapes.


* Version 0.3.0b (2005/09/02)
  - added:
    - battle statistics extended by hit rate and hits/kills per minute.
    - new equipment was added: passive sonar that detects the sound pressure level of engines
      and exploding weapons.
    - new example submarines for active and passive sonar (White/BlackSubmarine)
    - now engines can change its direction within a range (only the main engine of submarines 
      are able to change its direction). this can be done directly or by using a new
      commands named CSWSetEngineDirectionCommand.
     
  - changed:
    - visualization improved (health is displayed as bar, explosions, small objects are
      surrounded by yellow discs, selected object is visualized by frames).
    - engines and weapons are now sound emitter.
    - little performance improvements.
    - removed equipment: second active sonar.
    - now by default torpedos are armed 5 seconds after launching.
       
  - fixed:
    - rotation does not work correctly if absolut destination positions near to the back are 
      given.
    - visualization of boxes are now correct.
    - weapon now hits all objects within its explosion range.
    - weapon now can not be fired until it is armed.
    - dead lock occurs if exception was thrown in recalculation cycle.
    - active sonar determines not exactly the correct distance.
    - inserting an element into the map could cause creating a new map element although an 
      existing element was already in that near.


* Version 0.2.0b (2005/07/29)
  - added:
    - new equipment was added: map that stores detected objects with regarding information.
    - ative sonar has now different scanning modes (thanks to sebastian).
    - weapon batteries are now rechargeable. these object can request recharge to weapon supply.
    - new world (default 3) was added that has a weapon supply initially at position 0, 0, 0.
    - new sample submarine added: CyanSubmarine that shows advanced techniques with commands.
    - silent mode was added where the application runs without graphical output.
    - battles can now replayed using a simple viewer.
    - new basic commands are available that allow simple looping, push or pop of command 
      processor context.
     
  - changed:
    - displaying of sonar scans was improved.
    - performance improvements.
    - sample submarine GreenSubmarine shows now how to deal with maps and how to recharge weapon
      batteries.
    - capacity of front weapon batteries was decreased by factor 1.5 (in 110m long submarine 44 
      torpedos were loaded).
    - python 2.4.1 is now used.
    - log data will be stored in subpath /log.
    - now all (sub)objects within the object tree can be observed in the detail 3d view 
      (also not solid objects).
    - little handling improvement of object tree view.
    - zoom in/out works now by pressing middle mouse button and moving up/down or by using the 
      mouse wheel (thanks to martin).
    - now only pentium pro or higher is required and no sse.
       
  - fixed:
    - system crash occurs if clicking into view which has no camera selected (thanks to martin).
    - looking like memory leak regarding to event management is fixed.
    - under some conditions the rotation could not terminate.
    - the resulting impuls direction for explosions was wrong calculated.


* Version 0.1.0b (2005/05/22)
  - Initial release
