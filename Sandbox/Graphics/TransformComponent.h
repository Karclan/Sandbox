#pragma once

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Graphics
{
	namespace Constants
	{
		static constexpr glm::vec3 Right{ 1.f,0.f,0.f };
		static constexpr glm::vec3 Up{ 0.f,1.f,0.f };
		static constexpr glm::vec3 Forward{ 0.f,0.f,1.f };
	}

	struct TransformComponent
	{
		glm::vec3 Position{ 0.f };
		glm::vec3 Scale{ 1.f };
		glm::quat RotationQuat = glm::quat();
		glm::vec3 Rotation{ 0.f };
		glm::vec3 Up{ 0.f, 1.f, 0.f };
		glm::vec3 Forward{ 0.f, 0.f, -1.f };
		glm::vec3 Right{ 1.f, 0.f, 0.f };
	};
}
