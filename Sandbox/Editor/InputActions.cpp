#include "InputActions.h"

using namespace Input::Literals;

namespace Editor
{
	Input::ActionId const Actions::CameraPanToggle("Editor_Camera_Pan_Toggle"_ActionId);
	Input::ActionId const Actions::CameraPanLeft("Editor_Camera_Pan_Left"_ActionId);
	Input::ActionId const Actions::CameraPanRight("Editor_Camera_Pan_Right"_ActionId);
	Input::ActionId const Actions::CameraPanUp("Editor_Camera_Pan_Up"_ActionId);
	Input::ActionId const Actions::CameraPanDown("Editor_Camera_Pan_Down"_ActionId);
	Input::ActionId const Actions::CameraPanZoomIn("Editor_Camera_Pan_Zoom_In"_ActionId);
	Input::ActionId const Actions::CameraPanZoomOut("Editor_Camera_Pan_Zoom_Out"_ActionId);

	Input::ActionId const Actions::CameraOrbitToggle("Editor_Camera_Orbit_Toggle"_ActionId);
	Input::ActionId const Actions::CameraOrbitLeft("Editor_Camera_Orbit_Left"_ActionId);
	Input::ActionId const Actions::CameraOrbitRight("Editor_Camera_Orbit_Right"_ActionId);
	Input::ActionId const Actions::CameraOrbitUp("Editor_Camera_Orbit_Up"_ActionId);
	Input::ActionId const Actions::CameraOrbitDown("Editor_Camera_Orbit_Down"_ActionId);
	Input::ActionId const Actions::CameraOrbitZoomIn("Editor_Camera_Orbit_Zoom_In"_ActionId);
	Input::ActionId const Actions::CameraOrbitZoomOut("Editor_Camera_Orbit_Zoom_Out"_ActionId);
}