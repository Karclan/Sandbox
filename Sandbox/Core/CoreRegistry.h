#pragma once

#include "entt/entt.hpp"
#include "entt/entity/storage.hpp"

#include <bitset>
#include <type_traits>


using namespace entt::literals;

#include "../Generic/NameComponent.h"

#if WITH_EDITOR

#include "../Editor/ComponentManifestComponent.h"

#endif


namespace Core
{
	using Entity = entt::entity;
	constexpr auto NullEntity = entt::null;

	class Registry
	{
	public:
		Registry();
		~Registry();

		//////////////////////////////////////////////////////////////////////////
		//Creation
		//////////////////////////////////////////////////////////////////////////
		[[nodiscard]] Entity CreateEntity()
		{
			return m_registry.create();
		}

		[[nodiscard]] Entity CreateEntity( const char * name )
		{
			auto entity = m_registry.create();
			AddComponent<Generic::NameComponent>( entity, name );
			return entity;
		}

		[[nodiscard]] Entity CreateEntity( std::string && name )
		{
			auto entity = m_registry.create();
			AddComponent<Generic::NameComponent>( entity, name );
			return entity;
		}

		[[nodiscard]] Entity CreateEntity( Entity const hint )
		{
			return m_registry.create( hint );	
		}

		template<typename It>
		[[nodiscard]] Entity CreateEntity( It first, It last )
		{
			return m_registry.create( first, last );		
		}

		bool IsValidEntity( Entity const entity ) const
		{
			return m_registry.valid( entity );
		}

		//////////////////////////////////////////////////////////////////////////
		//Deletion
		//////////////////////////////////////////////////////////////////////////
		void DestroyEntity( Entity const entity )
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
			return m_registry.emplace<Component>( entity, std::forward<Args...>(args)... );
		}

		template<typename Component, typename It>
		std::enable_if_t<std::is_same_v<typename std::iterator_traits<It>::value_type, Entity>, void>
			AddComponent( It first, It last, const Component& value = {} )
		{
			return m_registry.emplace<Component>( first, last, value );
		}

		template<typename Component, typename EIt, typename CIt>
		std::enable_if_t<std::is_same_v<typename std::iterator_traits<EIt>::value_type, Entity>, void>
			AddComponent( EIt first, EIt last, CIt value )
		{
			return m_registry.emplace<Component>( first, last, value );
		}

		template<typename It>
		void AddComponent( It first, It last )
		{
			return m_registry.emplace( first, last );
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
		void TryRemoveComponent( Entity const entity )
		{
			if ( m_registry.all_of<Component...>( entity ) )
			{
				m_registry.remove<Component...>( entity );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//Has Components
		//////////////////////////////////////////////////////////////////////////
		template<typename ...Component>
		bool HasComponent( Entity const entity ) const
		{
			return m_registry.all_of<Component ...>( entity );
		}

		template<typename... Component>
		bool HasAnyComponent( Entity const entity ) const
		{
			return m_registry.any_of<Component ...>( entity );
		}

		//template <typename ...Component>
		//std::bitset<sizeof...( Component )> GetComponentBitset( Entity const entity )
		//{
		//	using List = entt::type_list<Component ...>;
		//	
		//	std::bitset<sizeof...( Component )> bitset = 0;
		//	size_t index = 0;
		//	Utils::forEach<List>( [&bitset, &index, &reg, entity] ( auto type ) {
		//		using Component = UTILS_TYPE( type );
		//		bitset[ index ] = reg.has<Component>( entity );
		//		++index;
		//						  } );
		//	return bitset;
		//}

		//////////////////////////////////////////////////////////////////////////
		//Get Components
		//////////////////////////////////////////////////////////////////////////
		template<typename... Component>
		[[nodiscard]] decltype( auto ) GetConstComponent( [[maybe_unused]] Entity const entity ) const {
			return m_registry.get<Component ...>( entity );
		}

		template<typename... Component>
		[[nodiscard]] decltype( auto ) GetComponent( [[maybe_unused]] Entity const entity ) {
			return m_registry.get<Component ...>( entity );
		}

		template<typename... Component>
		[[nodiscard]] auto TryGetConstComponent( [[maybe_unused]] Entity const entity ) const {
			return m_registry.try_get<Component ...>( entity );
		}

		template<typename... Component>
		[[nodiscard]] auto TryGetComponent( [[maybe_unused]] Entity const entity ) {
			return m_registry.try_get<Component ...>( entity );
		}


		//////////////////////////////////////////////////////////////////////////
		//Views
		//////////////////////////////////////////////////////////////////////////

		//template<typename Type, typename... Other, typename... Exclude>
		//[[nodiscard]] basic_view<get_t<storage_for_type<Type>, storage_for_type<Other>...>, exclude_t<storage_for_type<Exclude>...>>

		/*! @copydoc view */
		template<typename Component, typename... Other, typename... Exclude>
		[[nodiscard]] entt::basic_view<entt::get_t<entt::basic_registry<>::storage_for_type<Component>, entt::basic_registry<>::storage_for_type<Other>...>, entt::exclude_t<entt::basic_registry<>::storage_for_type<Exclude>...>> 
			View( entt::exclude_t<Exclude...> = entt::exclude_t{} ) {
			return m_registry.view<Component, Other...>( entt::exclude<Exclude>... );
		}

		
		template<typename Component, typename... Other, typename... Exclude>
		[[nodiscard]] entt::basic_view<entt::get_t<entt::basic_registry<>::storage_for_type<const Component>, entt::basic_registry<>::storage_for_type<const Other>...>, entt::exclude_t<entt::basic_registry<>::storage_for_type<const Exclude>...>>
			View( entt::exclude_t<Exclude...> = entt::exclude_t {} ) const {
			return m_registry.view<Component, Other...>( entt::exclude<Exclude>... );
		}

		//////////////////////////////////////////////////////////////////////////
		//For Each
		//////////////////////////////////////////////////////////////////////////
		template<typename Component, typename... Other, typename... Exclude, typename Func>
		void ForView( Func func, entt::exclude_t<Exclude>... )
		{
			auto view = View<Component, Other...>( entt::exclude<Exclude>... );
			view.each( func );
		}

		template<typename Component, typename... Other, typename... Exclude, typename Func>
		void ForConstView( Func func, entt::exclude_t<Exclude>... ) const
		{
			auto const view = View<Component, Other...>( entt::exclude<Exclude>... );
			view.each( func );
		}

		template<typename Func>
		void Each( Func func ) const
		{
			m_registry.each( func );
		}

		//////////////////////////////////////////////////////////////////////////
		//Event listening
		//////////////////////////////////////////////////////////////////////////
		template<typename Component>
		[[nodiscard]] auto on_construct()
		{
			return m_registry.on_construct<Component>();
		}

		//////////////////////////////////////////////////////////////////////////
		//Context components
		//////////////////////////////////////////////////////////////////////////
		template<typename Type>
		[[nodiscard]] std::add_const_t<Type> & GetConstContextComponent() const
		{
			return m_registry.ctx().get<Type>();
		}

		template<typename Type>
		[[nodiscard]] Type & GetContextComponent()
		{
			return m_registry.ctx().get<Type>();
		}

		template<typename Type, typename... Args>
		Type & SetContextComponent( Args &... args )
		{
			return m_registry.ctx().insert_or_assign<Type>(std::forward<Args>(args)...);
		}

		//template<typename Type>
		//void UnsetContextComponent()
		//{
		//	m_registry.unset<Type>();
		//}

		/*template<typename Type, typename... Args>
		[[nodiscard]] Type & GetContextComponentOrSet( Args &&... args )
		{
			return m_registry.ctx_or_set<Type>( std::forward<Args>( args )... );
		}*/

		//template<typename Type>
		//[[nodiscard]] std::add_const_t<Type> * TryGetConstContextComponent() const
		//{
		//	return m_registry.try_ctx<Type>();
		//}

	/*	template<typename Type>
		[[nodiscard]] Type * TryGetContextComponent()
		{
			return m_registry.try_ctx<Type>();
		}*/

		//entt::registry & get_base_registry()
		//{
		//	return m_registry;
		//}

		[[nodiscard]] auto GetStorage() noexcept
		{
			return m_registry.storage();
		}

		[[nodiscard]] auto GetStorage() const noexcept
		{
			return m_registry.storage();
		}

		
		template<typename Type>
		decltype( auto ) GetStorage( const entt::id_type id = entt::type_hash<Type>::value() )
		{
			return m_registry.storage<Type>( id );
		}

		
		template<typename Type>
		decltype( auto ) GetStorage( const entt::id_type id = entt::type_hash<Type>::value() ) const
		{
			return m_registry.storage<Type>( id );
		}

	private:
		entt::registry m_registry;

	};
}