#pragma once

#include "../Input/DeviceBindings.h"

namespace Editor
{
	class EditorMouseBindings : public Input::DeviceBindings
	{
	public:
		EditorMouseBindings() noexcept;
		virtual ~EditorMouseBindings() = default;

		void SetupBindings() override;
	};

	class EditorKeyboardBindings : public Input::DeviceBindings
	{
	public:
		EditorKeyboardBindings() noexcept;
		virtual ~EditorKeyboardBindings() = default;

		void SetupBindings() override;
	};
}