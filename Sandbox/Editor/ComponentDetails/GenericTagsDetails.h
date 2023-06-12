#pragma once

#include "entt/core/type_info.hpp"

#include "../Utils/TemplateUtils.h"
#include <string>

namespace Editor
{
	class TagDetails
	{
	public:
		constexpr TagDetails() = default;
		virtual ~TagDetails() = default;

		virtual constexpr entt::type_info GetHandledType() const = 0;
		virtual char const * GetTagName() const = 0;
		virtual constexpr entt::id_type GetTagId() const = 0;
	};

	template<typename Tag, Utils::StringLiteral humanReadableName>
	class TTagDetails : public TagDetails
	{
	public:
		using HandledType = Tag;

		constexpr TTagDetails() = default;
		virtual ~TTagDetails() = default;

		constexpr entt::type_info GetHandledType() const override { return entt::type_id<Tag>(); }
		constexpr char const * GetTagName() const override { return humanReadableName.Value; }
		constexpr entt::id_type GetTagId() const override { return entt::type_hash<Tag>(); }

	private:
	};
}