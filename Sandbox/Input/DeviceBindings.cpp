#include "DeviceBindings.h"

#include "../Utils/Logger.h"
#include "../Utils/format.h"

namespace Input
{
	Input::KeyCodeActionIdMap const & DeviceBindings::GetKeyCodeActionIdBinding() const noexcept
	{
		return m_keycodeActionIdMap;
	}

	void DeviceBindings::Merge( DeviceBindings const & other )
	{
		auto map = other.GetKeyCodeActionIdBinding();
		for ( auto it : map )
		{
			KeyCodeActionIdMap::const_iterator duplicateActionId = m_keycodeActionIdMap.find( it.first );
			if ( duplicateActionId != m_keycodeActionIdMap.end() )
			{
				Utils::Logger::LogMessage( Utils::LogType::Warning,
					fmt::format( "Duplicate binding entry found whilst merging {}'s ({}, {}) into {} which has ({},{})\n", 
					other.GetBindingName(),
					it.first,
					it.second.debugName(),
					GetBindingName(), 
					duplicateActionId->first,
					duplicateActionId->second.debugName()
				) );
				throw( "Duplicated Binding issue found!" );
			}
		}
		m_keycodeActionIdMap.insert( map.begin(), map.end() );
	}
}
