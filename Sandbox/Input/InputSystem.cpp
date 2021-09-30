#include "InputSystem.h"

#include "InputAction.h"

namespace Input
{
    InputSystem::InputSystem( Core::Registry& registry )
        : Core::Subsystem( registry )
    {

    }

	bool InputSystem::TestAction( Action const& action ) const
	{
        using namespace Literals;
        switch ( action )
        {
            case "Test"_IA:
            {

            }
        }

        return false;
	}

	void InputSystem::BuildWindow( double deltaTime )
	{
	
	}

}
