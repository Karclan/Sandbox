#pragma once

#include "DataBuffer.h"

namespace Graphics
{
	class UniformBufferObject : public DataBuffer
	{
	public:
		virtual ~UniformBufferObject() = default;

		virtual void SetData( void const * data, uint32_t size, uint32_t offset = 0 ) = 0;

		static std::shared_ptr<UniformBufferObject> Create( uint32_t size, uint32_t binding );
	};
}
