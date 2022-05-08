#ifndef P_ACTION_H
#define P_ACTION_H

namespace Pong {
	/**
		The idea of this class is the posibility of program Movements or actions for an Actor.
		this Actor maybe should loose the inputs control.
	*/
	class Action
	{
	public:
		bool active = true;  // Component to know if the action has finished
		
		virtual void derived() = 0;

		void run() {
			derived();
			active = false;
		}
		
		void reload(){active = true;}
	};
}
#endif // !P_ACTION_H
