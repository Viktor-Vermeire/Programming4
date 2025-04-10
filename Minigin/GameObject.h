#pragma once

#include <vector>
#include "memory"
#include "BaseComponent.h"
#include "unordered_map"
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:

		template <typename ComponentType, typename... Args>
		void AddComponent(Args&&... args) {
			static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "ComponentType must be derived from BaseComponent");
			m_Components.push_back(std::make_unique<ComponentType>(std::forward<Args>(args)...));
		}

		template<class ComponentType>
		ComponentType* GetComponent()
		{
			static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "ComponentType must be derived from BaseComponent");
			for (int looper{ 0 }; looper < m_Components.size(); ++looper)
			{
				if (ComponentType* derivedComponent = dynamic_cast<ComponentType*>(m_Components.at(looper).get())) {
					return static_cast<ComponentType*>(m_Components.at(looper).get());
				}
			}
			return nullptr;
		}

		template<class ComponentType>
		bool IsComponentPresent()
		{
			static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "ComponentType must be derived from BaseComponent");
			for (int looper{ 0 }; looper < m_Components.size(); ++looper)
			{
				if (ComponentType* derivedComponent = dynamic_cast<ComponentType*>(m_Components.at(looper).get())) {
					return true;
				}
			}
			return false;
		}

		template<class ComponentType>
		void RemoveComponent()
		{
			static_assert(std::is_base_of<BaseComponent, ComponentType>::value, "ComponentType must be derived from BaseComponent");
			for (int looper{ 0 }; looper < m_Components.size(); ++looper)
			{
				if (ComponentType* derivedComponent = dynamic_cast<ComponentType*>(m_Components.at(looper).get())) {
					m_Components.at(looper).SetToBeDeleted(true);
				}
			}
		}
		
		virtual void Update();
		virtual void Render();

		void SetPosition(float x, float y);
		void SetParent(GameObject* gameObject);
		Transform GetWorldTransform();
		Transform GetLocalTransform();
		//void CleanupChildren();
		void CleanupComponents();
		void SetWorldTransformOutdated();
		

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AddChild(GameObject* gameObject);
		void RemoveChild(GameObject* gameObject);
		GameObject* m_Parent{ nullptr };
		dae::Transform m_LocalTransform{};
		dae::Transform m_WorldTransform{};
		bool m_IsWorldTransformOutdated;
		std::vector<GameObject*> m_Children;
		std::vector<std::unique_ptr<BaseComponent>> m_Components{};

	};
}
