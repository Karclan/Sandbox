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
		Subsystem(Core::Registry& registry) noexcept : m_registry(registry) {}
		Subsystem() = delete;
		Subsystem(Subsystem const& other) noexcept : m_registry(other.m_registry) {}
		Subsystem(Subsystem&& other) noexcept : m_registry(other.m_registry) {}
		Subsystem operator= (Subsystem const& other) = delete;
		Subsystem operator=(Subsystem&& other) = delete;
		virtual ~Subsystem() = default;

		virtual void FixedUpdate([[maybe_unused]] double deltaTime) noexcept { };
		virtual void Update([[maybe_unused]] double deltaTime) noexcept {};

	protected:
		Core::Registry& m_registry;
	};

	class ImGuiWindowBuilder
	{
	public:
		virtual void BuildWindow(double deltaTime) = 0;
	};

	//Creates menu bar and some windows for base information
	class BaseImGuiWindowBuilder final : public Subsystem, public ImGuiWindowBuilder
	{
	public:
		BaseImGuiWindowBuilder(Core::Registry& registry, Graphics::Window const& windowRef);

		void GetInfo();
		void BuildWindow(double deltaTime) override;

	private:
		std::string m_vendorName{};
		std::string m_modelString{};
		bool m_showGraphicsStats{ false };
		bool m_showUpdateStats{ false };

		Graphics::Window const& m_windowRef;
		static constexpr size_t m_frameTimeSize = 90;
		/*std::array<float, m_frameTimeSize> m_pastFrameTimes;
		int m_pastFrameIndex = 0;*/
		bool m_showDemoWindow{ true };
	};
}