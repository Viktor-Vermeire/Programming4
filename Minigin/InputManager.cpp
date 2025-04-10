#include <SDL.h>
#include "SceneManager.h"
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include "Gamepad.h"
#include "iostream"
#include "memory"

bool dae::InputManager::ProcessInput()
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
}


void dae::InputManager::AddGamepad(Gamepad* gamepad)
{
	m_GamePads.emplace_back(gamepad);
}

void dae::InputManager::AddGameActor(GameObject* gameActor)
{
	m_GameActors.emplace_back(gameActor);
}

int dae::InputManager::GetGameActorSize() {
	return static_cast<int>(m_GameActors.size());
}