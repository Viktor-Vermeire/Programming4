#pragma once
#include <BaseComponent.h>
#include <AnimationComponent.h>
#include <glm.hpp>
namespace dae {
	class GameObject;
	class MovementComponent : 
		public BaseComponent
	{
	public:
		MovementComponent(GameObject& gameObject);
		void SetDistancePerMove(int distance);
		void SetTimePerMove(float time);
		void SetHallways(GameObject* hallways);
		void Update() override;
		void Move(dae::AnimationComponent::Direction direction);
	private:
		GameObject* m_Hallways;
		bool m_IsMoving;
		float m_TimeInMovement;
		int m_DistancePerMove;
		float m_TimePerMove;
		glm::vec2 m_CurrentMovement;
	};
}

