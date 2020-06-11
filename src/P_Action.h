
#ifndef PACTION
#define PACTION

namespace Pong {
	/**
		The idea of this class is the posibility of program movements or actions for an actor.
		this actor maybe should loose the inputs control.  
	*/
	class Action
	{
	public:
		bool active = true;  // component to know if the action has finished
		
		virtual void derived() = 0;

		void run() {
			derived();
			active = false;
		}
		
		void reload(){active = true;}
	};
}
#endif // !PACTION
