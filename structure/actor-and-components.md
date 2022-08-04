# Actor and Components

Actors are transformation Matrix combined with components.

Actors are stored in Map class and Components are stored in Actors.

A Component should connected to a unique Actor, so actors will store Components as `std::unique_ptr`_,_ If a component is instanciable should be resolved internally using `std::shared_ptr`.

## Actors

Two types, **Dynamic** actors and **Static** actors

## Components

CameraComponent, GraphicComponent, CollisionComponent, ScriptComponent and LightComponent

### GraphicComponent

Represents a renderable component, So should store a Material\* and a Mesh\* or any Shape data struct. Also the OpenGl ID.

Graphic Component should not contains the logic for openGL binding buffers. Should delegate this functionality to other system.

