#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "memory"



unsigned int dae::Scene::m_idCounter = 0;

dae::Scene::Scene(const std::string& name) : m_name(name) {}

dae::Scene::~Scene() = default;

void dae::Scene::Add(std::unique_ptr<dae::GameObject> object)
{
	m_Objects.emplace_back(object.get());
	m_BoundObjects.emplace_back(std::move(object));
}

void dae::Scene::Add(dae::GameObject* gameObject)
{
	m_Objects.emplace_back(gameObject);
}

void dae::Scene::Remove(dae::GameObject* object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
	//m_BoundObjects.erase(std::remove(m_BoundObjects.begin(), m_BoundObjects.end(), object), m_BoundObjects.end());
}

void dae::Scene::Cleanup()
{
	{
		auto it = m_Objects.begin();
		while ((it = std::find_if(it, m_Objects.end(), [](GameObject* gameObject) {
			return gameObject->IsToBeDeleted();
			})) != m_Objects.end()) {
			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), *it), m_Objects.end());
		}
	}
	{
		auto it = m_BoundObjects.begin();
		while ((it = std::find_if(it, m_BoundObjects.end(), [](std::unique_ptr <GameObject>& gameObject) {
			return gameObject->IsToBeDeleted();
			})) != m_BoundObjects.end()) {
			it->reset();
			m_BoundObjects.erase(std::remove(m_BoundObjects.begin(), m_BoundObjects.end(), *it), m_BoundObjects.end());
		}
	}
}

void dae::Scene::RemoveAll()
{
	m_Objects.clear();
	m_BoundObjects.clear();
}

void dae::Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}



void dae::Scene::SetStartUpFunctor(std::function<void(Scene*)>* functor, const StartUpInfo& start)
{
	m_StartUpFunctor = std::make_unique<std::function<void(Scene*)>>(*functor);
	m_StartupInfo = start;
}

dae::StartUpInfo& dae::Scene::GetStartUpInfo()
{
	return m_StartupInfo;
}

void dae::Scene::ExecuteStartUpFunctor(dae::Scene* previousScene)
{
	if (m_StartUpFunctor)
		(*m_StartUpFunctor.get())(previousScene);
}

void dae::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

std::string dae::Scene::GetName() const
{
	return m_name;
}

