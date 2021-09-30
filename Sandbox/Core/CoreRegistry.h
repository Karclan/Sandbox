#pragma once

#include "entt/entt.hpp"

namespace Core
{
	using Entity = entt::entity;

	class Registry
	{
	public:
		Registry();
		~Registry();

		//////////////////////////////////////////////////////////////////////////
		//Creation
		//////////////////////////////////////////////////////////////////////////
		Entity CreateEntity()
		{
			return m_registry.create();
		}

		Entity CreateEntity( Entity const hint )
		{
			return m_registry.create( hint );
		}

		template<typename It>
		Entity CreateEntity( It first, It last )
		{
			return m_registry.create( first, last );
		}

		//////////////////////////////////////////////////////////////////////////
		//Deletion
		//////////////////////////////////////////////////////////////////////////
		void destroy( Entity const entity )
		{
			m_registry.destroy( entity );
		}

		template<typename It>
		void DestroyEntity( It first, It last )
		{
			m_registry.destroy( first, last );
		}

		//////////////////////////////////////////////////////////////////////////
		//Assign Components
		//////////////////////////////////////////////////////////////////////////
		template<typename Component, typename... Args>
		decltype( auto ) AddComponent( Entity const entity, Args &&... args )
		{
			return m_registry.assign<Component>( entity, args ... );
		}

		template<typename Component, typename It>
		std::enable_if_t<std::is_same_v<typename std::iterator_traits<It>::value_type, Entity>, void>
			AddComponent( It first, It last, const Component& value = {} )
		{
			return m_registry.assign<Component>( first, last, value );
		}

		template<typename Component, typename EIt, typename CIt>
		std::enable_if_t<std::is_same_v<typename std::iterator_traits<EIt>::value_type, Entity>, void>
			AddComponent( EIt first, EIt last, CIt value )
		{
			return m_registry.assign<Component>( first, last, value );
		}

		template<typename It>
		void AddComponent( It first, It last )
		{
			return m_registry.assign( first, last );
		}

		//////////////////////////////////////////////////////////////////////////
		//Replace Components
		//////////////////////////////////////////////////////////////////////////
		template<typename Component, typename... Func>
		auto Replace( Entity const entity, Func &&... func ) -> decltype( std::enable_if_t<sizeof...( Func ) != 0>(), ( func( std::declval<Component&>() ), ... ), void() )
		{
			return m_registry.replace<Component...>( entity, func );
		}

		//////////////////////////////////////////////////////////////////////////
		//Remove Components
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component>
		void RemoveComponent( Entity const entity )
		{
			m_registry.remove<Component...>( entity );
		}

		template<typename... Component, typename It>
		void RemoveComponent( It first, It last )
		{
			m_registry.remove<Component...>( first, last );
		}

		template<typename... Component>
		void RemoveComponentIfExists( Entity const entity )
		{
			m_registry.remove_if_exists<Component...>( entity );
		}

		//////////////////////////////////////////////////////////////////////////
		//Has Components
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component>
		bool HasComponent( Entity const entity ) const
		{
			return m_registry.has<Component...>( entity );
		}

		template<typename... Component>
		bool HasAnyComponent( Entity const entity ) const
		{
			return m_registry.any<Component ...>( entity );
		}

		//////////////////////////////////////////////////////////////////////////
		//Get Components
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component>
		decltype( auto ) GetComponent( [[maybe_unused]] Entity const entity ) const {
			return m_registry.get<Component ...>( entity );
		}

		template<typename... Component>
		decltype( auto ) GetComponent( [[maybe_unused]] Entity const entity ) {
			return m_registry.get<Component ...>( entity );
		}

		template<typename... Component>
		auto TryGetComponent( [[maybe_unused]] Entity const entity ) const {
			return m_registry.try_get<Component ...>( entity );
		}

		template<typename... Component>
		auto TryGetComponent( [[maybe_unused]] Entity const entity ) {
			return m_registry.try_get<Component ...>( entity );
		}

		//////////////////////////////////////////////////////////////////////////
		//Views
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component, typename... Exclude>
		entt::basic_view<Entity, entt::exclude_t<Exclude...>, Component...> View( entt::exclude_t<Exclude...> = {} ) {
			return m_registry.view<Component ...>( entt::exclude<Exclude>... );
		}

		/*! @copydoc view */
		template<typename... Component, typename... Exclude>
		entt::basic_view<Entity, entt::exclude_t<Exclude...>, Component...> View( entt::exclude_t<Exclude...> = {} ) const {
			return m_registry.view<Component ...>( entt::exclude<Exclude>... );
		}

		//////////////////////////////////////////////////////////////////////////
		//For Each
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component, typename... Exclude, typename Func>
		void ForView( Func func, entt::exclude_t<Exclude>... )
		{
			auto view = View<Component ...>( entt::exclude<Exclude>... );
			view.each( func );
		}

		template<typename... Component, typename... Exclude, typename Func>
		void ForConstView( Func func, entt::exclude_t<Exclude>... ) const
		{
			auto view = View<Component ...>( entt::exclude<Exclude>... );
			view.each( func );
		}


	private:
		entt::registry m_registry;

	};
}