#pragma once

#include "../Core/Subsystem.h"

namespace Editor
{
	class EditorInspector : public Core::ImGuiWindowBuilder
	{
	public:
		void BuildWindow( double deltaTime ) override;

	};
}

