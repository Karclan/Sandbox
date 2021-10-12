#pragma once

#include "CoreRegistry.h"

#include <array>

namespace Graphics
{
	class Window;
}

namespace Core
{
	class Registry;

	class Subsystem;
	//A subsystem is created and registered to the game to have a fixed update order
	class Subsystem
	{
	public:
		Subsystem( Core::Registry& registry ) : m_registry( registry )
		{
		}
		virtual ~Subsystem() = default;

		virtual void FixedUpdate( double deltaTime ) {};
		virtual void Update( double deltaTime ) {};
	protected:
		Core::Registry& m_registry;
	};

	class ImGuiWindowBuilder
	{
	public:
		virtual void BuildWindow( double deltaTime ) = 0;

	protected:

	};


	//Creates menu bar and some windows for base information
	class BaseImGuiWindowBuilder final : public Subsystem, public ImGuiWindowBuilder
	{
	public:
		BaseImGuiWindowBuilder( Core::Registry& registry, Graphics::Window const& windowRef );

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