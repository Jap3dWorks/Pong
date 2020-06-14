#include "Collider.h"
#include "logger.h"

#include <limits>
#include <algorithm>


namespace Pong {

    glm::vec3 ray_on_plane(glm::vec3 plane_normal, glm::vec3 plane_point,
        glm::vec3 ray_direction, glm::vec3 ray_origin)
    {
        ray_direction = glm::normalize(ray_direction);

        float perpendicularity = glm::dot(plane_normal, ray_direction);
        if(perpendicularity == 0.f) {
            return glm::vec3(0);
        }

        float t = glm::dot(plane_normal, plane_point - ray_origin) / perpendicularity;

        // p + tV
        glm::vec3 pnt = ray_origin + t * ray_direction;

        return pnt;
    }


    // --check collision functions--
    // -----------------------------
    template<typename T>
    bool startCollision(Collider* myobj, Collider* other)
    {
        T* obj = static_cast<T*>(myobj);
        return other->collide(obj);
    }

    bool checkAABB(const Collider* cA, const Collider* cB)
    {
        glm::vec3* A_bbox = cA->getAABB();
        glm::vec3* B_bbox = cB->getAABB();

        // compare AABB to check collission
        if ((A_bbox[0].x <= B_bbox[1].x && A_bbox[1].x >= B_bbox[0].x) &&
            (A_bbox[0].y <= B_bbox[1].y && A_bbox[1].y >= B_bbox[0].y) &&
            (A_bbox[0].z <= B_bbox[1].z && A_bbox[1].z >= B_bbox[0].z))
        {
            delete[] A_bbox;
            delete[] B_bbox;
            return true;
        }
        delete[] A_bbox;
        delete[] B_bbox;
        return false;
    }

    // BoxCollider BoxCollider
    bool checkCollision(const BoxCollider* cA, const BoxCollider* cB)
    {
        if (!checkAABB(cA, cB))
            return false;

        // transform buffers
        glm::vec3 buff[2][8];
        for (int i = 0; i < 8; i++) {
            buff[0][i] = cA->actor->getTransform() * glm::vec4(cA->OBB_buffer[i], 1);
            buff[1][i] = cB->actor->getTransform() * glm::vec4(cB->OBB_buffer[i], 1);
        }

        // SAT collision
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 6; i++)
            {
                glm::vec3 pnt = (buff[j][BoxCollider::FACES[i][0]] + buff[j][BoxCollider::FACES[i][1]] +
                    buff[j][BoxCollider::FACES[i][2]] + buff[j][BoxCollider::FACES[i][3]]) / 4.f;
                glm::vec3 norm = glm::normalize(glm::cross(buff[j][BoxCollider::FACES[i][0]] - pnt,
                    buff[j][BoxCollider::FACES[i][1]] - pnt));

                // check points
                int frontV = 0;
                for (int k = 0; k < 8; k++) {
                    float dot = glm::dot(buff[1 - j][k] - pnt, norm);
                    if (dot >= 0)
                        frontV++;
                }
                if (frontV == 8)
                    return false;
            }
        }
        return true;
    }

    // boxCollider planeCollider
    bool checkCollision(const BoxCollider* cA, const PlaneCollider* pB) { return false; }
    bool checkCollisoon(const PlaneCollider* pA, const BoxCollider* cB) { return checkCollision(cB, pA); }

    // boxCollider meshCollider
    bool checkCollision(const BoxCollider* bA, const MeshCollider* mB) { return false; }
    bool checkCollision(const MeshCollider* mA, const BoxCollider* bB) { return checkCollision(bB, mA); }

    // SphereCollider BoxCollider
    bool checkCollision(const BoxCollider* cb, const SphereCollider* cs)
    {
        return checkCollision(cs, cb);
    }

    bool checkCollision(const SphereCollider* cs, const BoxCollider* cb)
    {
        if (!checkAABB(cs, cb)){
            return false;
        }

        // transform buffer
        glm::vec3 buff[8];
        for (int i = 0; i < 8; i++) {
            buff[i] = cb->actor->getTransform() * glm::vec4(cb->OBB_buffer[i], 1);
        }

        float collision_distance = std::numeric_limits<float>::max();
        CollisionData coll_dat;

        // SAT collision
        for (int i = 0; i < 6; i++)
        {
            // define a plane
            glm::vec3 pnt = (buff[BoxCollider::FACES[i][0]] + buff[BoxCollider::FACES[i][1]] +
                buff[BoxCollider::FACES[i][2]] + buff[BoxCollider::FACES[i][3]]) / 4.f;
            glm::vec3 norm = glm::normalize(glm::cross(buff[BoxCollider::FACES[i][0]] - pnt,
                buff[BoxCollider::FACES[i][1]] - pnt));

            // dot with plane normal should be lower than radius for collision
            glm::vec3 sph_translate = glm::vec3(cs->actor->getTransform()[3]);
            glm::vec3 sVec = sph_translate - pnt;
            float dot_val = glm::dot(norm, sVec);
            float dist_coll = dot_val - cs->getRadius();
            if (dist_coll > 0.f)
                {return false;}

            // save collision data
            if (collision_distance > abs(dist_coll))
            {
                collision_distance = abs(dist_coll);
                coll_dat.face_id = i;
                coll_dat.normal = norm;
                coll_dat.point = sph_translate + norm * (-dot_val);
                coll_dat.time = std::chrono::system_clock::now();
            }
        }

        // fill collision data
        CollisionData* sp_cdata = cs->_collision_data;
        CollisionData* bx_cdata = cb->_collision_data;

        for (auto &c_d : { sp_cdata, bx_cdata })
        {	c_d->point = coll_dat.point;
            c_d->time = coll_dat.time;}

        bx_cdata->face_id = coll_dat.face_id;
        bx_cdata->normal = coll_dat.normal;
        sp_cdata->normal = -coll_dat.normal;

        return true;
    }

    // sphere sphere
    bool checkCollision(const SphereCollider* scA, const SphereCollider* scB)
    {
        glm::vec3 pnA = scA->actor->getTransform()[3];
        glm::vec3 pnB = scB->actor->getTransform()[3];

        glm::vec3 AvB = pnB - pnA;

        float coll_distance = scA->getRadius() + scB->getRadius();
        if (coll_distance >= AvB.length())
            return true;

        return false;
    }

    //sphere plane
    bool checkCollision(const SphereCollider* scA, const PlaneCollider* pcB) { return false; }
    bool checkCollision(const PlaneCollider* pcB, const SphereCollider* scA)
    {
        return checkCollision(scA, pcB);
    }

    // sphere mesh
    bool checkCollision(const SphereCollider* scA, const MeshCollider* mcB) { return false; }
    bool checkCollision(const MeshCollider* mcA, const SphereCollider* scB)
    {
        return checkCollision(scB, mcA);
    }

    // --Collider--
    // ------------
    Collider::Collider(std::string name) : _name(name)
    {
        _collision_data = new CollisionData;
    }
    Collider::~Collider()
    {
        // Remove component ptrs
//        for (const auto &c : component_list)
//        {
//            delete c;
//        }
        delete _collision_data;
    }

    Actor* Collider::getActor() { return actor; }


    // --box collider--
    // ----------------
    const int BoxCollider::FACES[6][4] = { { 0,1,2,3 },
                                            {4,5,6,7},
                                            {0,4,7,1},
                                            {1,7,6,2},
                                            {2,6,5,3},
                                            {0,3,5,4} };

    BoxCollider::BoxCollider(std::string name, float width, float height, float depth):
        _width(width), _height(height), _depth(depth), Collider(name)
    {
        _setOBB_buffer(_width, _height, _depth);
    }

    void BoxCollider::_setOBB_buffer(float width, float height, float depth)
    {
        // should be sync with FACES
        // top floor
        OBB_buffer[0] = glm::vec3(width / 2, height / 2, depth / 2);
        OBB_buffer[1] = glm::vec3(width / 2, height / 2, - depth / 2);
        OBB_buffer[2] = glm::vec3(- width / 2, height / 2, - depth / 2);
        OBB_buffer[3] = glm::vec3(- width / 2, height / 2, depth / 2);
        // bottom floor
        OBB_buffer[4] = glm::vec3(width / 2, -height / 2, depth / 2);
        OBB_buffer[5] = glm::vec3(-width / 2, -height / 2, depth / 2);
        OBB_buffer[6] = glm::vec3(-width / 2, -height / 2, -depth / 2);
        OBB_buffer[7] = glm::vec3(width / 2, -height / 2, -depth / 2);
    }

    void BoxCollider::setBoxParameters(float width, float height, float depth )
    {
        if (width != NULL)
            _width = width;
        if (height != NULL)
            _height = height;
        if (depth != NULL)
            _depth = depth;

        // update buffer
        _setOBB_buffer(_width, _height, _depth);
    }

    glm::vec3* BoxCollider::getAABB() const
    {
        float xmin = std::numeric_limits<float>::max();
        float ymin = std::numeric_limits<float>::max();
        float zmin = std::numeric_limits<float>::max();
        float xmax = -std::numeric_limits<float>::max();
        float ymax = -std::numeric_limits<float>::max();
        float zmax = -std::numeric_limits<float>::max();

        for (int i = 0; i < 8; i++)
        {
            glm::vec3 pTrans = glm::vec3(actor->getTransform() * glm::vec4(OBB_buffer[i], 1));
            // compare min
            xmin = std::min<float>(xmin, pTrans.x);
            ymin = std::min<float>(ymin, pTrans.y);
            zmin = std::min<float>(zmin, pTrans.z);
            // compare max

            xmax = std::max<float>(xmax, pTrans.x);
            ymax = std::max<float>(ymax, pTrans.y);
            zmax = std::max<float>(zmax, pTrans.z);
        }

        glm::vec3* result = new glm::vec3[2];

        result[0] = glm::vec3(xmin, ymin, zmin);
        result[1] = glm::vec3(xmax, ymax, zmax);

        return result;
    }
    // collision methods
    bool BoxCollider::collide(BoxCollider* other) { return checkCollision(this, other); }
    bool BoxCollider::collide(SphereCollider* other) { return checkCollision(this, other); }
    bool BoxCollider::collide(PlaneCollider* other) { return checkCollision(this, other); }
    bool BoxCollider::collide(MeshCollider* other) { return checkCollision(this, other); }
    bool BoxCollider::collide(Collider* other)
    {
        return startCollision<BoxCollider>(this, other);
    }

    // raycast
    bool BoxCollider::ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const
    {
        bool check_ray = false;

        // do not transform buffer, transform ray instead
        glm::vec3 r_dir = glm::inverse(actor->getTransform()) * glm::vec4(ray.direction,0.f);
        glm::vec3 r_pos = glm::inverse(actor->getTransform()) * glm::vec4(ray.position,1.f);

        /*cout_vector(r_dir);
        cout_vector(r_pos);
        std::cout << std::endl;*/

        for (int i = 0; i < 6; i++)
        {
            // define a plane
            glm::vec3 pnt = (OBB_buffer[BoxCollider::FACES[i][0]] + OBB_buffer[BoxCollider::FACES[i][1]] +
                OBB_buffer[BoxCollider::FACES[i][2]] + OBB_buffer[BoxCollider::FACES[i][3]]) / 4.f;
            glm::vec3 norm = glm::normalize(glm::cross(OBB_buffer[BoxCollider::FACES[i][0]] - pnt,
                OBB_buffer[BoxCollider::FACES[i][1]] - pnt));

            //cout_vector(pnt);
            //cout_vector(norm);

            // get point on plane
            glm::vec3 pln_pnt = ray_on_plane(norm, pnt, r_dir, r_pos);

            //cout_vector(pln_pnt);
            if (pln_pnt == glm::vec3(0.f, 0.f, 0.f)){
                continue;
            }

            // look if ray is inside the plane
            bool check = false;
            // face posibilities TODO.face combinations
            if (i == 0){
                check = (pln_pnt.x >= OBB_buffer[3].x &&
                    pln_pnt.x <= OBB_buffer[0].x &&
                    pln_pnt.z >= OBB_buffer[1].z &&
                    pln_pnt.z <= OBB_buffer[0].z);
            }
            else if (i == 1) {
                check = (pln_pnt.x >= OBB_buffer[5].x &&
                    pln_pnt.x <= OBB_buffer[4].x &&
                    pln_pnt.z >= OBB_buffer[7].z &&
                    pln_pnt.z <= OBB_buffer[4].z);
            }
            else if (i == 2) {
                check = (pln_pnt.z >= OBB_buffer[1].z &&
                    pln_pnt.z <= OBB_buffer[0].z &&
                    pln_pnt.y >= OBB_buffer[4].y &&
                    pln_pnt.y <= OBB_buffer[0].y);
            }
            else if (i == 3) {
                check = (pln_pnt.x >= OBB_buffer[2].x &&
                    pln_pnt.x <= OBB_buffer[1].x &&
                    pln_pnt.y >= OBB_buffer[7].y &&
                    pln_pnt.y <= OBB_buffer[1].y);
            }
            else if (i == 4) {
                check = (pln_pnt.z >= OBB_buffer[2].z &&
                    pln_pnt.z <= OBB_buffer[3].z &&
                    pln_pnt.y >= OBB_buffer[5].y &&
                    pln_pnt.y <= OBB_buffer[3].y);
            }
            else if (i == 5) {
                check = (pln_pnt.x >= OBB_buffer[3].x &&
                    pln_pnt.x <= OBB_buffer[0].x &&
                    pln_pnt.y >= OBB_buffer[5].y &&
                    pln_pnt.y <= OBB_buffer[3].y);
            }

            // if check is true save collision
            //std::cout << check << std::endl;
            if(check){
                check_ray = true;
                glm::mat3 normal_mat =
                    glm::transpose(glm::inverse(glm::mat3(actor->getTransform())));

                RayCastData r_cast{glm::vec3(actor->getTransform() * glm::vec4(pln_pnt, 1)),
                    glm::normalize(normal_mat * norm),  // transform normal
                    i,
                    const_cast<BoxCollider*>(this)};

                ray_data.push_back(r_cast);
            }
        }
        return check_ray;
    }

    // --sphere collider--
    // -------------------
    bool SphereCollider::ray_cast(const RayCast ray, std::vector<RayCastData> &ray_data) const
    {
        // do not transform buffer, transform ray instead
        glm::vec3 r_dir = glm::inverse(actor->getTransform()) * glm::vec4(ray.direction, 0.f);
        glm::vec3 r_pos = glm::inverse(actor->getTransform()) * glm::vec4(ray.position, 1.f);

        // (-b +-root(b^2 - 4ac)) / 2a
        float b = 2 * glm::dot(r_pos, r_dir);
        float a = glm::dot(r_dir, r_dir);
        float c = glm::dot(r_pos, r_pos) - (_radius * _radius);

        float D = b * b - 4 * a *c;

        if (D < 0)
        {
            return false;
        }

        float t = (-b - sqrtf(D)) / 2 * a;
        glm::vec3 pnt = glm::vec3(actor->getTransform() * glm::vec4(r_pos + t * r_dir, 1));
        RayCastData rc_data{
            pnt,
            glm::normalize(pnt - glm::vec3(actor->getTransform()[3])),
            0,
            const_cast<SphereCollider*>(this) };

        ray_data.push_back(rc_data);

        if (D > 0)
        {
            float t = (-b + sqrtf(D)) / 2 * a;
            pnt = glm::vec3(actor->getTransform() * glm::vec4(r_pos + t * r_dir, 1));
            RayCastData rc_data_sec{
                pnt,
                glm::normalize(pnt - glm::vec3(actor->getTransform()[3])),
                0,
                const_cast<SphereCollider*>(this) };

            ray_data.push_back(rc_data_sec);
        }

        return true;
    }

    SphereCollider::SphereCollider(std::string name, float radius) :
        _radius(radius), Collider(name) {}

    SphereCollider::~SphereCollider() {}

    glm::vec3* SphereCollider::getAABB() const
    {
        glm::vec3* result = new glm::vec3[2];
        result[0] = actor->getTransform() * glm::vec4(-_radius, -_radius, -_radius, 1);
        result[1] = actor->getTransform() * glm::vec4(_radius, _radius, _radius, 1);

        return result;
    }

    float SphereCollider::getRadius() const
    {
        return _radius;
    }

    // collision methods
    bool SphereCollider::collide(BoxCollider* other) { return checkCollision(this, other); }
    bool SphereCollider::collide(SphereCollider* other) { return checkCollision(this, other); }
    bool SphereCollider::collide(PlaneCollider* other) { return checkCollision(this, other); }
    bool SphereCollider::collide(MeshCollider* other) { return checkCollision(this, other); }
    bool SphereCollider::collide(Collider* other)
    {
        return startCollision<SphereCollider>(this, other);
    }

    // --Plane collider--


    // --ray cast funtions --

    bool compare_raycast_data(const RayCastData &a, const RayCastData &b, const glm::vec3 &point)
    {
        return glm::length(a.point - point) < glm::length(b.point - point);
    }

    void sort_raycast_data(std::vector<RayCastData> &data_vector,
        glm::vec3 point,
        int begin, int end)
    {
        int range = end - begin;
        if (range < 1)
        {
            return;
        }

        LOG_DEBUG( "sort range: " << begin << ", " << end);

        int begin_end = begin + (range / 2);
        int end_start = begin + (range / 2) + 1;

        sort_raycast_data(data_vector, point, begin, begin_end);
        sort_raycast_data(data_vector, point, end_start, end);

        // compare ranges
        int b = 0;
        int e = 0;
        for (int i=0; i < range; i++)
        {
            if (compare_raycast_data(data_vector[begin + i],
                data_vector[end_start + e],
                point))
            {
                b++;
            }
            else
            {
                RayCastData tmp_dt = data_vector[end_start + e];
                data_vector.erase(data_vector.begin() + end_start + e);
                data_vector.insert(data_vector.begin() +  begin + i, tmp_dt);
                /*
                std::move(data_vector.begin() + end_start + e,
                    data_vector.begin() + end_start + e + 1,
                    data_vector.begin() + begin + i);
                std::cout << "sort index: " << end_start + e <<
                    ", " << begin + i << std::endl;*/
                e++;
            }

            if (b >= begin - begin_end || e >= end_start - end)
            {
                break;
            }

        }
    }
}