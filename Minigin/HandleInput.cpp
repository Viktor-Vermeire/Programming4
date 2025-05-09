#include "HandleInput.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "memory"
#include "GameObject.h"

dae::HandleInput::HandleInput(const Gamepad& gamePad)
{
	m_GamePad = std::make_unique<Gamepad>(gamePad);
}

void dae::HandleInput::Enter(GameObject* gameObject)
{
	gameObject->GetLocalTransform();
}

void dae::HandleInput::Execute(GameObject* gameObject)
{
	InputManager::GetInstance().ProcessPlayerInput(gameObject, m_GamePad.get());
}

void dae::HandleInput::Exit(GameObject* gameObject)
{
	gameObject->GetLocalTransform();
}
