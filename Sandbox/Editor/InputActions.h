#pragma once

#include "../Input/InputAction.h"

namespace Editor
{
	struct Actions
	{
		static Input::ActionId const CameraPanToggle;
		static Input::ActionId const CameraPanLeft;
		static Input::ActionId const CameraPanRight;
		static Input::ActionId const CameraPanUp;
		static Input::ActionId const CameraPanDown;
		static Input::ActionId const CameraPanZoomIn;
		static Input::ActionId const CameraPanZoomOut;

		static Input::ActionId const CameraOrbitToggle;
		static Input::ActionId const CameraOrbitLeft;
		static Input::ActionId const CameraOrbitRight;
		static Input::ActionId const CameraOrbitUp;
		static Input::ActionId const CameraOrbitDown;
		static Input::ActionId const CameraOrbitZoomIn;
		static Input::ActionId const CameraOrbitZoomOut;
	};

}