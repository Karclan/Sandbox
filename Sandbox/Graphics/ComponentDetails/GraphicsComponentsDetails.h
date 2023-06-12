#pragma once

#include "../Editor/ComponentDetails/ComponentDetailsBase.h"

namespace Graphics
{
	struct CameraComponent;
	struct TransformComponent;
	struct MeshComponent;
	struct ShaderComponent;
	struct TextureComponent;
}

namespace Editor
{
	class CameraComponentDetails : public TComponentDetails<Graphics::CameraComponent>
	{
	protected:
		void BuildDetailsInternal( Graphics::CameraComponent & inComponent ) const override;
	};

	class TransformComponentDetails : public TComponentDetails<Graphics::TransformComponent>
	{
	protected:
		void BuildDetailsInternal( Graphics::TransformComponent & inComponent ) const override;
	};

	class MeshComponentDetails : public TComponentDetails<Graphics::MeshComponent>
	{
	protected:
		void BuildDetailsInternal( Graphics::MeshComponent & inComponent ) const override;
	};

	class ShaderComponentDetails : public TComponentDetails<Graphics::ShaderComponent>
	{
	protected:
		void BuildDetailsInternal( Graphics::ShaderComponent & inComponent ) const override;
	};

	class TextureComponentDetails : public TComponentDetails<Graphics::TextureComponent>
	{
	protected:
		void BuildDetailsInternal( Graphics::TextureComponent & inComponent ) const override;
	};

}
