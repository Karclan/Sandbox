#pragma once

#include "../Core/Subsystem.h"

#include "entt/core/type_info.hpp"

#include <map>
#include <functional>
#include <utility>

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
			RegisterComponentDetailsInternal( std::in_place_type_t<Component>, std::forward( Delegate ) );
		}

		void BuildWindow( double deltaTime ) override;

	private:

		void RegisterComponentDetailsInternal(entt::type_info, std::function<void()> && Delegate);

		Core::Entity m_InspectedEntity = Core::NullEntity;

		struct InfoDelegatePair
		{
			entt::type_info Info;
			std::function<void()> Delegate;
		};

		using DelegateMap = std::map<entt::id_type, InfoDelegatePair>;
		DelegateMap m_DetailsViewDelegates;
	};
}

