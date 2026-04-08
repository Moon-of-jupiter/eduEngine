
#include <entt/entt.hpp>
#include "glmcommon.hpp"
#include "imgui.h"
#include "Log.hpp"
#include "Game.hpp"
#include "DataComponents.h"

bool Game::init()
{

    forwardRenderer = std::make_shared<eeng::ForwardRenderer>();
    forwardRenderer->init("shaders/phong_vert.glsl", "shaders/phong_frag.glsl");

    shapeRenderer = std::make_shared<ShapeRendering::ShapeRenderer>();
    shapeRenderer->init();

    // Do some entt stuff
    entity_registry = std::make_shared<entt::registry>();
    auto ent1 = entity_registry->create();
    struct Tfm
    {
        float x, y, z;
    };
    entity_registry->emplace<Tfm>(ent1, Tfm{});

    // Grass
    grassMesh = std::make_shared<eeng::RenderableMesh>();
    grassMesh->load("assets/grass/grass_trees_merged.fbx", false);

    // Horse
    horseMesh = std::make_shared<eeng::RenderableMesh>();
    horseMesh->load("assets/Animals/Horse.fbx", false);

    // Character
    characterMesh = std::make_shared<eeng::RenderableMesh>();
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
#if 1
    // Amy 5.0.1 PACK FBX
    characterMesh->load("assets/Amy/Ch46_nonPBR.fbx");
    characterMesh->load("assets/Amy/idle.fbx", true);
    characterMesh->load("assets/Amy/walking.fbx", true);
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

    grassWorldMatrix = glm_aux::TRS(
        { 0.0f, 0.0f, 0.0f },
        0.0f, { 0, 1, 0 },
        { 100.0f, 100.0f, 100.0f });

    horseWorldMatrix = glm_aux::TRS(
        { 30.0f, 0.0f, -35.0f },
        35.0f, { 0, 1, 0 },
        { 0.01f, 0.01f, 0.01f });


    BuildGameObjects();

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
            grassMesh
            });


    // build horse GO
    auto horse = entity_registry->create();

    entity_registry->emplace<Transform_Component>
        (horse, Transform_Component{
            
                { 30.0f, 0.0f, -35.0f },
                35.0f, 0.0f,
                { 0.01f, 0.01f, 0.01f }
            });

    entity_registry->emplace<RenderableMesh_Component>
        (horse, RenderableMesh_Component{
            horseMesh
            });

    entity_registry->emplace<UI_ModifyObject_Component>
        (horse, UI_ModifyObject_Component{
            "horse 1"
            });


    
    if (false) {
        // build character 1 GO
        auto c1 = entity_registry->create();

        entity_registry->emplace<Transform_Component>
            (c1, Transform_Component{
                
                    glm_aux::vec3_000,
                    0.0f, 0.0f,
                    { 0.03f, 0.03f, 0.03f }
                });

        entity_registry->emplace<RenderableMesh_Component>
            (c1, RenderableMesh_Component{
                characterMesh
                });
    }


    for (int i = 0; i < 20; i++) {
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
                characterMesh
                });

        entity_registry->emplace<LinearVelocity_Component>
            (c2, LinearVelocity_Component{
                glm_aux::vec3_001
                });

        entity_registry->emplace<SteeringBehavior_Component>
            (c2, SteeringBehavior_Component{
                
                });

        entity_registry->emplace<SB_Wander_Component>
            (c2, SB_Wander_Component{
                BitShiftRandom(i + 10)
                });

        entity_registry->emplace<SB_Separation_Component>
            (c2, SB_Separation_Component{
                });

        
    }


    
    // build character 3 GO
    auto c3 = entity_registry->create();

    entity_registry->emplace<Transform_Component>
        (c3, Transform_Component{
            
                {6,0,0},
                0.0f, 0.0f,
                { 0.03f, 0.03f, 0.03f }
            });

    entity_registry->emplace<RenderableMesh_Component>
        (c3, RenderableMesh_Component{
            characterMesh
            });

    entity_registry->emplace<LinearVelocity_Component>
        (c3, LinearVelocity_Component{
            glm_aux::vec3_000
            });
        
    entity_registry->emplace<UI_ModifyObject_Component>
        (c3, UI_ModifyObject_Component{
            "player"
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
            c3
            });

    entity_registry->emplace<Camera_Component>
        (cam, Camera_Component{
            
            });


    camera_entity = cam;



    // set player camera target
    entity_registry->emplace<PlayerController_Component>
        (c3, PlayerController_Component{
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

    // !!! legacy !!!
    if (false) {
        characterWorldMatrix1 = glm_aux::TRS(
            player.pos,
            0.0f, { 0, 1, 0 },
            { 0.03f, 0.03f, 0.03f });

        characterWorldMatrix2 = glm_aux::TRS(
            { -3, 0, 0 },
            time * glm::radians(50.0f), { 0, 1, 0 },
            { 0.03f, 0.03f, 0.03f });

        characterWorldMatrix3 = glm_aux::TRS(
            { 3, 0, 0 },
            time * glm::radians(50.0f), { 0, 1, 0 },
            { 0.03f, 0.03f, 0.03f });
    }

    // Intersect player view ray with AABBs of other objects 
    // Intersection results are stored in the ray's point_of_contact and can be used for picking, shooting, etc.
    glm_aux::intersect_ray_AABB(player.viewRay, character_aabb2.min, character_aabb2.max);
    glm_aux::intersect_ray_AABB(player.viewRay, character_aabb3.min, character_aabb3.max);
    glm_aux::intersect_ray_AABB(player.viewRay, horse_aabb.min, horse_aabb.max);

    // We can also compute a ray from the current mouse position picking etc
    // Let's try it: if the left mouse button is pressed, 
    // compute a ray from the camera through the mouse cursor and log it
    if (input->GetMouseState().rightButton)
    {
        // Note: mouse Y is typically inverted in window coordinates, so we flip it here to get the correct ray direction
        glm::ivec2 windowPos(camera.mouse_xy_prev.x, matrices.windowSize.y - camera.mouse_xy_prev.y);
        
        // Compute a ray from the camera through the mouse cursor
        auto ray = glm_aux::world_ray_from_window_coords(windowPos, matrices.V, matrices.P, matrices.VP);
        
        // Now we can use the ray for picking, intersection tests, etc. For now, let's just log it.
        eeng::Log("Picking ray origin = %s, dir = %s",
            glm_aux::to_string(ray.origin).c_str(),
            glm_aux::to_string(ray.dir).c_str());
    }
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

    matrices.V = legacyCamera ? glm::lookAt(camera.pos, camera.lookAt, camera.up) :  glm::inverse(local_to_world);// 
    
    // Viewport matrix
    matrices.VP = glm_aux::create_viewport_matrix(0.0f, 0.0f, windowWidth, windowHeight, 0.0f, 1.0f);
    
#pragma region RenderPass

    // Begin rendering pass
    forwardRenderer->beginPass(matrices.P, matrices.V, pointlight.pos, pointlight.color, camera_transform._position);
    
    // !!! legacy !!!
    if (false) {
        // Grass
        forwardRenderer->renderMesh(grassMesh, grassWorldMatrix);
        grass_aabb = grassMesh->m_model_aabb.post_transform(grassWorldMatrix);

        // Horse
        horseMesh->animate(3, time);
        forwardRenderer->renderMesh(horseMesh, horseWorldMatrix);
        horse_aabb = horseMesh->m_model_aabb.post_transform(horseWorldMatrix);

        // Character, instance 1 (middle, moving) - single clip demo
        characterMesh->animate(middleCharacterAnimIndex, time * characterAnimSpeed);
        forwardRenderer->renderMesh(characterMesh, characterWorldMatrix1);
        character_aabb1 = characterMesh->m_model_aabb.post_transform(characterWorldMatrix1);

        // Character, instance 2 (left) - two-clip full-body blend
        // Explanation: Both 'idle' and 'walk' clips are applied to the entire skeleton with a blend factor.
        characterMesh->animateBlend(1, 2, time, time, leftCharacterAnimBlend);
        forwardRenderer->renderMesh(characterMesh, characterWorldMatrix2);
        character_aabb2 = characterMesh->m_model_aabb.post_transform(characterWorldMatrix2);

        // Character, instance 3 (right) - filtered walk + wave
        // Explanation: Nodes in the "mixamorig:Spine" branch (upper body) gets the 'wave' clip, while the rest (lower body) gets the 'walk' clip.
        eeng::AnimationBranchDesc upperBodyFilter;
        upperBodyFilter.root_node_name = "mixamorig:Spine";
        upperBodyFilter.mode = rightCharacterSubtreeUsesWave
            ? eeng::AnimationBranchDesc::Mode::IncludeSubtree
            : eeng::AnimationBranchDesc::Mode::ExcludeSubtree;
        characterMesh->animateBlend(2 /* walk */, 3 /* wave */, time, time, upperBodyFilter);
        forwardRenderer->renderMesh(characterMesh, characterWorldMatrix3);
        character_aabb3 = characterMesh->m_model_aabb.post_transform(characterWorldMatrix3);

    }

    // update systems in the render pass
    renderPassSystems(time);


    // End rendering pass
    drawcallCount = forwardRenderer->endPass();
#pragma endregion

    // Debug draw player view ray
    // Green line if the ray hits an object, white line if it doesn't.
    if (player.viewRay)
    {
        shapeRenderer->push_states(ShapeRendering::Color4u{ 0xff00ff00 });
        shapeRenderer->push_line(player.viewRay.origin, player.viewRay.point_of_contact());
    }
    else
    {
        shapeRenderer->push_states(ShapeRendering::Color4u{ 0xffffffff });
        shapeRenderer->push_line(player.viewRay.origin, player.viewRay.origin + player.viewRay.dir * 100.0f);
    }
    shapeRenderer->pop_states<ShapeRendering::Color4u>();

    // Debug draw object bases
    /*{
        shapeRenderer->push_basis_basic(characterWorldMatrix1, 1.0f);
        shapeRenderer->push_basis_basic(characterWorldMatrix2, 1.0f);
        shapeRenderer->push_basis_basic(characterWorldMatrix3, 1.0f);
        shapeRenderer->push_basis_basic(grassWorldMatrix, 1.0f);
        shapeRenderer->push_basis_basic(horseWorldMatrix, 1.0f);
    }*/

    // Debug draw AABBs
    {
        shapeRenderer->push_states(ShapeRendering::Color4u{ 0xFFE61A80 });
        shapeRenderer->push_AABB(character_aabb1.min, character_aabb1.max);
        shapeRenderer->push_AABB(character_aabb2.min, character_aabb2.max);
        shapeRenderer->push_AABB(character_aabb3.min, character_aabb3.max);
        shapeRenderer->push_AABB(horse_aabb.min, horse_aabb.max);
        shapeRenderer->push_AABB(grass_aabb.min, grass_aabb.max);
        shapeRenderer->pop_states<ShapeRendering::Color4u>();
    }

#if 0
    // Demo draw other shapes
    {
        shapeRenderer->push_states(glm_aux::T(glm::vec3(0.0f, 0.0f, -5.0f)));
        ShapeRendering::DemoDraw(shapeRenderer);
        shapeRenderer->pop_states<glm::mat4>();
    }
#endif

    // Draw shape batches (lines etc)
    shapeRenderer->render(matrices.P * matrices.V);
    shapeRenderer->post_render();
}

void Game::renderUI()
{

    UI_Systems();




    // Begin game info ImGui window
    ImGui::Begin("Game Info");



    ImGui::Text("Drawcall count %i", drawcallCount);

    // Color picker for light color
    if (ImGui::ColorEdit3("Light color",
        glm::value_ptr(pointlight.color),
        ImGuiColorEditFlags_NoInputs))
    {
    }

    {
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Misc", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show GameObject Ui", &show_ModifyObjectUI);

        }
    }


    if (characterMesh)
    {
        ImGui::Separator();
        ImGui::Text("Middle Character (controllable): Single Clip");

        // Combo (drop-down) for animation clip
        int curAnimIndex = middleCharacterAnimIndex;
        std::string label = (curAnimIndex == -1 ? "Bind pose" : characterMesh->getAnimationName(curAnimIndex));
        if (ImGui::BeginCombo("Clip##middle_animclip", label.c_str()))
        {
            // Bind pose item
            const bool isSelected = (curAnimIndex == -1);
            if (ImGui::Selectable("Bind pose", isSelected))
                curAnimIndex = -1;
            if (isSelected)
                ImGui::SetItemDefaultFocus();

            // Clip items
            for (int i = 0; i < characterMesh->getNbrAnimations(); i++)
            {
                const bool isSelected = (curAnimIndex == i);
                const auto label = characterMesh->getAnimationName(i) + "##" + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), isSelected))
                    curAnimIndex = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        middleCharacterAnimIndex = curAnimIndex;
        ImGui::SliderFloat("Animation speed##middle_speed", &characterAnimSpeed, 0.1f, 5.0f);
    }

    ImGui::Separator();
    ImGui::Text("Left Character: 2-Clip Blend");
    ImGui::Text("Idle (1) + Walking (2)");
    ImGui::SliderFloat("Blend factor##left_blend", &leftCharacterAnimBlend, 0.0f, 1.0f);

    ImGui::Separator();
    ImGui::Text("Right Character: Filtered Blend");
    ImGui::Text("Walking (2) + Waving (3)");
    ImGui::Text("Branch root: mixamorig:Spine");
    ImGui::Checkbox("Spine subtree uses waving", &rightCharacterSubtreeUsesWave);

    ImGui::End(); // end info window

    //// In-world position label at horse position
    //const auto VP_P_V = matrices.VP * matrices.P * matrices.V;
    //auto world_pos = glm::vec3(horseWorldMatrix[3]);
    //glm::ivec2 window_coords;
    //if (glm_aux::window_coords_from_world_pos(world_pos, VP_P_V, window_coords))
    //{
    //    // Draw an ImGui label at the projected window coordinates of the horse
    //    ImGui::SetNextWindowPos(
    //        ImVec2{ float(window_coords.x), float(matrices.windowSize.y - window_coords.y) },
    //        ImGuiCond_Always,
    //        ImVec2{ 0.0f, 0.0f });
    //    ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x80000000);
    //    ImGui::PushStyleColor(ImGuiCol_Text, 0xffffffff);

    //    ImGuiWindowFlags flags =
    //        ImGuiWindowFlags_NoDecoration |
    //        ImGuiWindowFlags_NoInputs |
    //        // ImGuiWindowFlags_NoBackground |
    //        ImGuiWindowFlags_AlwaysAutoResize;

    //    if (ImGui::Begin("window_name", nullptr, flags))
    //    {
    //        ImGui::Text("In-world GUI element");
    //        ImGui::Text("Window pos (%i, %i)", window_coords.x, window_coords.y);
    //        ImGui::Text("World pos (%1.1f, %1.1f, %1.1f)", world_pos.x, world_pos.y, world_pos.z);
    //        ImGui::End();
    //    }
    //    ImGui::PopStyleColor(2);
    //}
}

void Game::destroy()
{
}


#pragma region legacy 
void Game::updateCamera(
    InputManagerPtr input)
{
    // Fetch mouse and compute movement since last frame
    auto mouse = input->GetMouseState();
    glm::ivec2 mouse_xy{ mouse.x, mouse.y };
    glm::ivec2 mouse_xy_diff{ 0, 0 };
    if (mouse.leftButton && camera.mouse_xy_prev.x >= 0)
        mouse_xy_diff = camera.mouse_xy_prev - mouse_xy;
    camera.mouse_xy_prev = mouse_xy;

    // Update camera rotation from mouse movement
    camera.yaw += mouse_xy_diff.x * camera.sensitivity;
    camera.pitch += mouse_xy_diff.y * camera.sensitivity;
    camera.pitch = glm::clamp(camera.pitch, -glm::radians(89.0f), 0.0f);

    // Update camera position
    const glm::vec4 rotatedPos = glm_aux::R(camera.yaw, camera.pitch) * glm::vec4(0.0f, 0.0f, camera.distance, 1.0f);
    camera.pos = camera.lookAt + glm::vec3(rotatedPos);
}

void Game::updatePlayer(
    float deltaTime,
    InputManagerPtr input)
{
    // Fetch keys relevant for player movement
    using Key = eeng::InputManager::Key;
    bool W = input->IsKeyPressed(Key::W);
    bool A = input->IsKeyPressed(Key::A);
    bool S = input->IsKeyPressed(Key::S);
    bool D = input->IsKeyPressed(Key::D);

    // Compute vectors in the local space of the player
    player.fwd = glm::vec3(glm_aux::R(camera.yaw, glm_aux::vec3_010) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    player.right = glm::cross(player.fwd, glm_aux::vec3_010);

    // Compute the total movement as a 3D vector
    auto movement =
        player.fwd * player.velocity * deltaTime * ((W ? 1.0f : 0.0f) + (S ? -1.0f : 0.0f)) +
        player.right * player.velocity * deltaTime * ((A ? -1.0f : 0.0f) + (D ? 1.0f : 0.0f));

    // Update player position and forward view ray
    player.pos += movement;
    player.viewRay = glm_aux::Ray{ player.pos + glm::vec3(0.0f, 2.0f, 0.0f), player.fwd };

    // Update camera to track the player
    camera.lookAt += movement;
    camera.pos += movement;

}
#pragma endregion



#pragma region systems

void Game::render_System() {
    auto view = entity_registry->view<Transform_Component, RenderableMesh_Component>();

    for (auto entity : view) {

        auto& transform = view.get<Transform_Component>(entity);
        auto& mesh = view.get<RenderableMesh_Component>(entity);

        forwardRenderer->renderMesh(mesh._renderable_mesh, transform.GetTransform());


    }

    
}

void Game::updateAABB_System() {
    // to do
}

void Game::debugDrawAABB_System() {
    // to do
}



void Game::velocity_System(float deltaTime) {
    auto view = entity_registry->view<
            Transform_Component, 
            LinearVelocity_Component
        >();

    for (auto entity : view) {

        auto& transform = view.get<Transform_Component>(entity);
        auto& velocity = view.get<LinearVelocity_Component>(entity);
        
        if (velocity._velocity.length == 0) continue;

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

//void Game::npc_System(float time) {
//    auto view = entity_registry->view<
//        LinearVelocity_Component,
//        NpcController_Component
//    >();
//
//    for (auto entity : view) {
//        auto& velocity = view.get<LinearVelocity_Component>(entity);
//        auto& npc = view.get<NpcController_Component>(entity);
//
//        auto forward = glm::normalize(velocity._velocity);
//        auto left = glm::cross(forward, glm_aux::vec3_010);
//
//        auto force = left * Get1DNoise(time * npc._driftSpeed + npc._seed % 500) * npc._speed;
//
//        velocity._velocity += force;
//
//
//        float speed = glm::length(velocity._velocity);
//
//        if (speed < npc._breakSpeed)
//            continue;
//
//        velocity._velocity -= velocity._velocity * npc._breakDampening;
//    }
//}

void  Game::SteeringBehavior_System(float deltaTime) {


    auto view = entity_registry->view<
        Transform_Component,
        SteeringBehavior_Component,
        LinearVelocity_Component
    >();

    float maxRadius = 10;

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

            if (mag <= 0)
                continue;

            f *= 1.0f / mag;

            this_steering._acceleration += f * this_behavior._speed;
        }            

    }

    




}


// does not work
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

        rotator._target_direction = glm::normalize(velocity._velocity);

        rotator._current_direction =
            rotator._target_direction * rotator._lerpValue +
            rotator._current_direction * (1 - rotator._lerpValue);

        rotator._current_direction = rotator._target_direction;


        

        //transform._world_transform = transform._world_transform * glm_aux::R(yaw,0);
    }
}







void Game::UiModifyObject_System() {
    auto view = entity_registry->view<Transform_Component, UI_ModifyObject_Component>();

    for (auto entity : view) {
        auto& transform = view.get<Transform_Component>(entity);
        auto& ui_window = view.get<UI_ModifyObject_Component>(entity);


        // get world pos
        ui_window._worldPos[0] = transform._position.x;
        ui_window._worldPos[1] = transform._position.y;
        ui_window._worldPos[2] = transform._position.z;

        // get rotation
        ui_window._rotation[0] = transform._yaw;
        ui_window._rotation[1] = transform._pitch;

        // get scale
        ui_window._scale[0] = transform._scale.x;
        ui_window._scale[1] = transform._scale.y;
        ui_window._scale[2] = transform._scale.z;

        // In-world position label at object position
        const auto VP_P_V = matrices.VP * matrices.P * matrices.V;
        auto world_pos = transform._position;
        glm::ivec2 window_coords;
        if (glm_aux::window_coords_from_world_pos(world_pos, VP_P_V, window_coords))
        {
            // Draw an ImGui label at the projected window coordinates of the horse
            ImGui::SetNextWindowPos(
                ImVec2{ float(window_coords.x), float(matrices.windowSize.y - window_coords.y) },
                ImGuiCond_Always,
                ImVec2{ 0.0f, 0.0f });
            ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x80000000);
            ImGui::PushStyleColor(ImGuiCol_Text, 0xffffffff);

            ImGuiWindowFlags flags = 
            //ImGuiWindowFlags_NoDecoration;
            //ImGuiWindowFlags_NoInputs |
            // ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_AlwaysAutoResize;

            if (ImGui::Begin(ui_window.text)) {

                ImGui::Text("Window pos (%i, %i)", window_coords.x, window_coords.y);
                

                //  world pos
                if (ImGui::InputFloat3("World pos", ui_window._worldPos/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._position = {
                        ui_window._worldPos[0],
                        ui_window._worldPos[1],
                        ui_window._worldPos[2]

                    };
                }
                //  rotation
                if (ImGui::InputFloat2("Yaw, Pitch", ui_window._rotation/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._yaw = ui_window._rotation[0];
                    transform._pitch = ui_window._rotation[1];

                }
                //  scale
                if (ImGui::InputFloat3("Scale", ui_window._scale/*, "%.3f", ImGuiInputTextFlags_AlwaysOverwrite*/)) {

                    transform._scale = {
                        ui_window._scale[0],
                        ui_window._scale[1],
                        ui_window._scale[2]

                    };
                }


            }

            ImGui::End();

            ImGui::PopStyleColor(2);
            
        
        }
       


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
        UiModifyObject_System();
    }
}

void Game::updateSystems(float time,
    float deltaTime,
    InputManagerPtr input) {
    

    LookAt_System(input);

    player_System(deltaTime, input);

    //npc_System(time);

    //RoateToDriection_System();

    SB_Wander_System(time);

    SB_Separation_System();

    SteeringBehavior_System(deltaTime);

    // update transform with velocity
    velocity_System(deltaTime);

    

}


void Game::renderPassSystems(float time) {

    // update animations
    //      to do
    
    //render meshes
    render_System();

    // update AABB
    //      to do



    Transform_DebugView();
    Velocity_DebugView();

}
