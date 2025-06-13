#pragma once
#include <Condition.h>
namespace dae {
    class GameObject;
    class HasMovementInput : public Condition
    {
    public:
        HasMovementInput() = default;
        bool IsMet(GameObject* gameObject);
    };

    class HasNearbyHallway : public Condition
    {
    public:
        HasNearbyHallway(float timeFloating);
        bool IsMet(GameObject* gameObject);
    private:
        float m_MinTimeFloating;
    };

    class HasValidDirection :
        public Condition
    {
    public:
        HasValidDirection() = default;
        bool IsMet(GameObject* gameObject);
    };

    class IsDoneRunning : public Condition
    {
    public:
        IsDoneRunning() = default;
        bool IsMet(GameObject* gameObject);
    };

    class WantsToFloat :
        public Condition
    {
    public:
        WantsToFloat(float time);
        bool IsMet(GameObject* gameObject);
    private:
        float m_MaxTime;
    };

    class IsInGrid :
        public Condition
    {
    public:
        IsInGrid() = default;
        bool IsMet(GameObject* gameObject);
    };

    class WantsToAttack :
        public Condition
    {
    public:
        WantsToAttack() = default;
        bool IsMet(GameObject* gameObject);
    };

    class FinishedAttack :
        public Condition
    {
    public:
        FinishedAttack() = default;
        bool IsMet(GameObject* gameObject);
    };

    class FygarWantsToAttack :
    public Condition
    {
    public:
        FygarWantsToAttack() = default;
        bool IsMet(GameObject* gameObject);
    };

    class FygarFinishedAttack :
        public Condition
    {
    public:
        FygarFinishedAttack() = default;
        bool IsMet(GameObject* gameObject);
    };

    
    class IsTethered :
        public Condition
    {
    public:
        IsTethered() = default;
        bool IsMet(GameObject* gameObject);
    };
    class IsUntethered :
        public Condition
    {
    public:
        IsUntethered() = default;
        bool IsMet(GameObject* gameObject);
    };

}

