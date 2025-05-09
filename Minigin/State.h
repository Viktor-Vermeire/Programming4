#pragma once
namespace dae {
	class GameObject;
	class State
	{
	public:
		virtual ~State() = default;
		virtual void Enter(GameObject* gameObject) = 0;
		virtual void Execute(GameObject* gameObject) = 0;
		virtual void Exit(GameObject* gameObject) = 0;
	};
}

