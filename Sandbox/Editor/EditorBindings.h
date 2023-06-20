#pragma once

#include "../Input/DeviceBindings.h"

namespace Editor
{
	class EditorMouseBindings : public Input::TDeviceBindings < Input::DeviceId( 0, { Input::DeviceType::Mouse } ), "Editor Mouse Bindings" >
	{
	public:
		void SetupBindings() final;
	};

	class EditorKeyboardBindings : public Input::TDeviceBindings < Input::DeviceId( 0, { Input::DeviceType::Keyboard } ), "Editor Keyboard Bindings" >
	{
	public:
		void SetupBindings() final;
	};
}