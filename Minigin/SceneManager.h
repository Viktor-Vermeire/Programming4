#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
		void SetActiveScene(Scene& scene);
		void SetToSwitchScene(Scene& scene);
		Scene* GetActiveScene();
		Scene* GetScene(std::string name);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		Scene* m_ActiveScene;
		Scene* m_ToSwitchScene{};
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
