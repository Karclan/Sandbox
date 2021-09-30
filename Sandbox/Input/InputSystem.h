#pragma once

#include "../Core/Subsystem.h"

#include <map>

namespace Input
{
    class Action;

    class InputSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
    {
    public:
        InputSystem( Core::Registry& registry );
        ~InputSystem() = default;

        bool TestAction(Action const & action) const;

        void BuildWindow( double deltaTime ) override;

    private:

    };
}
