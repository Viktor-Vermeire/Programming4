#pragma once
#include <BaseComponent.h>
#include "vector"
#include "memory"
namespace dae {
    class GameObject;
    class EnemyPresenceObserver;
    class PlayerPresenceObserver;
    class IObserver;
    class Scene;
    class GameMasterComponent : public BaseComponent
    {
    public:
        GameMasterComponent(GameObject& gameobject, std::string succesScene, std::string loseScene);
        void Update() override;
        IObserver* GetPlayerPresenceObserver();
        IObserver* GetEnemyPresenceObserver();
        void PrepGameMaster(Scene& scene);
    private:
        std::unique_ptr<PlayerPresenceObserver> m_PlayerPresenceObserver;
        std::unique_ptr<EnemyPresenceObserver> m_EnemyPresenceObserver;
        std::string m_SuccesScene;
        std::string m_LoseScene;
    };
}
