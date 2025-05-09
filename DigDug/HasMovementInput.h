#pragma once
#include "Condition.h"
namespace dae {
    class GameObject;
    class HasMovementInput : public Condition
    {
    public:
        HasMovementInput() = default;
        bool IsMet(GameObject* gameObject);
    };
}

