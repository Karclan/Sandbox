#pragma once

#include "Core/GameClass.h"

#include "PlayerControllerSubsystem.h"
#include "GameGridSubsystem.h"
#include "GameBindings.h"

#include <memory>

namespace Game
{
	class SpaceInvadersGame : public Core::GameClass
	{
	public:
		SpaceInvadersGame();

		void Init() override;
		void EditorInit() override;

		void DeEditorInit() override;

		void DeInit() override;

	protected:
		void RegisterComponentDetails() override;

	private:
		Game::PlayerControllerSubsystem m_PlayerControllerSubsytem;
		Game::GameGridSubsystem m_GridSubsystem;

		std::shared_ptr<Input::DeviceBindings> KeyboardBindings;
		std::shared_ptr<Input::DeviceBindings> MouseBindings;
	};
}
