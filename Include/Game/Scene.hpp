#pragma once

/*
    Scene
*/

namespace Game
{
    // Scene base class.
    class Scene
    {
    protected:
        // Protected constructor.
        Scene()
        {
        }

    public:
        // Virtual destructor.
        virtual ~Scene()
        {
        }

        // Called when the scene is about to enter.
        virtual void OnSceneEnter()
        {
        }

        // Called when the scene is about to exit.
        virtual void OnSceneExit()
        {
        }

        // Called when the scene need to be updated.
        virtual void OnUpdate(float timeDelta)
        {
        }

        // Called when the scene needs to be drawn.
        virtual void OnDraw(float timeAlpha)
        {
        }
    };
}
