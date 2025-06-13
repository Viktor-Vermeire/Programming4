#pragma once
#include "BaseComponent.h"
#include "ISubject.h"
#include "vector"
namespace dae {
    class HealthComponent :
        public BaseComponent, public ISubject
    {
    public:
        HealthComponent(GameObject& gameObject, int health, int lives, float damageCooldown);
        void SetHealth(int health);
        void Update() override;
        int GetHealth();
        int GetLives();
        void AddObserver(IObserver* observer) override;
        void RemoveObserver(IObserver* observer) override;
        void Notify(Event event) override;
        void Die();
    private:
        int m_Health = 1;
        int m_Lives = 5;
        float m_DamageCooldown;
        float m_CooldownTimer;

        std::vector<IObserver*> m_Observers;
    };
}