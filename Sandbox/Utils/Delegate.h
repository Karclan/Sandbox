#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace  Utils
{
	//Taken from https://codereview.stackexchange.com/questions/36251/c-delegate-implementation-with-member-functions
	//And http://coliru.stacked-crooked.com/a/55858bdee7561e79
	template <typename... Params>
	class Delegate1 {
	private:
		typedef std::function<void( Params... )> FunctionType;

		struct Key {
			void* m_object;
			size_t m_funcHash;

			bool operator==( const Key& other ) const { return tuple() == other.tuple(); }
			bool operator< ( const Key& other ) const { return tuple() < other.tuple(); }
		private:
			std::tuple<void* const&, size_t const&> tuple() const { return std::tie( m_object, m_funcHash ); }
		};

		std::multimap<Key, FunctionType> m_functions;

	public:
		template <typename Class>
		void connect( Class& obj, void ( Class::* func )( Params... ) ) {
			m_functions.insert( { Key {&obj, getHash( func )}, [&obj, func] ( Params... params ) { ( obj.*func )( params... ); } } );
		}

		template <typename Class>
		size_t getHash( void ( Class::* func )( Params... ) ) {
			const char* ptrptr = static_cast< const char* >( static_cast< const void* >( &func ) );
			int const size = sizeof( func );
			std::string str_rep( ptrptr, size );
			std::hash<std::string> strHasher;
			return strHasher( str_rep );
		}

		template <typename Class>
		void disconnect( Class& obj, void ( Class::* func )( Params... ) ) {
			m_functions.erase( Key { &obj, getHash( func ) } );
		}

		template <typename ... Args>
		void operator()( Args...args ) {
			for ( auto const& key : m_functions ) key.second( args... );
		}
	};

	template <typename... Params>
	class Delegate2 {
	private:
		typedef std::function<void( Params... )> Handler;
		typedef std::vector<Handler> Vector;
		Vector m_functions;
	public:
		typedef typename Vector::const_iterator Handle;

		Handle connect( Handler&& func ) {
			m_functions.push_back( std::move( func ) );
			return m_functions.begin() + m_functions.size() - 1;
		}

		template <typename... BindArgs, typename Sfinae = typename std::enable_if<( sizeof...( BindArgs ) > 1 ), void>::type>
				Handle connect( BindArgs&&... args ) {
					return connect( Handler( std::bind( std::forward<BindArgs>( args )... ) ) );
				}

				void disconnect( Handle which ) {
					m_functions.erase( which );
				}

				template <typename ... Args> void operator()( Args...args ) {
					for ( auto const& handler : m_functions )
						handler( args... );
				}
	};

	//////////////////////////////////////////////////////////////////////////
	//Hybrid of Delegate 1 and 2
	//////////////////////////////////////////////////////////////////////////
	template <typename... Params>
	class Delegate {
	private:
		typedef std::function<void( Params... )> FunctionType;
		typedef std::vector<FunctionType> Container;

		Container m_functions;

	public:
		typedef typename Container::const_iterator Handle;

		template <typename Class>
		Handle Connect( Class& obj, void ( Class::* func )( Params... ) )
		{
			m_functions.push_back( { [&obj, func]( Params... params ) { ( obj.*func )( params... ); } } );
			return m_functions.begin() + m_functions.size() - 1;
		}

		void Disconnect( Handle& handle )
		{
			m_functions.erase( handle );
		}

		template <typename ... Args>
		void operator()( Args...args ) const
		{
			for ( auto const& func : m_functions )
			{
				func( args... );
			}
		}
	};

}