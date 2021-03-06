#include "pongTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <vector>

#ifndef COLLIDER
#define COLLIDER

#include "Component.h"
#include "Actor.h"


namespace Pong {
	// declare classes
	class BoxCollider; class SphereCollider;
	class PlaneCollider; class MeshCollider;

	template<typename t>
	bool startCollision(Collider* myobj, Collider* other);
	bool checkAABB(const Collider*, const Collider*);

	glm::vec3 ray_on_plane(glm::vec3 plane_normal, glm::vec3 plane_point,
		glm::vec3 ray_direction, glm::vec3 ray_origin);

	struct CollisionData
	{
		glm::vec3 point;
		int face_id;
		glm::vec3 normal;
		std::chrono::system_clock::time_point time;
	};


	struct RayCastData
	{
		glm::vec3 point;
		glm::vec3 normal;
		int face_id;
		Collider* collider;  // const collider
	};

	class RayCast
	{
		//float _max_size;
	public:
		glm::vec3 direction;
		glm::vec3 position;

		RayCast(glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 position = glm::vec3(0, 0, 0)) :
			direction(direction), position(position) {}

		/**
			Returns a vector sorted by nearest from ray origin with all the RayCastData
			of the touched colliders.
		*/
		//std::vector<RayCastData> cast(Collider* collider_array, int size) const;

	private:
	};

	void sort_raycast_data(std::vector<RayCastData> &data_vector, glm::vec3 point, int begin, int end);

	bool compare_raycast_data(const RayCastData &a, const RayCastData &b, const glm::vec3 &point);

	// abstract collider class
	class Collider
	{
	public:
		// check collider funtions
		friend bool checkCollision(const BoxCollider*, const BoxCollider*);
		friend bool checkCollision(const BoxCollider*, const PlaneCollider*);
		friend bool checkCollision(const PlaneCollider*, const BoxCollider*);
		friend bool checkCollision(const BoxCollider*, const MeshCollider*);
		friend bool checkCollision(const MeshCollider*, const BoxCollider*);
		friend bool checkCollision(const BoxCollider*, const SphereCollider*);
		friend bool checkCollision(const SphereCollider*, const BoxCollider*);
		friend bool checkCollision(const SphereCollider*, const SphereCollider*);
		friend bool checkCollision(const SphereCollider*, const PlaneCollider*);
		friend bool checkCollision(const PlaneCollider*, const SphereCollider*);
		friend bool checkCollision(const MeshCollider*, const SphereCollider*);
		friend bool checkCollision(const SphereCollider*, const MeshCollider*);

	protected:		
		float _velocity;
		glm::vec3 direction;

		CollisionData* _collision_data = nullptr; // save last collision data

		std::string _name;

	public:
		Collider(std::string name);
		virtual ~Collider();

		const CollisionData* get_collision_data() const { return _collision_data; }

		// component list, at collision all components are excecuted
		std::list<CollisionComponent*> component_list;
		std::list<CollisionComponent*> get_components() { return component_list; }

		Actor* actor;

		// --methods--
		std::string getName() const { return _name; }

		virtual bool collide(BoxCollider* other) = 0;
		virtual bool collide(SphereCollider* other) = 0;
		virtual bool collide(PlaneCollider* other) = 0;
		virtual bool collide(MeshCollider* other) = 0;
		virtual bool collide(Collider* other) = 0;

		/**
		It is a vector because some shapes has front tho sides. like box, sphere, etc*/
		virtual bool ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const = 0 ;

		// virtual bool on_collision(Collider* other) {// run collission components}

		virtual glm::vec3* getAABB() const = 0;

		Actor* getActor();

		/**
		This method creates a CollisionComponent object and save it in component_list.
		class destructor will delete pointers at the end.*/
		template<typename T>
		T* add_component()
		{
			if (!std::is_base_of<CollisionComponent, T>::value) 
			{
				return nullptr;
			}
			T* component = new T;
			component_list.push_back(component);
			component->collider = this;
			return component;
		}
	};


	class BoxCollider: public Collider
	{
	private:
		float _width;
		float _height;
		float _depth;

		glm::vec3 direction;

		void _setOBB_buffer(float width, float height, float depth);

	public:
		/* constructor
		@param width collider width
		@param height collider height
		@param depth collider depth*/
		BoxCollider(std::string name, float width = 1.f, float height = 1.f, float depth = 1.f);

		bool collide(BoxCollider* other) override;
		bool collide(SphereCollider* other) override;
		bool collide(PlaneCollider* other) override;
		bool collide(MeshCollider* other) override;
		bool collide(Collider* other) override;

		virtual bool ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const override;

		glm::vec3 OBB_buffer[8];

		glm::vec3* getAABB() const override;

		void setBoxParameters(float width = NULL, float height = NULL, float depth = NULL);

		static const int FACES[6][4];
	};


	class SphereCollider : public Collider
	{
	private:
		float _radius;
		glm::vec3 position;

	public:
		SphereCollider(std::string name, float radius = 1.f);
		virtual ~SphereCollider();

		bool collide(BoxCollider* other) override;
		bool collide(SphereCollider* other) override;
		bool collide(PlaneCollider* other) override;
		bool collide(MeshCollider* other) override;
		bool collide(Collider* other) override;

		virtual bool ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const override;


		void setRadius(float r) { _radius = r; }
		float getRadius() const;

		glm::vec3* getAABB() const override;
	};

	// infinity area collider
	class PlaneCollider : public Collider
	{
	private:
		glm::vec3 _point;
		glm::vec3 _normal;

	public:
		PlaneCollider(std::string name) : Collider(name) {}
		virtual ~PlaneCollider() {}
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(std::string name) : Collider(name) {}
		~CapsuleCollider() {};
	};

	class MeshCollider : public Collider
	{
	private:
		unsigned int _VAO;
		// shape
		Shape* _shape = nullptr;

	public:
		MeshCollider(std::string name, Shape* _shape): Collider(name) {}
		virtual ~MeshCollider(); // {delete[] _buffer}

	};

}
#endif // COLLIDER