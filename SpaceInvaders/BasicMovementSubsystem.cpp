#include "BasicMovementSubsystem.h"

#include "entt/entt.hpp"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"

#include <corecrt_math_defines.h>

#include "magic_enum/magic_enum.hpp"

#define TAU (M_PI * 2.0f)

namespace Game
{
    BasicMovementSubsystem::BasicMovementSubsystem( entt::registry & m_registry )
        : Subsystem( m_registry )
    {
        m_entity = m_registry.create();
        m_registry.assign<MovementStruct>( m_entity, MovementStruct{});
    }

    void BasicMovementSubsystem::CreateComponents(Graphics::Window & window)
    {
        //Component and entity creation/registration
        {
            Graphics::Renderer * renderer = window.GetRenderer();

            using namespace magic_enum::bitwise_operators;
            //Move into system or at least out of here
            auto basicShaderId = renderer->CreateShader( "basic", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment }  );
            auto colourShiftShaderId = renderer->CreateShader( "colourShift", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
            auto textureEnabledShaderId = renderer->CreateShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
            auto containerTextureId = renderer->CreateTexture( "container.jpg" );
            auto awesomefaceTextureId = renderer->CreateTexture( "awesomeface.png" );

            Graphics::TextureComponent containerTextureComp { { containerTextureId } };
            Graphics::TextureComponent awesomeFaceTextureComp { { awesomefaceTextureId } };
            Graphics::TextureComponent multiTextureComp { { containerTextureId, awesomefaceTextureId } };

            Graphics::ShaderComponent basicShaderComp { basicShaderId };
            Graphics::ShaderComponent colourShiftShaderComp { colourShiftShaderId };
            Graphics::ShaderComponent textureEnabledShaderComp { textureEnabledShaderId };

            //Triangle
            {
                Graphics::TransformComponent triangleTransform;
                triangleTransform.Position = { -2.f,-2.f,-1.f };
                triangleTransform.Scale = { .5f, .5f, .5f };

                auto entity = m_registry.create();
                m_registry.assign<Graphics::TransformComponent>( entity, triangleTransform );
                m_registry.assign<Graphics::MeshComponent>( entity, renderer->GetMesh( Graphics::PrimativeType::Triangle ) );
                m_registry.assign<Graphics::TextureComponent>( entity, containerTextureComp );
                m_registry.assign<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
            }

            //Square
            {
                Graphics::TransformComponent squareTransform;
                squareTransform.Position = { 1.f, 1.f, 0.f };

                auto entity = m_registry.create();
                m_registry.assign<Graphics::TransformComponent>( entity, squareTransform );
                m_registry.assign<Graphics::MeshComponent>( entity, renderer->GetMesh( Graphics::PrimativeType::Square ) );
                m_registry.assign<Graphics::TextureComponent>( entity, awesomeFaceTextureComp );
                m_registry.assign<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
            }

            //Cube
            {
                Graphics::TransformComponent cubeTransform;

                auto entity = m_registry.create();
                m_registry.assign<Graphics::TransformComponent>( entity, cubeTransform );
                m_registry.assign<Graphics::MeshComponent>( entity, renderer->GetMesh( Graphics::PrimativeType::Cube ) );
                m_registry.assign<Graphics::TextureComponent>( entity, multiTextureComp );
                m_registry.assign<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
            }

            for( int i = 0; i < 10; ++i )
            {
                {
                    Graphics::TransformComponent randomTransform;
                    randomTransform.Position = { i, i, i / 2.f };

                    auto entity = m_registry.create();
                    m_registry.assign<Graphics::TransformComponent>( entity, randomTransform );
                    m_registry.assign<Graphics::MeshComponent>( entity, renderer->GetMesh( Graphics::PrimativeType::Cube ) );
                    m_registry.assign<Graphics::TextureComponent>( entity, multiTextureComp );
                    m_registry.assign<Graphics::ShaderComponent>( entity, textureEnabledShaderComp );
                }
            }
        }
    }

    void BasicMovementSubsystem::Integrate( double deltaTime )
    {
        MovementStruct & movementStruct = m_registry.get<MovementStruct>(m_entity);
             
        //Test movement code
        movementStruct.m_angle += movementStruct.speed * static_cast<float>( deltaTime );
        if( movementStruct.m_angle > static_cast< float >( TAU ) ) movementStruct.m_angle -= static_cast<float>( TAU );
		movementStruct.m_xy.x = glm::sin( movementStruct.m_angle );
		movementStruct.m_xy.y = glm::cos( movementStruct.m_angle );

        auto view = m_registry.view<Graphics::TransformComponent, Graphics::MeshComponent>();
        for( auto const & entity : view )
        {
            Graphics::TransformComponent & transform = view.get<Graphics::TransformComponent>( entity );
            Graphics::MeshComponent & mesh = view.get<Graphics::MeshComponent>( entity );

            switch( mesh.type )
            {
            case Graphics::PrimativeType::Cube:
                transform.Position.x = movementStruct.m_xy.x;
                transform.RotationQuat = glm::angleAxis(movementStruct.m_xy.x, Graphics::Constants::Right);
				transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.y, Graphics::Constants::Up);
                transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.x * movementStruct.m_xy.y, Graphics::Constants::Forward);
                break;
            case Graphics::PrimativeType::Triangle:
				transform.RotationQuat = glm::angleAxis(movementStruct.m_xy.x, Graphics::Constants::Right);
				transform.RotationQuat *= glm::angleAxis(movementStruct.m_xy.y, Graphics::Constants::Up);
                break;
            case Graphics::PrimativeType::Square:
                transform.Position = { movementStruct.m_xy.x ,movementStruct.m_xy.y, 1.f };
                break;
            default:
                break;
            }
        }
    }

    void BasicMovementSubsystem::FixedUpdate( double deltaTime )
    {
        Integrate( deltaTime );
    }
}

