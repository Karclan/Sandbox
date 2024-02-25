#pragma once

#include <glm/glm.hpp>
#include <string>

#include "entt/core/type_traits.hpp"
#include "entt/core/hashed_string.hpp"

namespace Graphics
{
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

		glm::vec3 m_up { 0.f, 1.f, 0.f };
		glm::vec3 m_target { 0.f, 0.f, -1.f }; //We're looking into the negative Z direction by default
		//glm::vec3 m_direction { 0.f }; //Should really be a calculated unit vector from our position to the target. Might help with visualization

		bool m_orbit { true }; //Should be removed and put into a behaviour component

		bool m_dirtyViewMatrix { true };
		bool m_dirtyProjectionMatrix { true };

		float m_viewPortWidth { 800 };
		float m_viewPortHeight { 600 };

		float m_aspectRatio = m_viewPortHeight / m_viewPortWidth;
		
		float m_orthoAspectScale = 1.f;

		float m_nearPlane { 0.1f };
		float m_farPlane { 100.f };
		float m_fov { 45.f };

		double m_cameraSpeed { 1.f };

		ProjectionType m_projectionType = {ProjectionType::Perspective};
	};
}
