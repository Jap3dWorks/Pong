#ifndef COLLIDER_H
#define COLLIDER_H

//#include "pongTypes.h"
namespace Pong
{
    class Actor;
    class CollisionComponent;
    class Collider;
}

#include "Pong/core/actor.h"
#include "Pong/core/shape.h"
#include "Pong/core/component.h"
#include "Pong/core/core_vals.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <vector>

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
        Collider* collider;  // const Collider
    };

    class RayCast {
        //float _max_size;
    public:
        glm::vec3 direction;
        glm::vec3 position;

        explicit RayCast(glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 position = glm::vec3(0, 0, 0)) :
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

    // abstract Collider class
    class Collider
    {
    public:
        // check Collider funtions
        friend bool checkCollision(const BoxCollider*, const BoxCollider*);
        friend bool checkCollision(const BoxCollider*, const PlaneCollider*);
        friend bool checkCollision(const PlaneCollider*, const BoxCollider*) {}
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

        // Component list, at collision all components are excecuted
        std::list<CollisionComponent*> component_list;
        std::list<CollisionComponent*> get_components() { return component_list; }

        Actor* actor;

        // --methods--
        _P_NODISCARD std::string getName() const { return _name; }

        virtual bool collide(BoxCollider* other) = 0;
        virtual bool collide(SphereCollider* other) = 0;
        virtual bool collide(PlaneCollider* other) = 0;
        virtual bool collide(MeshCollider* other) = 0;
        virtual bool collide(Collider* other) = 0;

        /**
        It is a vector because some _shapes has front tho sides. like box, sphere, etc*/
        virtual bool ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const = 0 ;

        // virtual bool on_collision(Collider* other) {// run collission components}

        virtual glm::vec3* getAABB() const = 0;

        Actor* getActor();

        /**
        Stores CollisionComponent object in Collider data.*/
        void add_component(CollisionComponent* component);
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
        @param width Collider width
        @param height Collider height
        @param depth Collider depth*/
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

    // infinity area Collider
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
        // Shape
        Shape* _shape = nullptr;

    public:
        MeshCollider(std::string name, Shape* _shape): Collider(name) {}
        virtual ~MeshCollider(); // {delete[] _buffer}

    };

}
#endif // COLLIDER_H