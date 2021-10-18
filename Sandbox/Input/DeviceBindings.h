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
		DeviceBindings(DeviceId inId) noexcept
			:m_deviceId(inId)
		{
		};

		bool operator==( DeviceId & inId ) const noexcept
		{
			return m_deviceId == inId;
		}

		//Function to setup bindings for a given device
		//This will in future take a processed file with a list of bindings and action ids
		virtual void SetupBindings() noexcept {};

		KeyCodeActionIdMap const & GetKeyCodeActionIdBinding() const noexcept { return m_keycodeActionIdMap; }

	private:
		DeviceId const m_deviceId;
		KeyCodeActionIdMap m_keycodeActionIdMap;
	};

}