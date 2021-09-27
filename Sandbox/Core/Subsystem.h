#pragma once

#include "entt/entt.hpp"
#include <array>

namespace Graphics
{
	class Window;
}

namespace Core
{
	class Subsystem;
	//A subsystem is created and registered to the game to have a fixed update order
	class Subsystem
	{
	public:
		Subsystem( entt::registry& registry ) : m_registry( registry )
		{
		}
		virtual ~Subsystem() = default;

		virtual void FixedUpdate( double deltaTime ) = 0;
		virtual void Update( double deltaTime ) {};
	protected:
		entt::registry& m_registry;
	};

	class ImGuiWindowBuilder
	{
	public:
		ImGuiWindowBuilder( entt::registry& registry ) : m_registry( registry )
		{
		}
		virtual void BuildWindow( double deltaTime ) = 0;
	protected:
		entt::registry& m_registry;
	};


	//Creates menu bar and some windows for base information
	class BaseImGuiWindowBuilder final : public ImGuiWindowBuilder
	{
	public:
		BaseImGuiWindowBuilder( entt::registry& registry, Graphics::Window const& windowRef );

		void GetInfo();
		void BuildWindow( double deltaTime ) override;

	private:

		std::string m_vendorName {};
		std::string m_modelString {};
		bool m_showGraphicsStats { false };
		bool m_showUpdateStats { false };

		Graphics::Window const& m_windowRef;
		static constexpr size_t m_frameTimeSize = 90;
		/*std::array<float, m_frameTimeSize> m_pastFrameTimes;
		int m_pastFrameIndex = 0;*/
		bool m_showDemoWindow { true };
	};
}