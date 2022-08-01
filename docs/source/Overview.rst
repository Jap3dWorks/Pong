==============
Overview
==============

--------
Abstract
--------
Pong Engine is a personal project for learning porpoises.
The idea is to build a basic functional 3d engine.
Core Technologies are c++ and OpenGl.


------------
Basic Design
------------


- Window: outputs images to a window
- Render: Generates images
- Scene: Container of the current objects and it's configurations. It's function is to be a container.
- Level: Logic above an scene object.
- Actor: Object with transforms, It's behabiour is defined with components.
- Component: Adition to an actor, components can be Graphic component, Collision component or simple Component


.. uml:: overview_uml.txt


----
TODO
----
- Actor: compose actor behaviour using components
- CameraComponent
-
