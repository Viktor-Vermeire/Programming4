#pragma once
#include <string>
#include "memory"
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(GameObject& gameObject);
		virtual ~BaseComponent();
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;

		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;
		
		virtual void Update();
		virtual void Render();
		GameObject* GetOwner();
		bool ShouldBeDeleted();
		void SetToBeDeleted(bool toBeDeleted);
	private:
		GameObject* m_pOwner;
		bool m_ToBeDeleted;

	};
}

