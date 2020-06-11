#include "pongTypes.h"
#include "Shape.h"
#include "Material.h"
#include <iostream>
#include <vector>
#include <string>

#ifndef ACTOR
#define ACTOR

#include "Component.h"
#include "Collider.h"

namespace Pong {
	class Actor
	{
	protected:
		bool _visible=true;
		Shape* _shape = nullptr;
		Material* _material = nullptr;
		Collider* _collider = nullptr;
		glm::mat4 _transform = glm::mat4(1);
		std::string _name;
		std::list<Component*> _componentList; // list is faster than vector

	public:
		Actor(std::string name): _name(name) {}

		virtual ~Actor();

		virtual void ProcessKeyboard(Movements movement, float deltaTime);

		virtual void draw() const;

		virtual void update(float delta_time){}

		// --setters--
		void setShape(Shape* shape) { _shape = shape; }
		void setTransform(glm::mat4 trans) { _transform = trans; }
		void setMaterial(Material* mat) { _material = mat; }
		void setCollider(Collider* coll);
		void setVisibility(bool vis) { _visible = vis; }
		
		// transforms setters
		void setScale(const glm::vec3 &scale);

		// getters
		Shape* getShape() const { return _shape; }
		glm::mat4 getTransform() const { return _transform; }
		Material* getMaterial() const { return _material; }
		Collider* getCollider() const { return _collider; }
		std::string getName() { return _name; }
		std::list<Component*> get_components() { return _componentList; }
		bool getVisibility() { return _visible; }

		// config members
		template <typename T>
		void addComponent(T* c_ptr = nullptr);
	};

	// --AKinetic class--
	// ------------------
	class AKinetic : public Actor
	{
	protected:
		glm::vec3 _vector_director;  // direction * velocity
		glm::vec3 direction;
		float _velocity = 0;
		float _acceleration = 0;

		float _base_drag = 0.f;
		float _base_speed = 0.f;

	public:
		AKinetic(std::string name): Actor(name){}
		AKinetic(std::string name, glm::vec3 vector_director);

		virtual ~AKinetic() { std::cout << "AKinetic destructor\n"; }

		virtual void update(float delta_time);

		// returns AKinetic vector director
		glm::vec3 getDirection() const;
		glm::vec3 get_vector_director() const { return _vector_director; }

		float getVelocity() const;

		// Set AKinetic vector director
		// normalized vector, only should represent the direction.
		void set_direction(glm::vec3);


		// velocity would be the vector magnitude.
		void setVelocity(float velocity);

	};

	// --APLAYER class--
	class APlayer : public AKinetic
	{
	private:
		bool _key_pressed = false;

	public:
		APlayer(std::string name):AKinetic(name)
		{
			_base_drag = 0.5f;
			_base_speed = 0.1f;
			direction = glm::vec3{ 0.f, 1.f, 0.f };
		}
		virtual ~APlayer() { std::cout << "APlayer destructor\n"; }

		void update(float delta_time) override;

		virtual void ProcessKeyboard(Movements movement, float delta_time) override;
	};

	// --ABALL class--
	class ABall : public AKinetic
	{
	private:

	public:
		ABall(std::string name, glm::vec3 vector_director=glm::vec3(2,0.f,0.f), 
			glm::vec3 start_position=glm::vec3()) : 
			AKinetic(name, vector_director){}

		virtual ~ABall() { std::cout << "ABall destructor\n"; }
	};
}
#endif // ACTOR
