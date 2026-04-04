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
    float speed;

    
};

struct NpcController_Component {
    // to do
};

struct PointLight_Component {
    // to do
};

struct Camera_Component {
    // to do
};


struct LookAt_Component {
    glm::vec3 _lookAtPos = glm_aux::vec3_000;
    glm::vec3 _upVector = glm_aux::vec3_010;
};


#endif