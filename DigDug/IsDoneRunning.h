#pragma once
#include <Condition.h>
namespace dae {
    class IsDoneRunning : public Condition
    {
    public:
        IsDoneRunning() = default;
        bool IsMet(GameObject* gameObject);
    };
}

