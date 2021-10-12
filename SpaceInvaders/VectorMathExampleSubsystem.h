#pragma once

#include "Core/Subsystem.h"

#include "glm/glm.hpp"

#include "entt/core/type_traits.hpp"

namespace Graphics
{
	class Window;
}

namespace Game
{
	class VectorMathExampleSubsystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		VectorMathExampleSubsystem( Core::Registry& registry );

		void CreateComponents( Graphics::Window& window );

		void FixedUpdate(double deltaTime) override;
		void Update(double deltaTime) override;

		void BuildWindow(double deltaTime) override;

	private:
		enum class EDirection
		{
			Left,
			Right,
			Up,
			Down,
			Unknown
		};

		entt::entity PointAEntity{ entt::null };
		entt::entity PointBEntity{ entt::null };
		entt::entity PointCEntity{ entt::null };
		glm::vec3 PointA{  0.f,  0.f,  0.f };
		glm::vec3 PointB{  2.f,  2.f,  0.f };
		glm::vec3 PointC{ -2.f, -2.f,  0.f };
		EDirection BDirection{ EDirection::Unknown };
		EDirection CDirection{ EDirection::Unknown };

		glm::vec3 PointAForward { 0.f };
		glm::vec3 PointARight { 0.f };

	
		using EntityTag = entt::tag<"VectorMaths"_hs>;
		
		bool bShowWindow{ false };

		Graphics::Window* m_window { nullptr };
	};
}
