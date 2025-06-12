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
        void SetGridTarget(glm::vec3 gridTarget);
        void SetPlayerTarget(GameObject* target);
        void SetInGrid(bool value);
        bool InGrid();
        float GetTimeFloating();
    private:
        float m_TimeFloating;
        float m_MovementSpeed;
        GameObject* m_Target;
        glm::vec3 m_GridTarget;
        bool m_InGrid;
    };
}

