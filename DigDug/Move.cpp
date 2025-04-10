#include "Move.h"
#include "GameObject.h"
#include "Transform.h"
#include "Command.h"

namespace dae {
	Move::Move(int inputValue,bool usingGamepad, std::pair<float, float> direction): Command(inputValue, usingGamepad), m_Direction{direction}
	{
	}
	void Move::execute(GameObject& gameObject)
	{
		gameObject.SetPosition(gameObject.GetLocalTransform().GetPosition().x + m_Direction.first, gameObject.GetLocalTransform().GetPosition().y + m_Direction.second);
	}
}
