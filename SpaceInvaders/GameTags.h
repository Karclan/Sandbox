#pragma once

#include "entt/core/type_traits.hpp"
#include "entt/core/hashed_string.hpp"

namespace Game
{
	using namespace entt::literals;
	using PlayerTag = entt::tag<"Player"_hs>;
}