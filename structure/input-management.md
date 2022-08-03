# Input management

Input system should translate a queue of inputs in events.

Any void(...) function in the engine could subscribe to an event.

Input class structure

* Event,&#x20;
* InputReg, for each input possible(keys and joystics) an Event object could exists. Event object could manage a boolean  input or a analogic input (e.g joystick vector and magnitude)
* InputQueue, a thread safe FIFO queue, each iteration of the Engine will check the InputQueue and execute all Event classes waiting in the InputQueue (Engine will determine first the quantity of events and later resolve only that number of Events).
* InputLogic, Input logic is the responsible of input detection and order inside InputQueue. InputLogic will be running in a self thread, and many InputLogics could coexists at same time.

Time between same inputs keys needs to be resolved. determine if a key is pressed or released or  if stay pressed and how much time.
