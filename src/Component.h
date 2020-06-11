#include <iostream>
#include "pongTypes.h"

#include "Collider.h"
#include "Actor.h"

#ifndef COMPONENT
#define COMPONENT


namespace Pong {
	/**
		Component class, an actor has a list of compponents, each component 
		can contain a custom script you only have to inherit from Component.
	*/
	class Component
	{
	public:
		virtual ~Component() {}

		virtual void at_init() {}
		virtual void each_frame() {}
	};


	// collision components
	class CollisionComponent: public Component
	{
	public:
		virtual ~CollisionComponent() { std::cout << "Collision component destructor\n"; }

		Collider* collider = nullptr;
		virtual void at_collision(Collider* other) = 0;
	};


	class BallCollisionComponent : public CollisionComponent
	{
		void at_collision(Collider* other) override;

		virtual ~BallCollisionComponent() { std::cout << "BallCollisionComponent destructor\n"; }
	};

	class BorderCollisionComponent : public CollisionComponent
	{
		void at_collision(Collider* other) override;
		virtual ~BorderCollisionComponent() { std::cout << "BorderCollisionComponent destructor\n"; }
	};

	// actor components
	class ActorComponent
	{
	public:
		virtual ~ActorComponent() 
		{
			std::cout << "Actor component destructor\n";
		}
		Actor* actor = nullptr;
	};
}
#endif // COMPONENT