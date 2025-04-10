#pragma once
#include "IObserver.h"
#include "BaseComponent.h"
namespace dae {
    class GameObject;
    class PlayerInfoComponent :
        public BaseComponent, public IObserver
    {
    public:
        PlayerInfoComponent(GameObject& gameObject);
        void onNotify(GameObject& entity, Event event) override;
    private:
    };
}

