#pragma once
#include <IObserver.h>
namespace dae {
    class HealthObserver :
        public IObserver
    {
    public:
        HealthObserver();
        void onNotify(GameObject& entity, Event event) override;
        int GetHealth();
    private:
        int m_Health;
    };
}

