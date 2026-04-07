#ifndef GameComponents_hpp
#define GameComponents_hpp
#pragma once

#include <entt/fwd.hpp>
#include "RenderableMesh.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"



struct Transform_Component {
public:
    glm::mat4 _world_transform = glm_aux::TRS(
        {0,0,0},
        0.0f, { 0, 1, 0 },
        { 1, 1, 1 });    
};





struct LinearVelocity_Component {
    glm::vec3 _velocity = glm_aux::vec3_000;
};

struct RenderableMesh_Component {
    std::shared_ptr<eeng::RenderableMesh> _renderable_mesh;
};

struct  AABB_Component {
    eeng::AABB _AABB;
};


struct PlayerController_Component {
    entt::entity _rotationRefrence;
    
    float speed;

    float movement_lerp = 0.5f;
    glm::vec3 target_movement = glm_aux::vec3_000;
    glm::vec3 current_movement = glm_aux::vec3_000;
};

struct NpcController_Component {
    // to do
};

struct PointLight_Component {
    // to do
};

struct Camera_Component {
    const float nearPlane = 1.0f;           // Rendering near plane
    const float farPlane = 500.0f;          // Rendering far plane


};


struct LookAtOrbit_Component {
    
    entt::entity _lookAtTarget;

    glm::vec3 _upVector = glm_aux::vec3_010;
    float _distance = 15.0f;

    float yaw = 0.0f;                       // Horizontal angle (radians)
    float pitch = -glm::pi<float>() / 8;    // Vertical angle (radians)
};




#endif