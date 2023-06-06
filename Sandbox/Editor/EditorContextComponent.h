#pragma once

#include "../Core/CoreRegistry.h"

namespace Editor
{
	//Component to hold various bits of state for the editor such as currently selected entity, current scene, camera, other glue data vals
	struct EditorContextComponent
	{
		Core::Entity m_InspectedEntity = Core::NullEntity;
	};
}