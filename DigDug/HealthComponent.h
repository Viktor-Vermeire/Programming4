#pragma once
#include "BaseComponent.h"
#include "ISubject.h"
#include "vector"
namespace dae {
    class HealthComponent :
        public BaseComponent, public ISubject
    {
    public:
        HealthComponent(GameObject& gameObject, int health, int lives);
        void SetHealth(int health);
        int GetHealth();
        int GetLives();
        void AddObserver(IObserver* observer) override;
        void RemoveObserver(IObserver* observer) override;
        void Notify(Event event) override;
        void Die();
    private:
        int m_Health = 1;
        int m_Lives = 5;

        std::vector<IObserver*> m_Observers;
    };
}