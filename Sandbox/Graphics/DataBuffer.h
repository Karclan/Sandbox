#pragma once

#include <string>
#include <vector>

#include <cstdint>
#include <memory>

namespace Graphics
{

	enum class ShaderDataType : int
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	uint32_t constexpr GetElementSize( ShaderDataType type )
	{
		switch ( type )
		{
			case Graphics::ShaderDataType::Float:
				return sizeof( float );
			case Graphics::ShaderDataType::Float2:
				return sizeof( float ) * 2;
			case Graphics::ShaderDataType::Float3:
				return sizeof( float ) * 3;
			case Graphics::ShaderDataType::Float4:
				return sizeof( float ) * 4;
			case Graphics::ShaderDataType::Mat3:
				return sizeof( float ) * 3 * 3; //3* float3
			case Graphics::ShaderDataType::Mat4:
				return sizeof( float ) * 4 * 4; //4* float4
			case Graphics::ShaderDataType::Int:
				return sizeof( int );
			case Graphics::ShaderDataType::Int2:
				return sizeof( int ) * 2;
			case Graphics::ShaderDataType::Int3:
				return sizeof( int ) * 3;
			case Graphics::ShaderDataType::Int4:
				return sizeof( int ) * 4;
			case Graphics::ShaderDataType::Bool:
				return sizeof( bool );
			case Graphics::ShaderDataType::None:
			default:
				return 0;
		}
	};

	uint32_t constexpr GetComponentCount( ShaderDataType type )
	{ 
		switch ( type )
		{
			case Graphics::ShaderDataType::Float:
				return 1;
			case Graphics::ShaderDataType::Float2:
				return 2;
			case Graphics::ShaderDataType::Float3:
				return 3;
			case Graphics::ShaderDataType::Float4:
				return 4;
			case Graphics::ShaderDataType::Mat3:
				return 3; //3* float3
			case Graphics::ShaderDataType::Mat4:
				return 4; //4* float4
			case Graphics::ShaderDataType::Int:
				return 1;
			case Graphics::ShaderDataType::Int2:
				return 2;
			case Graphics::ShaderDataType::Int3:
				return 3;
			case Graphics::ShaderDataType::Int4:
				return 4;
			case Graphics::ShaderDataType::Bool:
				return 1;
			case Graphics::ShaderDataType::None:
			default:
				return 0;
		}
	};

	struct BufferElement
	{
		static constexpr BufferElement Create( ShaderDataType type, const char * inName, bool inNormalized = false )
		{
			return BufferElement( type, GetElementSize( type ), GetComponentCount( type ), 0, inNormalized, inName );
		}

		ShaderDataType ElementType {ShaderDataType::None};
		uint32_t Size { 0 };
		uint32_t ElementCount { 0 };
		size_t Offset { 0 };
		bool Normalized { false };
		std::string Name {};

	private:
		constexpr BufferElement( ShaderDataType inType, uint32_t inSize, uint32_t inCount, size_t inOffset, bool inNormalized, const char * inName )
			: ElementType( inType )
			, Size( inSize )
			, ElementCount( inCount )
			, Offset( inOffset )
			, Normalized( inNormalized )
			, Name( inName )
		{
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout( std::initializer_list<BufferElement> elements )
			: m_Elements( elements )
		{
			CalculateOffsetsAndStride();
		}

		auto GetStride() const { return m_BufferStride; }
		auto const & GetElements() const { return m_Elements; }

		//lower case functions for C++ for( auto it : bufferLayout) use case
		//Iterators
		auto begin() { return m_Elements.begin(); }
		auto end() { return m_Elements.end(); }

		//Const iterators
		auto begin() const { return m_Elements.begin(); }
		auto end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_BufferStride = 0;
			for ( auto & element : m_Elements )
			{
				element.Offset = offset;
				offset += element.Size;
				m_BufferStride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements {};
		uint32_t m_BufferStride { 0 };
	};

	class DataBuffer
	{
	public:
		using BufferId = unsigned int;
		
		virtual ~DataBuffer() = default;

		BufferId GetId() const { return m_Id; }
		bool GetCreated() const { return m_Created; }

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

	protected:
		BufferId m_Id { 0 };
		bool m_Created { false };
	};

	class VertexBuffer : public DataBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData( const void * data, uint32_t dataSize ) = 0;

		virtual BufferLayout const & GetLayout() const = 0;
		virtual void SetLayout( BufferLayout const & layout ) = 0;

		virtual uint32_t GetVertexCount() const = 0;

		static std::shared_ptr<VertexBuffer> Create( uint32_t size );
		static std::shared_ptr<VertexBuffer> Create( float * verticies, uint32_t size );
		static std::shared_ptr<VertexBuffer> Create( float const * verticies, uint32_t size );
	};

	class IndexBuffer : public DataBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual uint32_t GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create( uint32_t * indicies, uint32_t count );
		static std::shared_ptr<IndexBuffer> Create( uint32_t const * indicies, uint32_t count );
	};

}
