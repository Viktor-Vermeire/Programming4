#pragma once
#include <BaseComponent.h>
namespace dae {
    class HallwaysComponent;
    class GameObject;
    class DiggingComponent :
        public BaseComponent
    {
    public:
        DiggingComponent(GameObject& gameObject, HallwaysComponent* component);
        void Dig();
    private:
        HallwaysComponent* m_pHallwaysComponent;
    };
}

