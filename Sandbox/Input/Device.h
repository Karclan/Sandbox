#pragma once

namespace Input
{
	enum class DeviceType : int
	{
		Invalid = -1,
		Mouse = 0,
		Keyboard,
		Controller
	};
	
	struct DeviceId
	{
		bool operator==(DeviceId const & other) const noexcept
		{
			bool bSame = true;
			bSame &= m_id == other.m_id;
			bSame &= m_type == other.m_type;
			return bSame;
		}

		int m_id{ -1 };
		DeviceType m_type;
	};

	class Device
	{
	public:
		constexpr Device(DeviceId inId) noexcept
			: m_id(inId)
		{

		}

		bool operator==(Device const & other) const noexcept
		{
			return m_id == other.m_id;
		}

	private:
		DeviceId const m_id;
	};
}