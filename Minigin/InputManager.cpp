#include <SDL.h>
#include "SceneManager.h"
#include "InputManager.h"
#include "Gamepad.h"
#include "GameObject.h"
#include "iostream"
#include "memory"
#include "map"
#include "Command.h"

bool dae::InputManager::ProcessPlayerInput(GameObject* player, Gamepad* gamepad, std::vector<Command*> commands)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	SDL_Event e;

	for (int commandLooper{ 0 }; commandLooper < commands.size(); ++commandLooper) {
		bool gamepadUsed = gamepad->IsUsed();
		bool gamepadCommand = commands.at(commandLooper)->IsUsingGamepad();
		if (gamepadUsed != gamepadCommand)
			continue;
		if (gamepad->IsUsed()) {
			gamepad->Update();
			if (gamepad->IsPressed(commands.at(commandLooper)->GetInputValue()) && gamepad->Connected()) {
				commands.at(commandLooper)->execute(*player);
			}
		}
		else {
			if (pStates[commands.at(commandLooper)->GetInputValue()]) {
			commands.at(commandLooper)->execute(*player);
			}
		}
	}
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}


void dae::InputManager::AddGamepad(const std::string& key, std::unique_ptr<Gamepad> gamepad)
{
	m_GamePads[key] = (std::move(gamepad));
}

dae::Gamepad* dae::InputManager::GetGamePad(const std::string& key) {
	return m_GamePads[key].get();
}

dae::GameObject* dae::InputManager::GetGameActor(const std::string& key)
{
	return m_GameActors[key].get();
}

void dae::InputManager::AddGameActor(const std::string& key,std::unique_ptr<GameObject> gameActor)
{
	m_GameActors[key] = (std::move(gameActor));
}

int dae::InputManager::GetGameActorSize() {
	return static_cast<int>(m_GameActors.size());
}


dae::Command* dae::InputManager::GetCommand(const std::string& key)
{
	return m_Commands[key].get();
}

bool dae::InputManager::CheckExit()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
