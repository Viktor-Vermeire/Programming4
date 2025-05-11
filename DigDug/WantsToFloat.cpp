#include "WantsToFloat.h"
#include "random"
#include "GameObject.h"
#include "FiniteStateMachineComponent.h"

dae::WantsToFloat::WantsToFloat(float time): m_MaxTime{time}
{
}

bool dae::WantsToFloat::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FiniteStateMachineComponent>();
    if (comp)
        return comp->GetTimeInCurrentState() > m_MaxTime;
    return false;
}
