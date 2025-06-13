#include "HandleInput.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "memory"
#include "GameObject.h"
#include "vector"
#include "Command.h"

dae::HandleInput::HandleInput()
{
}

void dae::HandleInput::Enter(GameObject*)
{
}

void dae::HandleInput::Execute(GameObject* gameObject)
{
	if (!gameObject->m_Active) return;
	for (auto gamepad : m_Gamepads) {
		InputManager::GetInstance().ProcessPlayerInput(gameObject, gamepad, m_Commands);
	}
}

void dae::HandleInput::Exit(GameObject*)
{
}

void dae::HandleInput::AddCommand(Command* command)
{
	m_Commands.emplace_back(command);
}
void dae::HandleInput::AddGamepad(Gamepad* gamepad)
{
	m_Gamepads.emplace_back(gamepad);
}
