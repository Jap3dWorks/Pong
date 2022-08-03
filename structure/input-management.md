# Input management

Input system should translate a queue of inputs in events.

Events could be Commands or any other interaction system, like the update function of a component.

Input class structure

* InputReg, for each input possible(keys and joystics) an Event object could exists. Event object could manage a boolean  input or a analogic input (e.g joystick vector and magnitude)
* InputQueue, a FIFO queue, each iteration of the Engine, this will check the InputQueue and execute all Event classes waiting in the InputQueue (Engine will determine first the quantity of events and the resolve only that number of Events).
* InputLogic, Input logic is the responsible of input detection and order inside InputQueue.
