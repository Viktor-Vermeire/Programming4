#pragma once
#include <BaseComponent.h>
#include <vector>
namespace dae {
    class GameObject;
    class EnemyComponent : public BaseComponent
    {
    public:
        EnemyComponent(GameObject& gameObject, int maxInflation, int value, float timeTethered);
        void AddPlayers(GameObject* gameObject);
        void Update() override;
        std::vector<GameObject*> GetPlayers();
        bool GetTethered();
        void SetTethered(bool value);
        int GetValue();
        bool IncreaseInflation();
        void ResetInflation();
    private:
        std::vector<GameObject*> m_Players;
        float m_CurrentTimeTethered;
        float m_MaxTimeTethered;
        int m_MaxInflation;
        int m_Inflation;
        bool m_Tethered;
        int m_Value;
    };
}

