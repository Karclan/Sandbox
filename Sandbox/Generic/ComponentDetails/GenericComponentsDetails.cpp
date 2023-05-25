#include "GenericComponentsDetails.h"

#include "../Graphics/CameraComponent.h"

#include "imgui.h"
#include "../Imgui/misc/cpp/imgui_stdlib.h"

namespace Editor
{
	void NameComponentDetails::BuildDetailsInternal( Generic::NameComponent & inComponent ) const
	{
		ImGui::InputText( "Name", &inComponent.Name );
	}
}
