#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "functional"
#include "vector"


namespace dae
{
	struct StartUpInfo {
		std::vector<std::pair<float, float>> PlayerPositions;
	};
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Add(GameObject* gameObject);
		void Remove(GameObject* object);
		void Cleanup();

		void RemoveAll();

		void Update();
		void SetStartUpFunctor(std::function<void(Scene*)>* functor, const StartUpInfo& startUpInfo);
		StartUpInfo& GetStartUpInfo();
		void ExecuteStartUpFunctor(Scene* previousScene);
		void Render() const;
		std::string GetName() const;

		template <typename ComponentType>
		std::vector<GameObject*> findGameObjectsWithComponent() {
			std::vector<GameObject*> result;

			auto it = m_Objects.begin();
			while ((it = std::find_if(it, m_Objects.end(), [](GameObject* gameObject) {
				return gameObject->IsComponentPresent<ComponentType>();
				})) != m_Objects.end()) {
				result.push_back(*it);
				++it;
			}
			return result;
		}

		template <typename ComponentType>
		std::vector<GameObject*> findActiveGameObjectsWithComponent() {
			std::vector<GameObject*> result;

			auto it = m_Objects.begin();
			while ((it = std::find_if(it, m_Objects.end(), [](GameObject* gameObject) {
				return gameObject->IsComponentPresent<ComponentType>() && gameObject->m_Active;
				})) != m_Objects.end()) {
				result.push_back(*it);
				++it;
			}
			return result;
		}

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::unique_ptr<std::function<void(Scene*)>> m_StartUpFunctor;

		std::string m_name;
		StartUpInfo m_StartupInfo;
		std::vector < std::unique_ptr<GameObject>> m_BoundObjects{};
		std::vector <GameObject*> m_Objects;

		static unsigned int m_idCounter; 
	};

}
