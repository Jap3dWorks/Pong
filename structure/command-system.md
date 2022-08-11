---
description: '#include "command.h"'
---

# Command

Command system is reponsible of execute some actions at engine level.

Command system are composed of 5 key classes

* **Command**, a class that represents a command, all generated commands must inherit from Command class.
* **CommandFactory**, Template class responsible of generate Command instances, this class is used as value in CommandReg::command\_register. This way we can assign a string literal to a command class, because CommandFactory\<Command>::create() will return a command instance ptr.\
  `CommandFactory<CommandType>::create() -> unique_ptr<CommandType>`
* **CommandReg**, Relation between command names and command classes, all commands must be registered in CommandReg
* **CommandQueue**, A thread safe FIFO unique\_ptr\<Command> queue, the queue must be the location of every Command created, later Engine will be resolving each Command in it's own thread.&#x20;
* **CommandParser**, This class should manage the input text and is the responsible in search and found the pertinent Comman (inside CommandRef).
