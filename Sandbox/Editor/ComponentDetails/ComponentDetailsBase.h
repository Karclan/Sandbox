#pragma once

#include "entt/core/type_info.hpp"

namespace Editor
{
	class ComponentDetails
	{
	public:
		constexpr ComponentDetails() = default;
		virtual ~ComponentDetails() = default;
		
		virtual constexpr entt::type_info GetHandledType() const = 0;

		virtual void BuildDetailsWindow( void * componentPtr ) const = 0;
	};

	template<typename Component>
	class TComponentDetails : public ComponentDetails
	{
	public:
		using HandledType = Component;

		constexpr TComponentDetails() = default;
		virtual ~TComponentDetails() = default;

		constexpr entt::type_info GetHandledType() const override { return m_HandledType; }

		void BuildDetailsWindow( void * componentPtr ) const override
		{
			if ( componentPtr )
			{
				Component * castedComponent = static_cast< Component * >( componentPtr );
				BuildDetailsInternal( *castedComponent );
			}
		}

	protected:
		virtual void BuildDetailsInternal( Component & inComponent ) const = 0;

		//for some reason can't be constexpr??
		const entt::type_info m_HandledType { entt::type_id<Component>() };
	};
}
