#pragma once
#include <BaseComponent.h>
namespace dae {
    class GameObject;
    class RockComponent :public BaseComponent
    {
    public:
        RockComponent(GameObject& gameObject);
    };
}

