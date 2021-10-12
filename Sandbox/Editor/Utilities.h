#pragma once

#include "../Core/CoreRegistry.h"

namespace Core
{
	class Registry;
}

namespace Graphics
{
	class Renderer;
}

namespace Editor
{
	class Utilities
	{
	public:
		//Need to create the editor camera
		//Need to create the editor floor shader
		void CreateEditorCamera( Core::Registry & Registry );
		void CreateGridFloor( Core::Registry& Registry, Graphics::Renderer & Renderer );

	private:
		Core::Entity m_editorCameraEntity { Core::NullEntity };
		Core::Entity m_gridFloorEntity { Core::NullEntity };

	};
}

