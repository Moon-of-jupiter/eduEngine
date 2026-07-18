#ifndef Game_hpp
#define Game_hpp
#pragma once

#include <entt/fwd.hpp>
#include "GameBase.h"
#include "RenderableMesh.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"
#include "Log.hpp"


#include "DataComponents.h"
#include "AnimationPackage.h"
#include "EventPackage.h"
#include "MeshManager.h"


/// @brief A Game may hold, update and render 3D geometry and GUI elements
class Game : public eeng::GameBase
{
public:
    /// @brief For game resource initialization
    /// @return 
    bool init() override;

    /// @brief General update method that is called each frame
    /// @param time Total time elapsed in seconds
    /// @param deltaTime Time elapsed since the last frame
    /// @param input Input from mouse, keyboard and controllers
    void update(
        float time,
        float deltaTime,
        InputManagerPtr input) override;

    /// @brief For rendering of game contents
    /// @param time Total time elapsed in seconds
    /// @param screenWidth Current width of the window in pixels
    /// @param screenHeight Current height of the window in pixels
    void render(
        float time,
        int windowWidth,
        int windowHeight) override;

    /// @brief For destruction of game resources
    void destroy() override;



private:

    
    


    /// @brief For rendering of GUI elements
    void renderUI();

    // Renderer for rendering imported animated or non-animated models
    eeng::ForwardRendererPtr forwardRenderer;

    // Immediate-mode renderer for basic 2D or 3D primitives
    ShapeRendererPtr shapeRenderer;

    // Entity registry - to use in labs
    std::shared_ptr<entt::registry> entity_registry;

    // Stores meshes by string id
    std::shared_ptr<my_eeng::MeshManager> meshManager;

    // Matrices for view, projection and viewport
    struct Matrices
    {
        glm::mat4 V;
        glm::mat4 P;
        glm::mat4 VP;
        glm::ivec2 windowSize;
    } matrices;

   

    // Light properties
    struct PointLight
    {
        glm::vec3 pos;
        glm::vec3 color{ 1.0f, 1.0f, 0.8f };
    } pointlight;

   


    EventP::EventQueue event_dispatcher;




   

    entt::entity camera_entity;


    



    // UI toggles
    bool show_ModifyObjectUI = true;
    bool show_debugAnimations = false;


    // Stats
    int drawcallCount = 0;

    enum QuestLine
    {
        GetGrass,
        GoToHorse,
        FeedHorse,
        Done,
    };

    QuestLine questLineState = GetGrass;

    std::map < QuestLine, std::string> questlineTipsMap{
        {GetGrass,  "Make your way to to grass patch"},
        {GoToHorse, "Make your way to the horse"},
        {FeedHorse, "Press E to feed the horse"},
        {Done,      "Quest Complete"},
    };


    std::map<eeng::InputManager::Key, bool> keyIsPressedMap;

    void BuildGameObjects();




    
#pragma region Systems


    void Input_System(InputManagerPtr input, EventP::EventQueue& event_dispatcher);

    void velocity_System(float deltaTime);


    void player_System(float deltaTime,InputManagerPtr input);

    //void npc_System(float time);


    void LookAt_System(InputManagerPtr input);

    // does not work
    void RoateToDriection_System();


    void SB_Wander_System(float time);
    
    void SB_Separation_System();

    void SteeringBehavior_System(float deltaTime);

    




    void Animation_BySpeed_System(std::shared_ptr<entt::registry> entity_registry);
    void Animation_ByQuest_System(std::shared_ptr<entt::registry> entity_registry, int questStep);

    void imGui_Animation_Selector(std::shared_ptr<eeng::RenderableMesh> mesh, int& currentIndex, std::string title, std::string id);


    void imGui_WorldToScreen_System(std::shared_ptr<entt::registry> entity_registry);


    
    void imGui_W_Transform_System();
    void imGui_W_TextBox_System(std::shared_ptr<entt::registry> entity_registry);
    void imGui_W_Animation_Controller_System(std::shared_ptr<entt::registry> entity_registry);
    void imGui_W_Tag_System(std::shared_ptr<entt::registry> entity_registry);

    bool imGui_Prepare_WorldToScreen(glm::vec3 worldPos, glm::ivec2& screenPos_Out);

    void imGui_Base_WorldPositionUI(Transform_Component& transform, entt::entity& entity, const std::function<void()>& func);
    void imGui_Base_WorldPositionUI(entt::entity& entity, const std::function<void()>& func);


    void Transform_DebugView();

    void Velocity_DebugView();

#pragma endregion





    void updateSystems(float time,
        float deltaTime,
        InputManagerPtr input);

    void UI_Systems();

    void renderPassSystems(float time);

    

    float Get1DNoise(const float seed) {
        return glm::sin(2 * seed) + glm::cos(glm::pi<float>() * seed);
    }

    
    uint32_t BitShiftRandom(uint32_t seed) {
        seed ^= seed << 17;
        seed ^= seed >> 13;
        seed ^= seed << 5;

        return seed;
    }
};

#endif
