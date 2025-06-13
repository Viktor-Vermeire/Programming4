#pragma once
#include "IObserver.h"
#include "vector"
namespace dae {
    class GameObject;
    class Scene;
    class EnemyPresenceObserver : public IObserver
    {
    public:
        EnemyPresenceObserver();
        void onNotify(GameObject& entity, Event event) override;
        std::vector<GameObject*> GetLivingEnemies();
        void PrepObserver(GameObject&, Event event, Scene& toCheckScene);
    private:
        std::vector<GameObject*> m_Enemies{};
    };
}

