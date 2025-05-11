#pragma once
#include <BaseComponent.h>
#include "fwd.hpp"
#include "GameObject.h"
namespace dae {
    class FloatingComponent :
        public BaseComponent
    {
    public:
        FloatingComponent(GameObject& gameObject, float movementSpeed);
        void MoveTarget();
        void MoveToGrid();
        void SetGridTarget(Transform gridTarget);
        void SetPlayerTarget(GameObject* target);
    private:
        float m_MovementSpeed;
        GameObject* m_Target;
        Transform m_GridTarget;
    };
}

