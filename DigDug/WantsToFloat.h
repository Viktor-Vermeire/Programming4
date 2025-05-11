#pragma once
#include <Condition.h>
namespace dae {
    class GameObject;
    class WantsToFloat :
        public Condition
    {
    public:
        WantsToFloat(float time);
        bool IsMet(GameObject* gameObject);
    private:
        float m_MaxTime;
    };
}

