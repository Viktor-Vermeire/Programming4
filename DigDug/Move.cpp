#include "Move.h"
#include "GameObject.h"
#include "Transform.h"
#include "Command.h"
#include "RenderComponent.h"
#include "MovementComponent.h"

namespace dae {
	Move::Move(int inputValue,bool usingGamepad, RenderComponent::Direction direction): Command(inputValue, usingGamepad), m_Direction{direction}
	{
	}
	void Move::execute(GameObject& gameObject)
	{
		MovementComponent* component = gameObject.GetComponent<MovementComponent>();
		RenderComponent* render = gameObject.GetComponent<RenderComponent>();
		if (component != nullptr) {
			render->SetDirection(m_Direction);
			component->Move(m_Direction);
		}
	}
}
