#include "Scene.h"
#include "GameObject.h"

#include <algorithm>



unsigned int dae::Scene::m_idCounter = 0;

dae::Scene::Scene(const std::string& name) : m_name(name) {}

dae::Scene::~Scene() = default;

void dae::Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void dae::Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void dae::Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void dae::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

std::string dae::Scene::GetName()
{
	return m_name;
}

