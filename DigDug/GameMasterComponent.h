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
        void SetToBeSkipped(bool value);
    private:
        std::unique_ptr<PlayerPresenceObserver> m_PlayerPresenceObserver;
        std::unique_ptr<EnemyPresenceObserver> m_EnemyPresenceObserver;
        float m_StartUpCooldown;
        float m_RemainingCooldown;
        std::string m_SuccesScene;
        bool m_ToBeSkipped{ false };
        std::string m_LoseScene;
    };
}
