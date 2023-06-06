#pragma once

#include "../Core/Subsystem.h"

#include "ComponentDetails/ComponentDetailsBase.h"

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
		template<typename ComponentDetails>
		void RegisterComponentDetailsView( std::unique_ptr<ComponentDetails> && detailsClass )
		{
			RegisterComponentDetailsInternal( std::in_place_type_t<ComponentDetails::HandledType>(), std::move( detailsClass ) );
		}

		void BuildWindow( double deltaTime ) override;

	private:
		void RegisterComponentDetailsInternal(entt::type_info, std::unique_ptr<ComponentDetails> && detailsClass );

		struct InfoDelegatePair
		{
			entt::type_info Info;
			std::unique_ptr<ComponentDetails> DetailsClass;
		};

		using DelegateMap = std::map<entt::id_type, InfoDelegatePair>;
		DelegateMap m_DetailsViewDelegates;
	};
}

