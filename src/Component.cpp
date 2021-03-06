#include "Component.h"
#include <time.h>
#include "Utils.h"

namespace Pong {
	void BallCollisionComponent::at_collision(Collider* other)
	{
		// Get ball actor
		ABall* ball = static_cast<ABall*>(collider->actor);
		//IcosphereShape* ball_shpe = static_cast<IcosphereShape*>(ball->getShape());
		SphereCollider* sphere_coll = static_cast<SphereCollider*>(collider);

		const CollisionData* coll_data = other->get_collision_data();
		// move ball out of collition zone
		glm::vec3 ball_pnt = ball->getTransform()[3];
		
		// add radius data		
		glm::vec3 intrs = glm::vec3(ball_pnt - coll_data->point);

		glm::vec3 radius_vec = coll_data->normal * sphere_coll->getRadius();
		std::cout << "--" << other->getName() << "--\n";
		std::cout << "Normal collision: " << coll_data->normal.x << ", " <<
			coll_data->normal.y << "," << coll_data->normal.z << std::endl;

		std::cout << "Point collision: " << coll_data->point.x << ", " <<
			coll_data->point.y << ", " << coll_data->point.z << std::endl;

		std::cout << "id collision: " << coll_data->face_id << std::endl;

		// add 0.01 normal value, to avoid double collisions
		glm::vec3 ajust_vec = intrs - radius_vec + coll_data->normal * 0.01f;

		// multiply ajust vector by 1.5 to avoid double collisions.
		ball->setTransform(glm::translate(ball->getTransform(), ajust_vec * 1.1f));

		// reflect direction
		glm::vec3 collide_direction = glm::reflect(ball->getDirection(), coll_data->normal);

		if (dynamic_cast<APlayer*>(other->actor))
 		{
			APlayer* other_actor = static_cast<APlayer*>(other->actor);
			collide_direction += other_actor->get_vector_director() * 30.f;  // player affecting ball direction
			ball->setVelocity(ball->getVelocity() + 0.5);
		}

		ball->set_direction(collide_direction);
	}

	// BorderCollisionComponent
	// ------------------------
	void BorderCollisionComponent::at_collision(Collider* other)
	{
		ABall* ball = dynamic_cast<ABall*>(other->getActor());

		if (ball)
		{
			glm::mat4 b_trnsform = ball->getTransform();
			b_trnsform[3] = glm::vec4(0, 0, 0, 1);
			ball->setTransform(b_trnsform);
			cout_matrix(b_trnsform);
			// set a random direction
			srand(time(NULL));
			const double pi = 3.14159265358979323846;
			float rand_angle =  2.f * pi * (rand() % 100) / 100.f;

			float sin_val = sin(rand_angle);
			float cos_val = cos(rand_angle);

			ball->set_direction(glm::vec3(cos_val, sin_val, 0));
			ball->setVelocity(2.f);
		}
	}
}