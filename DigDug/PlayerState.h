#pragma once
namespace dae {
	class GameObject;
	class Gamepad;
	class PlayerState
	{
	public:
		virtual ~PlayerState();
		virtual void HandleInput(GameObject& player, Gamepad& gamepad);
		virtual void Update(GameObject&);
	};
}
