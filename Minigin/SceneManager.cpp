#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	/*
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}*/
	m_ActiveScene->Update();
	m_ActiveScene->Cleanup();
	if (m_ToSwitchScene != nullptr) {
		m_ActiveScene = m_ToSwitchScene;
		m_ActiveScene->ExecuteStartUpFunctor();
		m_ToSwitchScene = nullptr;
	}
}

void dae::SceneManager::Render()
{
	/*
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}*/
	m_ActiveScene->Render();
}

void dae::SceneManager::SetActiveScene(Scene& scene)
{
	m_ActiveScene = &scene;
}

void dae::SceneManager::SetToSwitchScene(Scene& scene)
{
	m_ToSwitchScene = &scene;
}

dae::Scene* dae::SceneManager::GetActiveScene()
{
	return m_ActiveScene;
}

dae::Scene* dae::SceneManager::GetScene(std::string name)
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene> scene) {
		return scene->GetName() == name;});
	if (scene != m_scenes.end()) {
		return scene->get();
	}
	return nullptr;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
