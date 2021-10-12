#pragma once

#include <cstdint>
#include <string>

namespace Input
{
	using InputActionValueType = std::uint64_t;

	namespace Hash
	{
		// FNV-1a 64bit hashing algorithm.
		constexpr InputActionValueType fnv1a_64(char const* s, size_t count)
		{
			return ((count ? fnv1a_64(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
		}
	}

	class ActionIdLiteral;

	inline namespace Literals
	{
		constexpr ActionIdLiteral operator"" _ActionId( char const* string, size_t count );
	}

	class ActionIdLiteral
	{
	public:
		constexpr operator InputActionValueType() const
		{
			return m_hash;
		}

	private:
		friend class ActionId;
		friend constexpr ActionIdLiteral Literals::operator""_ActionId( char const* action, size_t count );

		constexpr ActionIdLiteral( InputActionValueType hash, std::string_view debugName )
			:m_hash( hash )
			, m_debugName( debugName )
		{

		}

		InputActionValueType const m_hash;
		std::string_view const m_debugName;
	};

	inline namespace Literals
	{
		constexpr ActionIdLiteral operator"" _ActionId( char const* string, size_t count )
		{
			return ActionIdLiteral( Hash::fnv1a_64( string, count ), string );
		}
	}

	class ActionId
	{
	public:
		constexpr ActionId(ActionIdLiteral Id)
			: m_hash(Id.m_hash)
			, m_debugName(Id.m_debugName)
		{
		}

		operator InputActionValueType() const
		{
			return m_hash;
		}

		constexpr InputActionValueType hash() const
		{
			return m_hash;
		}

		constexpr std::string_view const & debugName() const
		{
			return m_debugName;
		}

	private:
		InputActionValueType m_hash;
		std::string_view m_debugName;

	};
}
