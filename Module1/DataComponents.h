#ifndef GameComponents_hpp
#define GameComponents_hpp
#pragma once

#include <entt/fwd.hpp>
#include "RenderableMesh.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"



struct Transform_Component {
public:
    /*glm::mat4 _world_transform = glm_aux::TRS(
        {0,0,0},
        0.0f, { 0, 1, 0 },
        { 1, 1, 1 });  */  

    glm::mat4 GetTransform() {
        return  glm_aux::T(_position) *
            glm_aux::R(_yaw, _pitch) *
            glm_aux::S(_scale);
    }
    glm::vec3 _position = glm_aux::vec3_000;
    float _yaw = 0;
    float _pitch = 0;
    glm::vec3 _scale = glm_aux::vec3_111;

};





struct LinearVelocity_Component {
    glm::vec3 _velocity = glm_aux::vec3_000;
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


struct Animation_FromSpeed_Component {
    float speed_mult;
    
};

//struct NpcController_Component {
//    uint32_t _seed = 0;
//    float _speed = 0.1f;
//    float _driftSpeed = 0.5f;
//
//    float _breakSpeed = 3.0f;
//    float _breakDampening = 0.3f;
//    
//};

struct SteeringBehavior_Component {
    float _breakSpeed = 4.0f;
    float _breakDampening = 0.2f;

    glm::vec3 _forward = glm_aux::vec3_001;
    glm::vec3 _left = glm_aux::vec3_100;

    glm::vec3 _acceleration = glm_aux::vec3_000;

    glm::vec3 _refPos = glm_aux::vec3_000;

};

struct SB_Wander_Component {
    uint32_t _seed = 0;
    float _speed = 2.0f;
    float _driftSpeed = 0.5f;

    
};

struct SB_Separation_Component {
    float _speed = 1.0f;
    float _distance = 2.0f;
};



struct RotateToVelocity_Component {
    float _lerpValue = 0.5f;
    glm::vec3 _current_direction = glm_aux::vec3_000;
    glm::vec3 _target_direction = glm_aux::vec3_000;
};

struct PointLight_Component {
    // to do
};

struct Camera_Component {
    const float nearPlane = 1.0f;           // Rendering near plane
    const float farPlane = 500.0f;          // Rendering far plane


};


struct UI_ModifyObject_Component {
    std::string text = "gameObject";

    float _worldPos[3];
    float _rotation[2];
    float _scale[3];
};


struct LookAtOrbit_Component {
    
    entt::entity _lookAtTarget;

    glm::vec3 _upVector = glm_aux::vec3_010;
    float _distance = 15.0f;

    //float _yaw = 0.0f;                       // Horizontal angle (radians)
    //float pitch = -glm::pi<float>() / 8;    // Vertical angle (radians)

    glm::ivec2 _mouse_xy_prev{0,0};
    float sensitivity = 0.005f;;
};




#endif