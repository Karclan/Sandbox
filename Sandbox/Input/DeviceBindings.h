#pragma once

#include "Device.h"
#include "InputAction.h"
#include "InputState.h"

#include "../Utils/TemplateUtils.h"

#include <map>

namespace Input
{
	//A mapping for action ids to device inputs
	class DeviceBindings
	{
	public:
		DeviceBindings() = default;
		virtual ~DeviceBindings() = default;

		bool operator==( DeviceId const & inId ) const noexcept
		{
			return GetDeviceId() == inId;
		}

		bool operator<( DeviceBindings const & other ) const noexcept
		{
			return GetDeviceId() < other.GetDeviceId();
		}

		virtual constexpr DeviceId GetDeviceId() const = 0;
		virtual char const * GetBindingName() const = 0;

		//Function to setup bindings for a given device
		//This will in future take a processed file with a list of bindings and action ids
		virtual void SetupBindings() = 0;

		KeyCodeActionIdMap const & GetKeyCodeActionIdBinding() const noexcept;

		void Merge( DeviceBindings const & other );

	protected:
		KeyCodeActionIdMap m_keycodeActionIdMap {};
	};

	template<DeviceId deviceId, Utils::StringLiteral humanReadableName>
	class TDeviceBindings : public DeviceBindings
	{
	public:
		virtual constexpr DeviceId GetDeviceId() const { return deviceId; }
		virtual char const * GetBindingName() const { return humanReadableName.Value; }

		virtual void SetupBindings() = 0;

	};
}