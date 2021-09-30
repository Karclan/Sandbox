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

	class Action;
	namespace Literals
	{
		class ActionId
		{
			friend Action;
			friend constexpr ActionId operator""_IA(char const* action, size_t count);

			constexpr ActionId(InputActionValueType hash, std::string_view debugName)
				:m_hash(hash)
				, m_debugName(debugName)
			{

			}

			InputActionValueType m_hash;
			std::string_view m_debugName;

		public:
			constexpr operator InputActionValueType() const
			{
				return m_hash;
			}
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

		operator InputActionValueType() const
		{
			return m_hash;
		}

		InputActionValueType hash() const
		{
			return m_hash;
		}

		std::string_view const & debugName() const
		{
			return m_debugName;
		}

	private:
		InputActionValueType m_hash;
		std::string_view m_debugName;

	};
}
