#pragma once
#include <IObserver.h>
#include <vector>
namespace dae {
	class GameObject;
	class Scene;
	class PlayerPresenceObserver : public IObserver
	{
	public:
		PlayerPresenceObserver();
		void onNotify(GameObject& entity, Event event) override;

		void PrepObserver(GameObject&, Event event, Scene& toCheckScene);
		std::vector<GameObject*> GetLivingPlayers();
	private:
		std::vector<GameObject*> m_LivingPlayers;
	};
}
