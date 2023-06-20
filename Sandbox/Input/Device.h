#pragma once

#include <cstdint>

namespace Input
{
	enum class DeviceType : int32_t
	{
		Invalid = -1,
		Mouse = 0,
		Keyboard,
		Controller
	};


	struct DeviceId
	{
		using IdType = int32_t;
		constexpr DeviceId( IdType id, DeviceType type )
			: m_id( id )
			, m_type( type )
		{
		}

		bool operator==( DeviceId const & other ) const noexcept
		{
			bool bSame = true;
			bSame &= m_id == other.m_id;
			bSame &= m_type == other.m_type;
			return bSame;
		}

		bool operator<( DeviceId const & other ) const noexcept
		{
			if ( m_type == other.m_type )
			{
				return m_id < other.m_id;
			}
			else
			{
				return m_type < other.m_type;
			}
		}

		int32_t const m_id;
		DeviceType const  m_type;
	};

	class Device
	{
	public:
		constexpr Device( DeviceId inId ) noexcept
			: m_id( inId )
		{
		}

		bool operator==( Device const & other ) const noexcept
		{
			return m_id == other.m_id;
		}

	private:
		DeviceId const m_id;
	};
}