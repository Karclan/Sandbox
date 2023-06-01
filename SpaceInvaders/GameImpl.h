#pragma once

#include "Core/GameClass.h"

#include "PlayerControllerSubsystem.h"

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
	};
}
