#pragma once
namespace dae {
	class GameObject;
	class Condition
	{
	public:
		virtual ~Condition() = default;
		virtual bool IsMet(GameObject* gameObject) = 0;

	};
}

