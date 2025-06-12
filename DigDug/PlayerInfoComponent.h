#pragma once
#include "IObserver.h"
#include "BaseComponent.h"
#include "memory"
namespace dae {
    class GameObject;
    class ScoreObserver;
    class HealthObserver;
    class PlayerInfoComponent :
        public BaseComponent
    {
    public:
        PlayerInfoComponent(GameObject& gameObject);
        void Update() override;
        int GetScore();
        int GetHealth();
        IObserver* GetScoreContainer();
        IObserver* GetHealthContainer();

    private:
        std::unique_ptr<ScoreObserver> m_ScoreContainer;
        std::unique_ptr<HealthObserver> m_HealthContainer;
    };
}

