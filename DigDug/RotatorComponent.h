#pragma once
#include "BaseComponent.h"
#include "glm.hpp"

namespace dae {
	class RotatorComponent : public BaseComponent
	{
	public:
		RotatorComponent(GameObject& gameObject, float turnRate);
		void SetTurnRate(float turnRate);
		void Update() override;
		void Rotate();
	private:
		float m_TurnRate;
		glm::mat4 m_TransformMatrix;
	};
}

