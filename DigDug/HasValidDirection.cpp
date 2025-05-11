#include "HasValidDirection.h"
#include "GameObject.h"
#include "RenderComponent.h"

bool dae::HasValidDirection::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<RenderComponent>();
    if (comp) {

    }
    return false;
}
