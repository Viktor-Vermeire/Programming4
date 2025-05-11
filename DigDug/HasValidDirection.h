#pragma once
#include <Condition.h>
namespace dae {
    class HasValidDirection :
        public Condition
    {
    public:
        HasValidDirection() = default;
        bool IsMet(GameObject* gameObject);
    };
}

