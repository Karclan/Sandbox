#pragma once

#include <glm/glm.hpp>
#include <string>

#include "entt/core/type_traits.hpp"
#include "entt/core/hashed_string.hpp"

namespace Graphics
{
	using namespace entt::literals;
	using ActiveCameraTag = entt::tag<"ActiveCamera"_hs>;


	enum class ProjectionType : uint8_t
	{
		Orthographic,
		Perspective,
	};

	struct CameraComponent
	{
	public:

		glm::mat4 m_viewMatrix { 0.f };
		glm::mat4 m_projectionMatrix { 0.f };

		glm::vec3 m_direction { 0.f };
		glm::vec3 m_up { 0.f, 1.f, 0.f };
		glm::vec3 m_target { 0.f, 0.f, -1.f };

		bool m_orbit { true };

		bool m_dirtyViewMatrix { true };
		bool m_dirtyProjectionMatrix { true };

		float m_viewPortWidth { 800 };
		float m_viewPortHeight { 600 };

		
		float m_orthoLeft{ 0 };
		float m_orthoRight { 600 };
		float m_orthoBottom { 800 };
		float m_orthoTop { 0 };

		float m_nearPlane { 0.1f };
		float m_farPlane { 100.f };
		float m_fov { 45.f };

		double m_cameraSpeed { 1.f };

		ProjectionType m_projectionType = {ProjectionType::Perspective};
	};
}
