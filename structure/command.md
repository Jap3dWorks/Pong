# Command

Command system is reponsible of execute some actions at engine level.

Command system are composed of 4 key classes

* Command, a class that represents a command, all generated commands must inherit from Command class.
* CommandFactory, Template class responsible of generate Command instances, this class is used as value in CommandReg::command\_register. This way we can assign a string literal to a command class, because CommandFactory\<Command>::create() will return a command instance ptr.\
  `CommandFactory<CommandType>::create() -> unique_ptr<CommandType>`
* CommandReg, Relation between command names and command classes, all commands must be registered in CommandReg
