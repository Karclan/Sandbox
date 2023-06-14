#pragma once

namespace Core
{
	using ResourceId = unsigned int;

	//This could eventually be changed so instead we've got some option of user defined resource types
	enum class ResourceType
	{
		Texture,
		Mesh,
		Shader,

	};
}