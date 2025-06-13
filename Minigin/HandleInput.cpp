#include "HandleInput.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "memory"
#include "GameObject.h"
#include "vector"
#include "Command.h"

dae::HandleInput::HandleInput(Gamepad* gamePad)
{
	m_GamePad = gamePad;
}

void dae::HandleInput::Enter(GameObject*)
{
}

void dae::HandleInput::Execute(GameObject* gameObject)
{
	if (!gameObject->m_Active) return;
	InputManager::GetInstance().ProcessPlayerInput(gameObject, m_GamePad, m_Commands);
}

void dae::HandleInput::Exit(GameObject*)
{
}

void dae::HandleInput::AddCommand(Command* command)
{
	m_Commands.emplace_back(command);
}
