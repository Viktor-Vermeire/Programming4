#pragma once
#include <BaseComponent.h>
#include <vector>
namespace dae {
    class GameObject;
    class EnemyComponent : public BaseComponent
    {
    public:
        EnemyComponent(GameObject& gameObject);
        void AddPlayers(GameObject* gameObject);
        std::vector<GameObject*> GetPlayers();
    private:
        std::vector<GameObject*> m_Players;
    };
}

