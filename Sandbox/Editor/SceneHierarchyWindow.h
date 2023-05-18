#pragma once

#include "../Core/Subsystem.h"

namespace Editor
{
	class SceneHierarchyWindow : public Core::ImGuiWindowBuilder
	{
	public:
		void BuildWindow( double deltaTime ) override;


	private:

		Core::Entity SelectedEntity = Core::NullEntity;

	};
}

