#pragma once

#include "../Core/Subsystem.h"

#include "ComponentDetails/ComponentDetailsBase.h"
#include "ComponentDetails/GenericTagsDetails.h"

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
		
		template<typename ComponentDetails>
		void RegisterNameComponentDetailsView( std::unique_ptr<ComponentDetails> && detailsClass )
		{
			m_NameComponentDetails = { std::in_place_type_t<ComponentDetails::HandledType>() , m_ComponentDetailsViewDelegates.size(), std::move( detailsClass ) };
		}

		template<typename TagDetails>
		void RegisterTagDetailsView( std::unique_ptr<TagDetails> && detailsClass )
		{
			RegisterTagDetailsInternal( std::in_place_type_t<TagDetails::HandledType>(), std::move( detailsClass ) );
		}

		void BuildWindow( double deltaTime ) override;

	private:
		void RegisterComponentDetailsInternal(entt::type_info, std::unique_ptr<ComponentDetails> && detailsClass );
		void RegisterTagDetailsInternal( entt::type_info, std::unique_ptr<TagDetails> && detailsClass );

		struct ComponentDetailsDelegatePair
		{
			entt::type_info Info = entt::type_id<int>();
			size_t Order = 0;
			std::unique_ptr<ComponentDetails> DetailsClass = nullptr;
		};

		using ComponentDetailsDelegateMap = std::map<entt::id_type, ComponentDetailsDelegatePair>;
		ComponentDetailsDelegateMap m_ComponentDetailsViewDelegates;
		ComponentDetailsDelegatePair m_NameComponentDetails;//Special case to ensure this is above all else
		
		struct TagDelegatePair
		{
			entt::type_info Info;
			std::unique_ptr<TagDetails> DetailsClass;
		};

		using TagDetailsDelegateMap = std::map<entt::id_type, TagDelegatePair>;
		TagDetailsDelegateMap m_TagDetailsViewDelegates;
	};
}

