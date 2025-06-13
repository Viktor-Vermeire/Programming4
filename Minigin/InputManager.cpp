#include <SDL.h>
#include "SceneManager.h"
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include "Gamepad.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "iostream"
#include "memory"
#include "map"
#include "Command.h"

/*bool dae::InputManager::ProcessInput()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	SDL_Event e;
	

	for (int actorLooper{ 0 }; actorLooper < m_GameActors.size(); ++actorLooper) {
		for (int commandLooper{ 0 }; commandLooper < m_Commands.size(); ++commandLooper) {
			if (m_GamePads.at(actorLooper)->IsUsed() != m_Commands.at(commandLooper)->IsUsingGamepad())
				continue;
			if (m_GamePads.at(actorLooper)->IsUsed()) {
				m_GamePads.at(actorLooper)->Update();
				if (m_GamePads.at(actorLooper)->IsPressed(m_Commands.at(commandLooper)->GetInputValue())) {
					m_Commands.at(commandLooper)->execute(*m_GameActors.at(actorLooper));
				}
			}
			else {
				if (pStates[m_Commands.at(commandLooper)->GetInputValue()]) {
					m_Commands.at(commandLooper)->execute(*m_GameActors.at(actorLooper));
				}
				ImGui_ImplSDL2_ProcessEvent(&e);
			}
		}
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				return false;
			}
		}
	}
		return true;
}*/

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
			if (gamepad->IsPressed(commands.at(commandLooper)->GetInputValue())) {
				commands.at(commandLooper)->execute(*player);
			}
		}
		else {
			if (pStates[commands.at(commandLooper)->GetInputValue()]) {
			commands.at(commandLooper)->execute(*player);
			}
			ImGui_ImplSDL2_ProcessEvent(&e);
		}
	}
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}


void dae::InputManager::AddGamepad(std::string key, std::unique_ptr<Gamepad> gamepad)
{
	m_GamePads[key] = (std::move(gamepad));
}

dae::Gamepad* dae::InputManager::GetGamePad(std::string key) {
	return m_GamePads[key].get();
}

std::shared_ptr<dae::GameObject> dae::InputManager::GetGameActor(std::string key)
{
	return m_GameActors[key];
}

void dae::InputManager::AddGameActor(std::string key,std::shared_ptr<GameObject> gameActor)
{
	m_GameActors[key] = gameActor;
}

int dae::InputManager::GetGameActorSize() {
	return static_cast<int>(m_GameActors.size());
}


dae::Command* dae::InputManager::GetCommand(std::string key)
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
