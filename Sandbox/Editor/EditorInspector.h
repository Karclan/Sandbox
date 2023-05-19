#pragma once

#include "../Core/Subsystem.h"

#include "entt/core/type_info.hpp"

#include <map>
#include <functional>

namespace Editor
{
	class EditorInspector : public Core::ImGuiWindowBuilder, public Core::Subsystem
	{
	public:
		EditorInspector( Core::Registry & registry );

		//Add a delegate for viewing a given component's data
		template<typename Component>
		void RegisterComponentDetailsView( std::function<void()> && Delegate )
		{
			RegisterComponentDetailsInternal( entt::type_info<Component>, std::forward( Delegate ) );
		}

		void BuildWindow( double deltaTime ) override;

	private:

		void RegisterComponentDetailsInternal(entt::type_info, std::function<void()> && Delegate);

		Core::Entity m_InspectedEntity = Core::NullEntity;

		std::map<entt::type_info, std::function<void()>> m_DetailsViewDelegates;
	};
}

