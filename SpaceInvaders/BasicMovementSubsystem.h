#pragma once

#include "Core/Subsystem.h"

#include "glm/glm.hpp"

namespace Graphics
{
    class Window;
}

namespace Game
{
    class BasicMovementSubsystem : public Core::Subsystem
    {
    public:
        BasicMovementSubsystem( entt::registry & registry );

        void CreateComponents( Graphics::Window & window );

        struct MovementStruct
        {
            float m_angle = 0.f;
            float speed = 1.f;
            glm::vec2 m_xy {0.f, 0.f};
        };
        // Inherited via Subsystem
        virtual void FixedUpdate( double deltaTime ) override;

    private:
        void Integrate( double deltaTime );

        MovementStruct m_movementStruct;
        entt::entity m_entity;
    };
}