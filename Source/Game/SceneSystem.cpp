#include "Precompiled.hpp"
#include "Game/SceneSystem.hpp"
#include "Game/Scene.hpp"
using namespace Game;

SceneSystem::SceneSystem() :
    m_engine(nullptr),
    m_scene(nullptr),
    m_initialized(false)
{
}

SceneSystem::~SceneSystem()
{
}

SceneSystem::SceneSystem(SceneSystem&& other) :
    SceneSystem()
{
    // Call the move assignment.
    *this = std::move(other);
}

SceneSystem& SceneSystem::operator=(SceneSystem&& other)
{
    // Swap class members.
    std::swap(m_engine, other.m_engine);
    std::swap(m_scene, other.m_scene);
    std::swap(m_initialized, other.m_initialized);

    return *this;
}

bool SceneSystem::Initialize(Engine::Root* engine)
{
    LOG() << "Initializing scene system...";

    // Make sure that this instance has not been initialized yet.
    VERIFY(!m_initialized, "Scene system has already been initialized!");

    // Validates arguments.
    if(engine == nullptr)
    {
        LOG_ERROR() << "Invalid argument - \"engine\" is null!";
        return false;
    }

    // Save engine reference.
    m_engine = engine;

    // Success!
    return m_initialized = true;
}

void SceneSystem::ChangeScene(std::shared_ptr<Scene> scene)
{
    ASSERT(m_initialized, "Scene system has not been initialized yet!");

    // Notify previous scene about the change.
    if(m_scene)
    {
        m_scene->OnSceneExit();
    }
    
    // Change the current scene.
    m_scene = scene;

    // Notify new scene about the change.
    if(m_scene)
    {
        m_scene->OnSceneEnter();
    }
}

void SceneSystem::UpdateScene(float timeDelta)
{
    ASSERT(m_initialized, "Scene system has not been initialized yet!");

    // Update the current scene.
    if(m_scene)
    {
        m_scene->OnUpdate(timeDelta);
    }
}

void SceneSystem::DrawScene(float timeAlpha)
{
    ASSERT(m_initialized, "Scene system has not been initialized yet!");

    // Draw the current scene.
    if(m_scene)
    {
        m_scene->OnDraw(timeAlpha);
    }
}
