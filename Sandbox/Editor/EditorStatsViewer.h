#pragma once
#include "../Core/Subsystem.h"

namespace Editor
{
	//Stats viewer window - useful for seeing the contents of various context components
	class EditorStatsViewer : public Core::ImGuiWindowBuilder, public Core::Subsystem
	{
	public:
		EditorStatsViewer( Core::Registry & registry );

		void BuildWindow( double deltaTime ) override;
	private:
		int bob = 0;
	};
}
