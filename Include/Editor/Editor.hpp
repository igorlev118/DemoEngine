#pragma once

#include "Common/NonCopyable.hpp"
#include "Events/Receiver.hpp"
#include "System/Window.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/InputLayout.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

/*
    Editor

    Displays and handles built in editor UI system.
*/

namespace Engine
{
    class Editor : private NonCopyable
    {
    public:
        Editor(Graphics::Context* graphics);
        ~Editor();

        // Initializes the editor system.
        bool Initialize(System::Window* window);

        // Updates the editor interface.
        void Update(float deltaTime);

        // Draws the editor interface.
        void Draw();

    private:
        // Callback function for cursor position events.
        void CursorPositionCallback(const System::Window::Events::CursorPosition& event);

        // Callback function for mouse button events.
        void MouseButtonCallback(const System::Window::Events::MouseButton& event);

        // Callback function for mouse scroll events.
        void MouseScrollCallback(const System::Window::Events::MouseScroll& event);

        // Callback function for keyboard key events.
        void KeyboardKeyCallback(const System::Window::Events::KeyboardKey& event);

        // Callback function for text input events.
        void TextInputCallback(const System::Window::Events::TextInput& event);

    private:
        // Destroys the user interface context.
        void DestroyContext();

    private:
        // Graphics context.
        Graphics::Context* m_graphics;

        // User interface context.
        ImGuiContext* m_interface;

        // Window reference.
        System::Window* m_window;

        // Window event callbacks.
        Common::Receiver<void(const System::Window::Events::CursorPosition&)> m_receiverCursorPosition;
        Common::Receiver<void(const System::Window::Events::MouseButton&)> m_receiverMouseButton;
        Common::Receiver<void(const System::Window::Events::MouseScroll&)> m_receiverMouseScroll;
        Common::Receiver<void(const System::Window::Events::KeyboardKey&)> m_receiverKeyboardKey;
        Common::Receiver<void(const System::Window::Events::TextInput&)> m_receiverTextInput;

        // User interface rendering.
        Graphics::VertexBuffer m_vertexBuffer;
        Graphics::IndexBuffer m_indexBuffer;
        Graphics::InputLayout m_inputLayout;
        Graphics::Texture m_fontTexture;
        Graphics::Shader m_shader;

        // Initialization state.
        bool m_initialized;
    };
}
