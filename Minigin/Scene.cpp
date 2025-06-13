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

void dae::Scene::Cleanup()
{
	auto it = m_objects.begin();
	while ((it = std::find_if(it, m_objects.end(), [](std::shared_ptr <GameObject>& gameObject) {
		return gameObject->IsToBeDeleted();
		})) != m_objects.end()) {
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), *it), m_objects.end());
	}
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



void dae::Scene::SetStartUpFunctor(std::function<void()>* functor, const StartUpInfo& start)
{
	m_StartUpFunctor = std::make_unique<std::function<void()>>(*functor);
	m_StartupInfo = start;
}

dae::StartUpInfo& dae::Scene::GetStartUpInfo()
{
	return m_StartupInfo;
}

void dae::Scene::ExecuteStartUpFunctor()
{
	if (m_StartUpFunctor)
		(*m_StartUpFunctor.get())();
}

void dae::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

std::string dae::Scene::GetName() const
{
	return m_name;
}

