
#include <entt/entt.hpp>
#include "glmcommon.hpp"

#include "imgui.h"
#include "Log.hpp"
#include "Game.hpp"
#include "DataComponents.h"
#include "AnimationPackage.h"
#include "CollisionPackage.h"
#include "EventPackage.h"
#include "MeshManager.h"

bool Game::init()
{

    forwardRenderer = std::make_shared<eeng::ForwardRenderer>();
    forwardRenderer->init("shaders/phong_vert.glsl", "shaders/phong_frag.glsl");

    shapeRenderer = std::make_shared<ShapeRendering::ShapeRenderer>();
    shapeRenderer->init();

    // Do some entt stuff
    entity_registry = std::make_shared<entt::registry>();
    
    meshManager = std::make_shared<my_eeng::MeshManager>();

    meshManager->Load("grass_trees_merged_001", "assets/grass/grass_trees_merged.fbx");
    
    meshManager->Load("horse_001", "assets/Animals/Horse.fbx", {});
   
    meshManager->Load("character_001", "assets/Amy/Ch46_nonPBR.fbx",  { 
        "assets/Amy/idle.fbx",
        "assets/Amy/walking.fbx",
        "assets/Amy/running.fbx",
        "assets/Amy/waving.fbx",
    });
    meshManager->RemoveTranslationKeys("character_001", "mixamorig:Hips");


#if 0
    // Character
    characterMesh->load("assets/Ultimate Platformer Pack/Character/Character.fbx", false);
#endif
#if 0
    // Enemy
    characterMesh->load("assets/Ultimate Platformer Pack/Enemies/Bee.fbx", false);
#endif
#if 0
    // ExoRed 5.0.1 PACK FBX, 60fps, No keyframe reduction
    characterMesh->load("assets/ExoRed/exo_red.fbx");
    characterMesh->load("assets/ExoRed/idle (2).fbx", true);
    characterMesh->load("assets/ExoRed/walking.fbx", true);
    // Remove root motion
    characterMesh->removeTranslationKeys("mixamorig:Hips");
#endif
#if 0
    // Amy 5.0.1 PACK FBX
    characterMesh->load("assets/Amy/Ch46_nonPBR.fbx");
    characterMesh->load("assets/Amy/idle.fbx", true);
    characterMesh->load("assets/Amy/walking.fbx", true);
    characterMesh->load("assets/Amy/running.fbx", true);
    characterMesh->load("assets/Amy/waving.fbx", true);
    // Remove root motion
    characterMesh->removeTranslationKeys("mixamorig:Hips");
#endif
#if 0
    // Eve 5.0.1 PACK FBX
    // Fix for assimp 5.0.1 (https://github.com/assimp/assimp/issues/4486)
    // FBXConverter.cpp, line 648: 
    //      const float zero_epsilon = 1e-6f; => const float zero_epsilon = Math::getEpsilon<float>();
    characterMesh->load("assets/Eve/Eve By J.Gonzales.fbx");
    characterMesh->load("assets/Eve/idle.fbx", true);
    characterMesh->load("assets/Eve/walking.fbx", true);
    // Remove root motion
    characterMesh->removeTranslationKeys("mixamorig:Hips");
#endif

   


    BuildGameObjects();

    Animation_Systems::render_System(entity_registry, nullptr);


    EventP::EventQueue::Listener eventLogger = [&](EventP::EventQueue::Event e) {
        eeng::Log(e->_event.c_str());
        

     };


    EventP::EventQueue::Listener getFood_GameEvent = [&](EventP::EventArgs* e) {
        if (questLineState != QuestLine::GetGrass)
            return;
        
        if (e->_event != "EVENT_TriggerStart" && e->_event != "EVENT_TriggerEnd")
            return;

        bool isStart = e->_event == "EVENT_TriggerStart";

        CollisionPackage::Collision_Args* args = static_cast<CollisionPackage::Collision_Args*>(e);

        if (!entity_registry->any_of<Tag_Component>(args->_sender))
            return;
        auto& senderTag = entity_registry->get<Tag_Component>(args->_sender);

        if (!entity_registry->any_of<Tag_Component>(args->otherCollider))
            return;
        auto& colliderTag = entity_registry->get<Tag_Component>(args->otherCollider);


        if (!senderTag.tags.contains("food") || !colliderTag.tags.contains("player"))
            return;


        questLineState = QuestLine::GoToHorse;
        //colliderTag.tags.emplace("hasFood");
        
       

    };
    
    EventP::EventQueue::Listener goToHorse_GameEvent = [&](EventP::EventArgs* e) {
        if (questLineState != QuestLine::GoToHorse && questLineState != QuestLine::FeedHorse)
            return;
        
        if (e->_event != "EVENT_TriggerStart" && e->_event != "EVENT_TriggerEnd")
            return;

        bool isStart = e->_event == "EVENT_TriggerStart";

        CollisionPackage::Collision_Args* args = static_cast<CollisionPackage::Collision_Args*>(e);

        if (!entity_registry->any_of<Tag_Component>(args->_sender))
            return;
        auto& senderTag = entity_registry->get<Tag_Component>(args->_sender);

        if (!entity_registry->any_of<Tag_Component>(args->otherCollider))
            return;
        auto& colliderTag = entity_registry->get<Tag_Component>(args->otherCollider);

        
        if (!senderTag.tags.contains("horse") || !colliderTag.tags.contains("player"))
            return;

        
        if (!entity_registry->any_of<Animation_Component>(args->_sender))
            return;
        questLineState = isStart ? QuestLine::FeedHorse : QuestLine::GoToHorse;

    };
    
    EventP::EventQueue::Listener giveFood_GameEvent = [&](EventP::EventArgs* e) {
            if ( questLineState != QuestLine::FeedHorse)
                return;

            if (e->_event != "EVENT_KeyStartPress_E")
                return;

        
            questLineState = QuestLine::Done;
       

        };


    event_dispatcher.RegisterListener(eventLogger);

    event_dispatcher.RegisterListener(getFood_GameEvent);
    event_dispatcher.RegisterListener(goToHorse_GameEvent);

    event_dispatcher.RegisterListener(giveFood_GameEvent);



    

    return true;
}


// turns placeholder stuff into game objects (this too is a placeholder function)
void Game::BuildGameObjects() {
    // build grass GO
    auto grass = entity_registry->create();
    
    

    entity_registry->emplace<Transform_Component>
        (grass, Transform_Component{
            
            { 0.0f, 0.0f, 0.0f },
            0.0f,0.0f,
            { 100.0f, 100.0f, 100.0f }
        });

    entity_registry->emplace<RenderableMesh_Component>
        (grass, RenderableMesh_Component{
            meshManager->Get("grass_trees_merged_001")
            });

    entity_registry->emplace<CollisionPackage::PlaneCollider_Component>
        (grass, CollisionPackage::PlaneCollider_Component{});


    auto foodSpot = entity_registry->create();
    entity_registry->emplace<Transform_Component>
        (foodSpot, Transform_Component{

                { 0.0f, 0.0f, -35.0f },
                0.0f, 0.0f,
                { 1, 1, 1 }
            });

    entity_registry->emplace<CollisionPackage::PhysicsObject_Component>
        (foodSpot, CollisionPackage::PhysicsObject_Component{});

    eeng::AABB a;
    a.max = { 10,10,10 };
    a.min = { 0,0,0 };
    entity_registry->emplace<CollisionPackage::PhysicsCollider_Component>
        (foodSpot, CollisionPackage::PhysicsCollider_Component{
            true,
            a
            
        });

    entity_registry->emplace<Tag_Component>
        (foodSpot, Tag_Component{
            {"food"}
            });


    for (int i = 0; i < 1; i++) {

        // build horse GO
        auto horse = entity_registry->create();

        entity_registry->emplace<Transform_Component>
            (horse, Transform_Component{

                    { 30.0f, 0.0f, -35.0f },
                    35.0f, 0.0f,
                    { 0.01f, 0.01f, 0.01f }
                });

        entity_registry->emplace<IMGUI_WorldPos_Window_Component>
            (horse, IMGUI_WorldPos_Window_Component{});

        entity_registry->emplace<RenderableMesh_Component>
            (horse, RenderableMesh_Component{
                meshManager->Get("horse_001")
                });

        

        entity_registry->emplace<LinearVelocity_Component>
            (horse, LinearVelocity_Component{});

        entity_registry->emplace<CollisionPackage::PhysicsObject_Component>
            (horse, CollisionPackage::PhysicsObject_Component{});

        entity_registry->emplace<CollisionPackage::PhysicsCollider_Component>
            (horse, CollisionPackage::PhysicsCollider_Component{true });

        entity_registry->emplace<Animation_Basic_Component>
            (horse, Animation_Basic_Component{
                1,1
                });


        entity_registry->emplace<Animation_Component>
            (horse, Animation_Component{
                0, 1,
                false,
                0
                });

        entity_registry->emplace<Tag_Component>
            (horse, Tag_Component{
                {"horse"}
                });

 
    }

    
    //auto a = std::make_shared<eeng::RenderableMesh>(*characterMesh);

    
    for (int i = 0; i < 30; i++) {
        // build character 1 GO
        auto c2 = entity_registry->create();

        entity_registry->emplace<Transform_Component>
            (c2, Transform_Component{
                
                    {-3,0,0},
                    0.0f, 0.0f,
                    { 0.03f, 0.03f, 0.03f }
                });

        
        
        entity_registry->emplace<RenderableMesh_Component>
            (c2, RenderableMesh_Component{
                meshManager->Get("character_001")
                });

        entity_registry->emplace<LinearVelocity_Component>
            (c2, LinearVelocity_Component{
                glm_aux::vec3_001
                });

        entity_registry->emplace<RotateToVelocity_Component>
            (c2, RotateToVelocity_Component{
                });


        entity_registry->emplace<SteeringBehavior_Component>
            (c2, SteeringBehavior_Component{
                8
                });

        auto seed = BitShiftRandom(i + 10);

        entity_registry->emplace<SB_Wander_Component>
            (c2, SB_Wander_Component{
                seed
                });

        entity_registry->emplace<SB_Separation_Component>
            (c2, SB_Separation_Component{
                });



        entity_registry->emplace<Animation_Component>
            (c2, Animation_Component{
                1, 2,
                false,
                0.5,
                });
        /*entity_registry->emplace<Animation_Basic_Component>
            (c3, Animation_Basic_Component{
                1,1
                });*/

        entity_registry->emplace<Animation_1dBlendSpace_Component>
            (c2, Animation_1dBlendSpace_Component{
                {
                    BlendSpaceElement {1,0},
                    BlendSpaceElement {2,2},
                    BlendSpaceElement {2,5},
                    BlendSpaceElement {3,6},
                }

                });

        entity_registry->emplace<Animation_FromSpeed_Component>
            (c2, Animation_FromSpeed_Component{
                1
                });


        std::string str = std::to_string(seed);
        const char* chars = str.c_str();


        /*entity_registry->emplace<UI_ModifyObject_Component>
            (c2, UI_ModifyObject_Component{
                "npc"
                });*/
    }


    
    // build character 3 GO
    auto playerCharacter = entity_registry->create();

    entity_registry->emplace<UI_TextBox_Component>
        (playerCharacter, UI_TextBox_Component{"hello wolrd"});

    entity_registry->emplace<Transform_Component>
        (playerCharacter, Transform_Component{
            
                {6,0,0},
                0.0f, 0.0f,
                { 0.03f, 0.03f, 0.03f }
            });

    entity_registry->emplace<IMGUI_WorldPos_Window_Component>
        (playerCharacter, IMGUI_WorldPos_Window_Component{});

    entity_registry->emplace<RenderableMesh_Component>
        (playerCharacter, RenderableMesh_Component{
            meshManager->Get("character_001")
            });

    entity_registry->emplace<LinearVelocity_Component>
        (playerCharacter, LinearVelocity_Component{
            glm_aux::vec3_000
            });
    entity_registry->emplace<RotateToVelocity_Component>
        (playerCharacter, RotateToVelocity_Component{
            0.5f
            });



   entity_registry->emplace<Animation_Basic_Component>
        (playerCharacter, Animation_Basic_Component{
            1,1
            });
   

    entity_registry->emplace<Animation_Component>
        (playerCharacter, Animation_Component{
            2, 4,
            true,
            0.5,
            eeng::AnimationBranchDesc{
                "mixamorig:Spine2",
            }
        });

    entity_registry->emplace<CollisionPackage::PhysicsObject_Component>
        (playerCharacter, CollisionPackage::PhysicsObject_Component{});

    entity_registry->emplace<CollisionPackage::PhysicsCollider_Component>
        (playerCharacter, CollisionPackage::PhysicsCollider_Component{});


    /*entity_registry->emplace<Animation_Component>
        (c3, Animation_Component{
            1, 2,
            false,
            0.5,
            });
    

    entity_registry->emplace<Animation_1dBlendSpace_Component>
        (c3, Animation_1dBlendSpace_Component{
            {
                BlendSpaceElement {1,0},
                BlendSpaceElement {2,6},
                BlendSpaceElement {3,8},
            }
            
            });

    entity_registry->emplace<Animation_FromSpeed_Component>
        (c3, Animation_FromSpeed_Component{
            1
            });*/





    entity_registry->emplace<Tag_Component>
        (playerCharacter, Tag_Component{
            {"player"}
            });
    

    // build camera GO
    auto cam = entity_registry->create();

    entity_registry->emplace<Transform_Component>
        (cam, Transform_Component{
            
                {0,0,0},
                0.0f,  -glm::pi<float>() / 8,
                { 1, 1, 1 }
            });

    entity_registry->emplace<LookAtOrbit_Component>
        (cam, LookAtOrbit_Component{
            playerCharacter
            });

    entity_registry->emplace<Camera_Component>
        (cam, Camera_Component{
            
            });


    camera_entity = cam;



    // set player camera target
    entity_registry->emplace<PlayerController_Component>
        (playerCharacter, PlayerController_Component{
            cam,
            6
            });

    

    

    


}


void Game::update(
    float time,
    float deltaTime,
    InputManagerPtr input)
{

    updateSystems(time, deltaTime, input);
    


    //updateCamera(input);

    //updatePlayer(deltaTime, input);

    pointlight.pos = glm::vec3(
        glm_aux::R(time * 0.1f, { 0.0f, 1.0f, 0.0f }) *
        glm::vec4(100.0f, 100.0f, 100.0f, 1.0f));

  

    

    
}

void Game::render(
    float time,
    int windowWidth,
    int windowHeight)
{
    renderUI();

    matrices.windowSize = glm::ivec2(windowWidth, windowHeight);

    auto& camera_transform = entity_registry->get<Transform_Component>(camera_entity);
    auto& camera_component = entity_registry->get<Camera_Component>(camera_entity);

    // Projection matrix
    const float aspectRatio = float(windowWidth) / windowHeight;
    matrices.P = glm::perspective(glm::radians(60.0f), aspectRatio, camera_component.nearPlane, camera_component.farPlane);
    // View matrix
    auto local_to_world = camera_transform.GetTransform();
    


    bool legacyCamera = false;

    matrices.V = glm::inverse(local_to_world);// 
    
    // Viewport matrix
    matrices.VP = glm_aux::create_viewport_matrix(0.0f, 0.0f, windowWidth, windowHeight, 0.0f, 1.0f);
    
#pragma region RenderPass

    // Begin rendering pass
    forwardRenderer->beginPass(matrices.P, matrices.V, pointlight.pos, pointlight.color, camera_transform._position);
    
    

    // update systems in the render pass
    renderPassSystems(time);


    // End rendering pass
    drawcallCount = forwardRenderer->endPass();
#pragma endregion
    
    // Draw shape batches (lines etc)
    shapeRenderer->render(matrices.P * matrices.V);
    shapeRenderer->post_render();
}

void Game::renderUI()
{

    UI_Systems();




    // Begin game info ImGui window
    ImGui::Begin("Game Info");

    ImGui::Text("%i", questLineState);


    ImGui::Text("Drawcall count %i", drawcallCount);

    // Color picker for light color
    if (ImGui::ColorEdit3("Light color",
        glm::value_ptr(pointlight.color),
        ImGuiColorEditFlags_NoInputs))
    {
    }

    {
        /*ImGui::Separator();
        if (ImGui::Button("Quit")) {
            
        }*/

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Misc", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show GameObject Ui", &show_ModifyObjectUI);
            ImGui::Checkbox("Show Bone Gizmos", &show_debugAnimations);
        }
    }


    ImGui::End(); // end info window

}

void Game::imGui_Animation_Selector(std::shared_ptr<eeng::RenderableMesh> mesh, int& currentIndex, std::string title, std::string id) {

    // Combo (drop-down) for animation clip
    int curAnimIndex = currentIndex;
    std::string label = (curAnimIndex == 0 ? "Bind pose" : mesh->getAnimationName(curAnimIndex));
    
    

    std::string titleId = title+ "##" + id;

    if (ImGui::BeginCombo(titleId.c_str(), label.c_str()))
    {
        // Bind pose item
        const bool isSelected = (curAnimIndex == 0);
        if (ImGui::Selectable("Bind pose", isSelected))
            curAnimIndex = 0;
        if (isSelected)
            ImGui::SetItemDefaultFocus();

        // Clip items
        for (int i = 1; i < mesh->getNbrAnimations(); i++)
        {
            const bool isSelected = (curAnimIndex == i);
            const auto label = mesh->getAnimationName(i) + "##" + std::to_string(i) + id;
            if (ImGui::Selectable(label.c_str(), isSelected))
                curAnimIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    currentIndex = curAnimIndex;


}

void Game::destroy()
{
    
}






#pragma region systems


void Game::Input_System(InputManagerPtr input, EventP::EventQueue& event_dispatcher) {

    using Key = eeng::InputManager::Key;

    
    
    if (input->IsKeyPressed(Key::E)) {
        if (!keyIsPressedMap[Key::E]) {
            keyIsPressedMap[Key::E] = true;
            auto arg = new EventP::EventArgs{"EVENT_KeyStartPress_E"};
            event_dispatcher.EnqueueEvent(arg);
        }
    }
    else if (keyIsPressedMap[Key::E]) {    
        keyIsPressedMap[Key::E] = false;
    }

    // add other keys here

}




void Game::velocity_System(float deltaTime) {
    auto view = entity_registry->view<
            Transform_Component, 
            LinearVelocity_Component
        >();

    for (auto entity : view) {

        auto& transform = view.get<Transform_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);
        
        transform._position += velocity._velocity * deltaTime;
    }

    
}

void Game::player_System(float deltaTime, InputManagerPtr input) {
    auto view = entity_registry->view<
        Transform_Component,
        LinearVelocity_Component,
        PlayerController_Component
    >();

    for (auto entity : view) {

        auto& player = view.get<PlayerController_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);
        
        auto& ref_transform = view.get<Transform_Component>(player._rotationRefrence);
        
        // Fetch keys relevant for player movement
        using Key = eeng::InputManager::Key;
        bool W = input->IsKeyPressed(Key::W);
        bool A = input->IsKeyPressed(Key::A);
        bool S = input->IsKeyPressed(Key::S);
        bool D = input->IsKeyPressed(Key::D);


        glm::vec3 local_movement =
            glm_aux::vec3_001 * deltaTime * ((W ? -1.0f : 0.0f) + (S ? 1.0f : 0.0f)) +
            glm_aux::vec3_100 * deltaTime * ((A ? -1.0f : 0.0f) + (D ? 1.0f : 0.0f));
        

        

        glm::vec4 world_movement = ref_transform.GetTransform() * glm::vec4(local_movement, 0);

        world_movement.y = 0;

        auto l = glm::length(world_movement);


        player.target_movement = l <= 0 ? glm_aux::vec3_000 : glm::normalize(glm::vec3(world_movement));

        player.current_movement = player.current_movement * player.movement_lerp + player.target_movement * (1.0f - player.movement_lerp);

        velocity._velocity = player.current_movement * player.speed;
       
    }


}


void Game::LookAt_System(InputManagerPtr input) {
    auto view = entity_registry->view<
        Transform_Component,
        LookAtOrbit_Component
    >();

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& lookAt = view.get<LookAtOrbit_Component>(entity);
        auto& target_transform = view.get<Transform_Component>(lookAt._lookAtTarget);

        // Fetch mouse and compute movement since last frame
        auto mouse = input->GetMouseState();
        glm::ivec2 mouse_xy{ mouse.x, mouse.y };
        glm::ivec2 mouse_xy_diff{ 0, 0 };
        if (mouse.leftButton && lookAt._mouse_xy_prev.x >= 0)
            mouse_xy_diff = lookAt._mouse_xy_prev - mouse_xy;
        lookAt._mouse_xy_prev = mouse_xy;

        // Update camera rotation from mouse movement
        transform._yaw += mouse_xy_diff.x * lookAt.sensitivity;
        transform._pitch += mouse_xy_diff.y * lookAt.sensitivity;
        transform._pitch = glm::clamp(transform._pitch, -glm::radians(89.0f), 0.0f);

        // Update camera position
        const auto toTarget = glm_aux::T(glm::vec3(target_transform._position));

        auto t = toTarget * glm_aux::R(transform._yaw, transform._pitch) * glm_aux::T(glm::vec3(0.0f, 0.0f, lookAt._distance));
        
        
        //const glm::vec4 targetPos = target_transform._world_transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        


        //transform._world_transform = toTarget * transform._world_transform;

        transform._position = t[3];

    }

}



void  Game::SteeringBehavior_System(float deltaTime) {


    auto view = entity_registry->view<
        Transform_Component,
        SteeringBehavior_Component,
        LinearVelocity_Component
    >();

    float maxRadius = 20;

    float radiusForce = 5;

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);

        auto& steering = view.get<SteeringBehavior_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);
        
        auto pos = transform._position;
        float distanceToCenter = glm::length(pos);
        if (distanceToCenter > maxRadius) {
            steering._acceleration -= glm::normalize(pos)* (distanceToCenter - maxRadius) * radiusForce;
        }
       

        velocity._velocity += steering._acceleration / 60.0f;

        steering._acceleration = glm_aux::vec3_000;

        float speed = glm::length(velocity._velocity);



        if (speed >= steering._breakSpeed){
            velocity._velocity -= velocity._velocity * steering._breakDampening;
        }
        
        steering._forward = glm::normalize(velocity._velocity);
        steering._left = glm::cross(steering._forward, glm_aux::vec3_010);
        

        steering._refPos = transform._position;//glm::vec3(transform._world_transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) + velocity._velocity;
    }

}

void Game::SB_Wander_System(float time) {
    auto view = entity_registry->view<
        SteeringBehavior_Component,
        SB_Wander_Component
    >();

    for (auto entity : view) {
        auto& steering = view.get<SteeringBehavior_Component>(entity);
        auto& sb_wander = view.get<SB_Wander_Component>(entity);

        

        auto force = steering._left * Get1DNoise(time * sb_wander._driftSpeed + (sb_wander._seed % 60000) / 3.0f) * sb_wander._speed;

        steering._acceleration += force;
    }
}

void Game::SB_Separation_System() {

    auto steeringEntities = entity_registry->view<SteeringBehavior_Component>();

    auto view = entity_registry->view<SB_Separation_Component, SteeringBehavior_Component>();


    for (auto entity : view) {
            
        auto& this_steering = view.get<SteeringBehavior_Component>(entity);
        auto& this_behavior = view.get<SB_Separation_Component>(entity);

           

        for (auto other : steeringEntities) {
            auto& other_steering = view.get<SteeringBehavior_Component>(other);

            auto f = this_steering._refPos - other_steering._refPos;

            float mag = glm::length(f);

            if (mag <= 0 || mag > this_behavior._distance)
                continue;

            f *= 1.0f / mag;

            this_steering._acceleration += f * this_behavior._speed;
        }            

    }

    




}



void Game::RoateToDriection_System() {
    auto view = entity_registry->view<
        Transform_Component,
        LinearVelocity_Component,
        RotateToVelocity_Component
    >();

    

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);
        auto& rotator = view.get<RotateToVelocity_Component>(entity);

        auto c = glm::normalize(glm::normalize(velocity._velocity));

        float dotForward = glm::dot(c, glm_aux::vec3_001);
        float dotLeft = glm::dot(c, glm_aux::vec3_100);

        float absAngle = (1 - dotForward) * glm::pi<float>() * 0.5f;
        
        float angle = dotLeft < 0 ? -absAngle : absAngle;
        
        if (std::isnan(angle))
            continue;

        transform._yaw = angle;

    }
}





void Game::imGui_WorldToScreen_System(std::shared_ptr<entt::registry> entity_registry) {
    auto view = entity_registry->view<Transform_Component, IMGUI_WorldPos_Window_Component>();

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& ui_window = view.get<IMGUI_WorldPos_Window_Component>(entity);

        if (!ui_window._isOn)
            return;

        glm::ivec2 window_cords;

        if (!imGui_Prepare_WorldToScreen(transform._position, window_cords))
            continue;

        

        imGui_Base_WorldPositionUI(entity, [&]() {
            ImGui::Text("ScreenPos: (%i, %i)", window_cords.x, window_cords.y);
            });

    }


}


void Game::imGui_W_Transform_System() {
    auto view = entity_registry->view<Transform_Component, IMGUI_WorldPos_Window_Component>();

    float worldPos[3];
    float rotation[2];
    float scale[3];

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& ui_window = view.get<IMGUI_WorldPos_Window_Component>(entity);

        if (!ui_window._isOn)
            return;

        // get world pos
        worldPos[0] = transform._position.x;
        worldPos[1] = transform._position.y;
        worldPos[2] = transform._position.z;

        // get rotation
        rotation[0] = transform._yaw;
        rotation[1] = transform._pitch;

        // get scale
        scale[0] = transform._scale.x;
        scale[1] = transform._scale.y;
        scale[2] = transform._scale.z;

        auto lambda = [&]() {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                //  world pos
                if (ImGui::InputFloat3("World pos", worldPos/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._position = {
                        worldPos[0],
                        worldPos[1],
                        worldPos[2]

                    };
                }
                //  rotation
                if (ImGui::InputFloat2("Yaw, Pitch", rotation/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._yaw = rotation[0];
                    transform._pitch = rotation[1];
                }
                //  scale
                if (ImGui::InputFloat3("Scale", scale/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._scale = {
                        scale[0],
                        scale[1],
                        scale[2]

                    };
                }
            }
        };

        imGui_Base_WorldPositionUI(entity, lambda);
    }


}


void Game::imGui_W_Animation_Controller_System(std::shared_ptr<entt::registry> entity_registry) {
    auto view = entity_registry->view<Transform_Component, RenderableMesh_Component, Animation_Component, Animation_Basic_Component, IMGUI_WorldPos_Window_Component>();
    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& mesh      = view.get<RenderableMesh_Component>(entity);
        auto& animation = view.get<Animation_Component>(entity);
        auto& animation_controller = view.get<Animation_Basic_Component>(entity);
        auto& ui_window = view.get<IMGUI_WorldPos_Window_Component>(entity);
        
        if (!ui_window._isOn)
            return;

        auto lambda = [&]() {
            if (ImGui::CollapsingHeader("Animations", ImGuiTreeNodeFlags_DefaultOpen)){
                ImGui::Separator();
                ImGui::Text("Animations");
                imGui_Animation_Selector(mesh._renderable_mesh, animation.baseAnimation, "A", "A_Animation_Selection");
                imGui_Animation_Selector(mesh._renderable_mesh, animation.secondaryAnimation, "B", "B_Animation_Selection");

                ImGui::Separator();
                ImGui::Text("Blending");
                ImGui::Checkbox("Use Layering", &animation.useLayering);
                ImGui::SliderFloat("Blend", &animation.blendFactor, 0, 1);

                ImGui::Separator();
                ImGui::Text("Speed");
                ImGui::InputFloat("A", &animation_controller.speedMult0);
                ImGui::InputFloat("B", &animation_controller.speedMult1);

            }    
        };

        


        imGui_Base_WorldPositionUI(entity, lambda);
    }
}

void Game::imGui_W_Tag_System(std::shared_ptr<entt::registry> entity_registry) {
    auto view = entity_registry->view<Transform_Component,Tag_Component, IMGUI_WorldPos_Window_Component>();
    
    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& tags = view.get<Tag_Component>(entity);
        auto& ui_window = view.get<IMGUI_WorldPos_Window_Component>(entity);
        

        if (!ui_window._isOn)
            return;

        

        auto lambda = [&]() {
            if (ImGui::CollapsingHeader("Tag", ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& tag : tags.tags) {
                    ImGui::Text(tag.c_str());
                }
                
                //ImGui::InputText("Tag", (char*)str->c_str(), str->capacity() + 1);
            }
        };

        imGui_Base_WorldPositionUI(entity, lambda);
        //imGui_Base_WorldPositionUI(transform, entity, lambda);



    }
}

void Game::imGui_W_TextBox_System(std::shared_ptr<entt::registry> entity_registry) {
    auto view = entity_registry->view<UI_TextBox_Component, IMGUI_WorldPos_Window_Component>();

    for (auto entity : view) {
        auto& ui_window = view.get<IMGUI_WorldPos_Window_Component>(entity);
        auto& textbox = view.get<UI_TextBox_Component>(entity);

        if (!ui_window._isOn)
            return;



        auto lambda = [&]() {
            ImGui::Text(textbox.text.c_str());
            };

        imGui_Base_WorldPositionUI(entity, lambda);
        //imGui_Base_WorldPositionUI(transform, entity, lambda);



    }
}



bool Game::imGui_Prepare_WorldToScreen(glm::vec3 worldPos, glm::ivec2& screenPos_Out) {

    // In-world position label at object position
    const auto VP_P_V = matrices.VP * matrices.P * matrices.V;
    auto world_pos = worldPos;
    glm::ivec2 window_coords;
    if (glm_aux::window_coords_from_world_pos(world_pos, VP_P_V, window_coords))
    {

        screenPos_Out = window_coords;

        // Draw an ImGui label at the projected window coordinates of the horse
        ImGui::SetNextWindowPos(
            ImVec2{ float(window_coords.x), float(matrices.windowSize.y - window_coords.y) },
            ImGuiCond_Always,
            ImVec2{ 0.0f, 0.0f });
        return true;
    }

    return false;
}




void Game::imGui_Base_WorldPositionUI(entt::entity& entity, const std::function<void()>& func) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x80000000);
    ImGui::PushStyleColor(ImGuiCol_Text, 0xffffffff);

    ImGuiWindowFlags flags =
        //ImGuiWindowFlags_NoDecoration;
        //ImGuiWindowFlags_NoInputs |
        // ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize;

    

    if (ImGui::Begin(("GameObject" + std::to_string((int)entity)).c_str())) {
        func();
    }

    ImGui::End();

    ImGui::PopStyleColor(2);
}





void Game::Animation_BySpeed_System(std::shared_ptr<entt::registry> entity_registry) {
    auto view = entity_registry->view<
        Transform_Component, 
        LinearVelocity_Component, 
        Animation_FromSpeed_Component, 
        Animation_1dBlendSpace_Component
    >();

    for (auto entity : view) {
        auto& transform =   view.get<Transform_Component>(entity);
        auto& velocity =    view.get<LinearVelocity_Component>(entity);
        auto& aBySpeed =    view.get<Animation_FromSpeed_Component>(entity);
        auto& aBlendSpace = view.get<Animation_1dBlendSpace_Component>(entity);

        float speed = glm::length(velocity._velocity);
        
        aBlendSpace.value = speed * aBySpeed.speed_mult;


    }

}



void Game::Transform_DebugView() {
    auto view = entity_registry->view<Transform_Component>();

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);

        shapeRenderer->push_basis_basic(transform.GetTransform(), 1.0f);

    }
}

void  Game::Velocity_DebugView() {
    auto view = entity_registry->view<
        Transform_Component,
        LinearVelocity_Component
    >();

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);

        auto origin = glm::vec3(transform.GetTransform() * glm::vec4(0, 0, 0, 1));

        auto dest = origin + velocity._velocity;

        shapeRenderer->push_line(origin, dest);

    }


    auto view2 = entity_registry->view<
        Transform_Component,
        SteeringBehavior_Component
    >();

    for (auto entity : view2) {
        auto& transform = view2.get<Transform_Component>(entity);
        auto& steering = view2.get<SteeringBehavior_Component>(entity);

        auto origin = glm::vec3(transform.GetTransform() * glm::vec4(0, 1, 0, 1));

        auto dest = origin + steering._forward;

        shapeRenderer->push_line(origin, dest);
        

    }
}






#pragma endregion




void Game::UI_Systems() {
    if (show_ModifyObjectUI) {
        imGui_WorldToScreen_System(entity_registry);

        imGui_W_TextBox_System(entity_registry);

        imGui_W_Transform_System();

        imGui_W_Animation_Controller_System(entity_registry);

        imGui_W_Tag_System(entity_registry);
    }
}

void Game::updateSystems(float time,
    float deltaTime,
    InputManagerPtr input) {

    Input_System(input, event_dispatcher);

    LookAt_System(input);

    player_System(deltaTime, input);

    //npc_System(time);


    SB_Wander_System(time);

    SB_Separation_System();

    SteeringBehavior_System(deltaTime);

    // animations
    Animation_BySpeed_System(entity_registry);
    
    Animation_Systems::animation_Basic_System(entity_registry, deltaTime);
    Animation_Systems::animation_1DBlendSpace_System(entity_registry, deltaTime);


    // update transform with velocity
    RoateToDriection_System();
    CollisionPackage::UpdateColliders_System(entity_registry);

    CollisionPackage::Gravity_System(entity_registry, glm::vec3{ 0,-9.82,0 });
    CollisionPackage::PhysicsUpdate_System(entity_registry, deltaTime);
    CollisionPackage::PlaneColission_System(entity_registry);
    CollisionPackage::DynamicColission_System(entity_registry, event_dispatcher);
    
    CollisionPackage::CollisionStartEnd_System(entity_registry, event_dispatcher);

    velocity_System(deltaTime);
    CollisionPackage::UpdateColliders_System(entity_registry);

    event_dispatcher.InvokeEvents();
}


void Game::renderPassSystems(float time) {


    //render meshes
    Animation_Systems::render_System(entity_registry, forwardRenderer);




    Transform_DebugView();
    Velocity_DebugView();
    if (show_debugAnimations) {
        Animation_Systems::debug_mesh_bones_System(entity_registry, shapeRenderer);
    }

    CollisionPackage::DebugColliders_System(entity_registry, shapeRenderer);

}


