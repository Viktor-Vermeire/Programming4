#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "iostream"
#include "unordered_map"
#include "memory"
#include "BaseComponent.h"
#include "Transform.h"

dae::GameObject::GameObject()
{
	m_WorldTransform = dae::Transform();
}

dae::GameObject::~GameObject() = default;


void dae::GameObject::Update(){
	for (const auto& component : m_Components) {
		component->Update();
	}
	
}

void dae::GameObject::Render()
{
	if (m_IsWorldTransformOutdated) {
		if (m_Parent == nullptr) {
			m_WorldTransform.SetPosition((m_LocalTransform.GetPosition().x), (m_LocalTransform.GetPosition().y), 0.0f);
		}
		else {
			m_WorldTransform.SetPosition((m_LocalTransform.GetPosition().x + m_Parent->GetWorldTransform().GetPosition().x), (m_LocalTransform.GetPosition().y + m_Parent->GetWorldTransform().GetPosition().y), 0.0f);
		}
		m_IsWorldTransformOutdated = false;
	}
	for (const auto& component : m_Components) {
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
	SetWorldTransformOutdated();
}

void dae::GameObject::SetParent(GameObject* gameObject)
{
	if (gameObject != this) {
		gameObject->AddChild(this);
		glm::vec3 temp;
		if (m_Parent != nullptr) {
			m_Parent->RemoveChild(gameObject);
			temp = m_LocalTransform.GetPosition() - m_Parent->GetWorldTransform().GetPosition() + gameObject->GetWorldTransform().GetPosition();
		}
		else {
			temp = m_LocalTransform.GetPosition() + gameObject->GetWorldTransform().GetPosition();
		}
		
		m_Parent = gameObject;
		m_LocalTransform.SetPosition(temp.x, temp.y, temp.z);
	}
}

void dae::GameObject::AddChild(GameObject* gameObject)
{
	m_Children.push_back(gameObject);
}

void dae::GameObject::RemoveChild(GameObject* gameObject)
{
	for (int looper{ 0 }; looper < m_Children.size(); ++looper)
	{
		if (m_Children.at(looper) == gameObject) {
			m_Children.at(looper) = nullptr;
		}
	}
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), nullptr), m_Children.end());
}

dae::Transform dae::GameObject::GetWorldTransform()
{
	return m_WorldTransform;
}

dae::Transform dae::GameObject::GetLocalTransform()
{
	return m_LocalTransform;
}

void dae::GameObject::CleanupComponents()
{
	for (int looper {0}; looper < m_Components.size() ; ++looper)
	{
		if (m_Components.at(looper)->ShouldBeDeleted()) {
			m_Components.at(looper) = nullptr;
		}
	}
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), nullptr), m_Components.end());
}

void dae::GameObject::SetWorldTransformOutdated()
{
	m_IsWorldTransformOutdated = true;
	for (int looper = 0; looper < m_Children.size(); ++looper) {
		m_Children.at(looper)->SetWorldTransformOutdated();
	}

}
