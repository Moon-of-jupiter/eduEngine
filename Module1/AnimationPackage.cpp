
#include <entt/entt.hpp>
#include "glmcommon.hpp"
#include "imgui.h"
#include "Log.hpp"
#include "DataComponents.h"
#include "RenderableMesh.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"



#include "AnimationPackage.h"

void Animation_Systems::render_System(std::shared_ptr<entt::registry> entity_registry, eeng::ForwardRendererPtr forwardRenderer) {
    auto view = entity_registry->view<Transform_Component, RenderableMesh_Component>();
    auto animated_view = entity_registry->view<Animation_Component>();
    for (auto entity : view) {

        auto& transform = view.get<Transform_Component>(entity);
        auto& mesh = view.get<RenderableMesh_Component>(entity);

        if (entity_registry->all_of<Animation_Component>(entity)) {
            auto& animation = animated_view.get<Animation_Component>(entity);

            if (animation.useLayering) {
                mesh._renderable_mesh->animateBlend(
                    animation.baseAnimation,
                    animation.secondaryAnimation,
                    animation.time0,
                    animation.time1,
                    animation.filter
                );
            }
            else {
                mesh._renderable_mesh->animateBlend(
                    animation.baseAnimation,
                    animation.secondaryAnimation,
                    animation.time0,
                    animation.time1,
                    animation.blendFactor
                );
            }
        }
        else {
            mesh._renderable_mesh->animate(0, 0);
        }

        forwardRenderer->renderMesh(mesh._renderable_mesh, transform.GetTransform());


    }


}



void Animation_Systems::animation_Basic_System(std::shared_ptr<entt::registry> entity_registry, float dT) {   
    auto view = entity_registry->view<Animation_Component,Animation_Basic_Component>();

    for (auto entity : view) {
        auto& animation = view.get<Animation_Component>(entity);
        auto& animation_updator = view.get<Animation_Basic_Component>(entity);

        animation.time0 += animation_updator.speedMult0 * dT;
        animation.time1 += animation_updator.speedMult1 * dT;
    }

}

void Animation_Systems::animation_1DBlendSpace_System(std::shared_ptr<entt::registry> entity_registry, float dT) {
    auto view = entity_registry->view<Animation_Component, Animation_1dBlendSpace_Component>();

    for (auto entity : view) {
        auto& animation = view.get<Animation_Component>(entity);
        auto& animation_updator = view.get<Animation_1dBlendSpace_Component>(entity);

        for (auto& animationState : animation_updator.blendSpaceElements) {
            animationState.time += dT * animationState.speedMult;
            
        }



        float value = animation_updator.value;

        auto animationA = animation_updator.blendSpaceElements.back();
        auto animationB = animationA;

        for (auto& animationState : animation_updator.blendSpaceElements) {
            animationB = animationState;
            if (animationState.value >= value) {
                break;
            }
            animationA = animationState;
        }



        float blend = (value - animationA.value) / (animationB.value - animationA.value);

        animation.time0 = animationA.time;
        animation.time1 = animationB.time;

        animation.baseAnimation = animationA.animation;
        animation.secondaryAnimation = animationB.animation;

        animation.blendFactor = blend;
        animation.useLayering = false;
    }

}




void Animation_Systems::debug_mesh_bones_System(std::shared_ptr<entt::registry> entity_registry, ShapeRendererPtr shapeRenderer) {

    auto view = entity_registry->view<Transform_Component, RenderableMesh_Component>();
    auto animated_view = entity_registry->view<Animation_Component>();


    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& mesh = view.get<RenderableMesh_Component>(entity);
        
        if (entity_registry->all_of<Animation_Component>(entity)) {
            auto& animation = animated_view.get<Animation_Component>(entity);

            if (animation.useLayering) {
                mesh._renderable_mesh->animateBlend(
                    animation.baseAnimation,
                    animation.secondaryAnimation,
                    animation.time0,
                    animation.time1,
                    animation.filter
                );
            }
            else {
                mesh._renderable_mesh->animateBlend(
                    animation.baseAnimation,
                    animation.secondaryAnimation,
                    animation.time0,
                    animation.time1,
                    animation.blendFactor
                );
            }
        }
        else {
            mesh._renderable_mesh->animate(0, 0);
        }

        
        AnimationHelpers::drawMeshBones_Debug(mesh._renderable_mesh, transform.GetTransform(), shapeRenderer, 0.02f);

    }
    


}

void AnimationHelpers::drawMeshBones_Debug(const std::shared_ptr<eeng::RenderableMesh> mesh, const glm::mat4& localToWorld, ShapeRendererPtr shapeRenderer, const float boneScale) {

    


    float axisLen = 25.0f;
   

    for (int i = 0; i < mesh->boneMatrices.size(); i++) {

        auto IBinverse = glm::inverse(mesh->m_bones[i].inversebind_tfm);
        glm::mat4 global = localToWorld * mesh->boneMatrices[i] * IBinverse;

        glm::vec3 pos = global[3];

        

        glm::vec3 right = glm::normalize(glm::vec3(global[0])) * boneScale; // X
        glm::vec3 up    = glm::normalize(glm::vec3(global[1])) * boneScale; // Y
        glm::vec3 fwd   = glm::normalize(glm::vec3(global[2])) * boneScale; // Z

        shapeRenderer->push_states(ShapeRendering::Color4u::Red);
        shapeRenderer->push_line(pos, pos + axisLen * right);

        shapeRenderer->push_states(ShapeRendering::Color4u::Green);
        shapeRenderer->push_line(pos, pos + axisLen * up);

        shapeRenderer->push_states(ShapeRendering::Color4u::Blue);
        shapeRenderer->push_line(pos, pos + axisLen * fwd);

        shapeRenderer->pop_states<ShapeRendering::Color4u>();
        shapeRenderer->pop_states<ShapeRendering::Color4u>();
        shapeRenderer->pop_states<ShapeRendering::Color4u>();
    }


}


