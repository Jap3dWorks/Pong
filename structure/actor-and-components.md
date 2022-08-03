# Actor and Components

Actors are transformation Matrix combined with components.

Actors are stored in Map class and Components are stored in Actors.

A Component should connected to a unique Actor, so actors will store Components as `std::unique_ptr`_,_ If a component is instanciable should be resolved internally using `std::shared_ptr`.

## Actors

Dynamic actors and static actors

## Components


