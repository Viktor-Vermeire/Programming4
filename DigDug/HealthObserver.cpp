#include "HealthObserver.h"

dae::HealthObserver::HealthObserver()
{
    m_Health = 0;
}

void dae::HealthObserver::onNotify(GameObject& , Event )
{
}

int dae::HealthObserver::GetHealth()
{
    return m_Health;
}
