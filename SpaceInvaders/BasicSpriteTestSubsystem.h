#pragma once

#include "Core/Subsystem.h"

#include "glm/glm.hpp"

namespace Graphics
{
    class Window;
}

namespace Game
{
    class BasicSpriteTestSubsystem final : public Core::Subsystem, public Core::ImGuiWindowBuilder
    {
    public:
        BasicSpriteTestSubsystem( entt::registry & registryRef, Graphics::Window* window );
        virtual ~BasicSpriteTestSubsystem();

        void CreateComponents( Graphics::Window & windowRef );
        
        // Inherited via Subsystem
        void FixedUpdate( double deltaTime ) override;
        void Update( double deltaTime ) override;
        void BuildWindow( double deltaTime ) override;
    private:
        Graphics::Window * m_windowPtr;
        glm::vec3 AStart, AEnd;
        glm::vec3 BStart, BEnd;

        bool m_showWindow { false };
        entt::entity m_entity;
    };
}