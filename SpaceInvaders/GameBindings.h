#pragma once

#include "Input/DeviceBindings.h"

namespace Game
{
	class GameMouseBindings : public Input::TDeviceBindings < Input::DeviceId( 1, { Input::DeviceType::Mouse } ), "Game Mouse Bindings" >
	{
	public:
		void SetupBindings() final;
	};

	class GameKeyboardBindings : public Input::TDeviceBindings < Input::DeviceId( 0, { Input::DeviceType::Keyboard } ), "Game Keyboard Bindings" >
	{
	public:
		void SetupBindings() final;
	};
}
