#pragma once

#include "Device.h"
#include "InputAction.h"
#include "InputState.h"

#include <map>

namespace Input
{
	//A mapping for action ids to device inputs
	class DeviceBindings
	{
	public:
		DeviceBindings( DeviceId inId ) noexcept
			:m_deviceId( inId )
		{
		};

		virtual ~DeviceBindings() = default;

		bool operator==( DeviceId const & inId ) const noexcept
		{
			return m_deviceId == inId;
		}

		bool operator<( DeviceBindings const & other ) const noexcept
		{
			return m_deviceId < other.m_deviceId;
		}

		DeviceId const & GetDeviceId() const noexcept 
		{
			return m_deviceId;
		}

		//Function to setup bindings for a given device
		//This will in future take a processed file with a list of bindings and action ids
		virtual void SetupBindings() {};

		KeyCodeActionIdMap const & GetKeyCodeActionIdBinding() const noexcept { return m_keycodeActionIdMap; }

	protected:
		DeviceId const m_deviceId;
		KeyCodeActionIdMap m_keycodeActionIdMap;
	};

}