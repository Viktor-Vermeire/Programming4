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
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void Cleanup();

		void RemoveAll();

		void Update();
		void SetStartUpFunctor(std::function<void()>* functor, const StartUpInfo& startUpInfo);
		StartUpInfo& GetStartUpInfo();
		void ExecuteStartUpFunctor();
		void Render() const;
		std::string GetName() const;

		template <typename ComponentType>
		std::vector<GameObject*> findGameObjectsWithComponent() {
			std::vector<GameObject*> result;

			auto it = m_objects.begin();
			while ((it = std::find_if(it, m_objects.end(), [](std::shared_ptr <GameObject>& gameObject) {
				return gameObject->IsComponentPresent<ComponentType>();
				})) != m_objects.end()) {
				result.push_back(it->get());
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

		std::unique_ptr<std::function<void()>> m_StartUpFunctor;

		std::string m_name;
		StartUpInfo m_StartupInfo;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
