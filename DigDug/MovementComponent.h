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
		void ResetMovement();
		bool WantsToMove();
		void SetWantsToMove(bool value);
		bool IsRunning();
		void Update() override;
		void ExecuteMove();
		void Move(dae::RenderComponent::Direction direction);
	private:
		GameObject* m_Hallways;
		bool m_IsMoving;
		bool m_WantsToMove;
		float m_TimeInMovement;
		int m_DistancePerMove;
		float m_TimePerMove;
		glm::vec2 m_CurrentMovement;
	};
}

