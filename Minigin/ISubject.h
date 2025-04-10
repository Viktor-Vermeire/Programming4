#pragma once
#include "IObserver.h"
namespace dae {
    class ISubject {
    public:
        virtual ~ISubject() {}
        virtual void AddObserver(IObserver* observer) = 0;
        virtual void RemoveObserver(IObserver* observer) = 0;
        virtual void Notify(Event event) = 0;
    };
}

