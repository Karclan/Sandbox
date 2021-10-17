#pragma once

#include "Core/Subsystem.h"

#include "Core/CoreRegistry.h"

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
        BasicSpriteTestSubsystem( Core::Registry& registryRef, Graphics::Window& window );

        void CreateComponents( Graphics::Window & windowRef );
        
        // Inherited via Subsystem
        void FixedUpdate( double deltaTime ) noexcept override;
        void Update( double deltaTime ) noexcept override;
        void BuildWindow( double deltaTime ) override;
    private:
        Graphics::Window & m_windowRef;
        glm::vec3 AStart, AEnd;
        glm::vec3 BStart, BEnd;

        bool m_showWindow { false };
        Core::Entity m_entity {};

        glm::vec3 m_bobsEulers {0.f};
    };
}