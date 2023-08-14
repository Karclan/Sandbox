#include "ResourceManager.h"

#include "../Graphics/ShaderComponent.h"
#include "../Graphics/TextureComponent.h"
#include "../Graphics/MeshComponent.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"


namespace Core
{
	//////////////////////////////////////////////////////////////////////////
	//ResourceManager
	//////////////////////////////////////////////////////////////////////////

	Graphics::ShaderComponent ResourceManager::CreateOrLoadShader( char const * shaderName )
	{
		Graphics::ShaderComponent comp;

		SB::Shared<Graphics::ShaderProgram> resource = GetOrLoadResource<ResourceType::Shader, Graphics::ShaderProgram >( shaderName );
		comp.ShaderHandle = SB::Weak<Graphics::IShaderHandle>( resource );
		comp.ShaderId = resource->GetShaderId();

		return comp;
	}

	Graphics::TextureComponent ResourceManager::CreateOrLoadTexture( char const * textureName )
	{
		Graphics::TextureComponent component;
		//GetOrLoadResource<ResourceType::Texture, Graphics::TextureComponent>( textureName );
		return component;
	}

	Graphics::MeshComponent ResourceManager::CreateOrLoadMesh( char const * meshName )
	{
		Graphics::MeshComponent component;
		//GetOrLoadResource<ResourceType::Mesh, Graphics::MeshComponent>( meshName );
		return component;
	}

	Graphics::MeshComponent ResourceManager::CreateOrLoadMesh( Graphics::PrimativeType type )
	{
		Graphics::MeshComponent component;
		//GetOrLoadResource<ResourceType::Mesh, Graphics::MeshComponent>( std::string( magic_enum::enum_name<Graphics::PrimativeType>( type ) ).c_str() );
		return component;
	}

	void ResourceManager::AddResourceHandler( std::shared_ptr<ResourceTypeHandler> & Handler )
	{
		SB_CORE_ASSERT( Handler, "Passed an invalid handler ptr!" );
		if ( Handler )
		{
			ResourceType const handledType = Handler->GetHandledResourceType();
			if ( ResourceManagerMap::const_iterator HandlerIt = m_resourceHandlers.find( handledType ); HandlerIt == m_resourceHandlers.end() )
			{
				m_resourceHandlers[handledType] = Handler;
			}
			else
			{
				SB_CORE_ASSERT( false, fmt::format( "Duplicate Handler registration attempt! With type: {}", std::string( magic_enum::enum_name<ResourceType>( handledType ) ).c_str() ) );
			}
		}
	}

	void ResourceManager::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
		//Display all the loaded resources we have in some form or another
		//Basically the start of an asset browser?
	}

	ResourceManager::ResourceManager()
	{
		Resources::AssignResourceManager( this );
	}

	//////////////////////////////////////////////////////////////////////////
	//Resources
	//////////////////////////////////////////////////////////////////////////

	Core::ResourceManager * Resources::s_ResourceManager = nullptr;

	Graphics::ShaderComponent Resources::CreateOrLoadShader( char const * shaderName )
	{
		SB_CORE_ASSERT( s_ResourceManager, "Resource Manager not assigned!" );
		return s_ResourceManager->CreateOrLoadShader( shaderName );
	}

	Graphics::TextureComponent Resources::CreateOrLoadTexture( char const * textureName )
	{
		SB_CORE_ASSERT( s_ResourceManager, "Resource Manager not assigned!" );
		return s_ResourceManager->CreateOrLoadTexture( textureName );
	}

	Graphics::MeshComponent Resources::CreateOrLoadMesh( char const * meshName )
	{
		SB_CORE_ASSERT( s_ResourceManager, "Resource Manager not assigned!" );
		return s_ResourceManager->CreateOrLoadMesh( meshName );
	}

	Graphics::MeshComponent Resources::CreateOrLoadMesh( Graphics::PrimativeType type )
	{
		SB_CORE_ASSERT( s_ResourceManager, "Resource Manager not assigned!" );
		return s_ResourceManager->CreateOrLoadMesh( type );
	}

	void Resources::AddResourceHandler( SB::Shared<ResourceTypeHandler> & handler )
	{
		s_ResourceManager->AddResourceHandler( handler );
	}

	void Resources::AssignResourceManager( ResourceManager * manager )
	{
		s_ResourceManager = manager;
	}

}
