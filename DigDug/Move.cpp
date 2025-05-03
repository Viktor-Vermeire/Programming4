#include "Move.h"
#include "GameObject.h"
#include "Transform.h"
#include "Command.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"

namespace dae {
	Move::Move(int inputValue,bool usingGamepad, AnimationComponent::Direction direction): Command(inputValue, usingGamepad), m_Direction{direction}
	{
	}
	void Move::execute(GameObject& gameObject)
	{
		MovementComponent* component = gameObject.GetComponent<MovementComponent>();
		if (component != nullptr) {
			component->Move(m_Direction);
		}
	}
}
