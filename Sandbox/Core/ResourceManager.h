#pragma once

#include "Sandbox.h"

#include "Subsystem.h"

#include "../magic_enum/magic_enum_format.hpp"

#include <map>
#include <string>

namespace Graphics
{
	struct ShaderComponent;
	struct TextureComponent;
	struct MeshComponent;
	enum class PrimativeType;
}

namespace Core
{
	enum class ResourceType : int
	{
		Texture,
		Shader,
		Mesh,
		Audio,
	};

	//////////////////////////////////////////////////////////////////////////

	class ResourceTypeHandler
	{
	public:
		virtual ResourceType GetHandledResourceType() const = 0;
		virtual void BuildWindow() = 0;
	};

	template<ResourceType Type, typename HandledObject>
	class TResourceTypeHandler : public ResourceTypeHandler
	{
	public:
		ResourceType HandledType = Type;
		using ReturnTypeClass = SB::Shared<HandledObject>;

		ResourceType GetHandledResourceType() const override { return HandledType; }
		virtual ReturnTypeClass GetOrLoadResource( char const * resourceName ) = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	class ResourceManager : public ImGuiWindowBuilder
	{
	public:
		ResourceManager();
		virtual ~ResourceManager() = default;

		Graphics::ShaderComponent CreateOrLoadShader( char const * shaderName );
		Graphics::TextureComponent CreateOrLoadTexture( char const * textureName );
		Graphics::MeshComponent CreateOrLoadMesh( char const * meshName );
		Graphics::MeshComponent CreateOrLoadMesh( Graphics::PrimativeType type );

		template<ResourceType Type, typename ReturnType>
		SB::Shared<ReturnType> GetOrLoadResource( char const * resourceName )
		{
			//Try find our resource type handler for type
			ResourceManagerMap::iterator itr = m_resourceHandlers.find( Type );
			SB_CORE_ASSERT( itr != m_resourceHandlers.end(), fmt::format( "Failed to find resource manager for type: {}", magic_enum::enum_name<ResourceType>( Type ) ) );
			SB::Shared<ResourceTypeHandler> foundTypeHandler = itr->second;

			//Get the resource
			using TResManager = TResourceTypeHandler<Type, ReturnType>;
			SB::Shared<TResManager> resourceHandler = std::static_pointer_cast< TResManager >( foundTypeHandler );
			auto loadedResource = resourceHandler->GetOrLoadResource( resourceName );
			//Check it's okay
			SB_CORE_ASSERT( loadedResource != nullptr, "Null data on GetOrLoadResource! Returning nullptr" );

			//Return
			return loadedResource;
		}

		void AddResourceHandler( SB::Shared<ResourceTypeHandler> & handler );
		void BuildWindow( double deltaTime ) override;

	private:
		using ResourceManagerMap = std::map<ResourceType, SB::Shared<ResourceTypeHandler>>;
		ResourceManagerMap m_resourceHandlers {};
	};

	//////////////////////////////////////////////////////////////////////////

	struct Resources
	{
		//Friend so the resource manager can assign itself upon creation!
		friend ResourceManager;

		static Graphics::ShaderComponent CreateOrLoadShader( char const * shaderName );
		static Graphics::TextureComponent CreateOrLoadTexture( char const * textureName );
		static Graphics::MeshComponent CreateOrLoadMesh( char const * meshName );
		static Graphics::MeshComponent CreateOrLoadMesh( Graphics::PrimativeType type );

		//Generic template function. Not recommended for use as return types have no rules caus it's POD
		template<ResourceType Type, typename ReturnType>
		static ReturnType GetOrLoadResource( char const * resourceName )
		{
			return s_ResourceManager->GetOrLoadResource<Type, ReturnType>( resourceName );
		}

		static void AddResourceHandler( SB::Shared<ResourceTypeHandler> & handler );

	private:
		static void AssignResourceManager( ResourceManager * manager );
		static ResourceManager * s_ResourceManager;
	};
}
