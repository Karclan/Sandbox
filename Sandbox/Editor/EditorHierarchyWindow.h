#pragma once

#include "../Core/Subsystem.h"

namespace Editor
{
	class EditorHierarchyWindow : public Core::ImGuiWindowBuilder, public Core::Subsystem
	{
	public:
		EditorHierarchyWindow( Core::Registry & registry );
		void BuildWindow( double deltaTime ) override;

	private:
		Core::Entity m_selectedEntity = Core::NullEntity;

	};
}
