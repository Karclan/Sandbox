#pragma once

#include <cstdint>
#include <string>

namespace Input
{
	namespace Hash
	{
		// FNV-1a 32bit hashing algorithm.
		constexpr std::uint64_t fnv1a_64(char const* s, size_t count)
		{
			return ((count ? fnv1a_64(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
		}
	}

	class Action;
	namespace Literals
	{
		class ActionId
		{
			friend Action;
			friend constexpr ActionId operator""_IA(char const* action, size_t count);

			constexpr ActionId(std::uint64_t hash, std::string_view debugName)
				:m_hash(hash)
				, m_debugName(debugName)
			{

			}
			std::uint64_t m_hash;
			std::string_view m_debugName;
		};

		constexpr ActionId operator"" _IA(char const* string, size_t count)
		{
			return ActionId(Hash::fnv1a_64(string, count), string);
		}
	}

	class Action
	{
	public:
		constexpr Action(Literals::ActionId Id)
			: m_hash(Id.m_hash)
			, m_debugName(Id.m_debugName)
		{
		}

		operator std::uint64_t() const
		{
			return m_hash;
		}

		std::uint64_t hash() const
		{
			return m_hash;
		}

		std::string_view const & debugName() const
		{
			return m_debugName;
		}

	private:
		std::uint64_t m_hash;
		std::string_view m_debugName;

	};
}
