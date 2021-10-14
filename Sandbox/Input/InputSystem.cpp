#include "InputSystem.h"

#include "InputAction.h"
#include "../Utils/UnusedMacro.h"

namespace Input
{
    InputSystem::InputSystem( Core::Registry& registry )
        : Core::Subsystem( registry )
    {

    }

	bool InputSystem::TestAction( ActionId const& action ) const
	{
        using namespace Literals;
        switch ( action )
        {
            case "Test"_ActionId:
            {

            }
        }

        return false;
	}

	void InputSystem::BuildWindow( double deltaTime )
	{
        UNUSED( deltaTime );
	}

}
