#pragma once
#include <BaseComponent.h>
namespace dae {
    class GameObject;
    class DiggingComponent :
        public BaseComponent
    {
    public:
        DiggingComponent(GameObject& gameObject);
        void Dig();
    };
}

