#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Graphics
{
    struct CameraComponent
    {
    public:
        CameraComponent();
        ~CameraComponent() = default;

        std::string m_name { "Unnamed Camera" };

        int id { -1 };

        glm::mat4 m_viewMatrix { 0.f };
        glm::mat4 m_projectionMatrix { 0.f };

        glm::vec3 m_direction { 0.f };
        glm::vec3 m_up { 0.f, 1.f, 0.f };

        bool m_dirtyViewMatrix { true };
        bool m_dirtyProjectionMatrix { true };

        float m_viewPortWidth { 800 };
        float m_viewPortHeight { 600 };
        float m_nearPlane { 0.1f };
        float m_farPlane { 100.f };
        float m_fov { 45.f };

        double m_cameraSpeed { 1.f };
    };
}
