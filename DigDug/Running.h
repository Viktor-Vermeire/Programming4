#pragma once
#include <State.h>
namespace dae {
	class GameObject;
	class Running : public State
	{
		void Enter(GameObject* gameObject) override;
		void Execute(GameObject* gameObject) override;
		void Exit(GameObject* gameObject) override;
	};
}

