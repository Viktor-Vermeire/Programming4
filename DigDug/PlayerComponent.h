#pragma once
#include "BaseComponent.h"
#include "ISubject.h"
#include "vector"
namespace dae {
    class PlayerComponent :
        public BaseComponent, public ISubject
    {
    public:
        PlayerComponent(GameObject& gameobject,std::string name);
        std::string GetName();
        int GetScore();
        void SetScore(int score);
        void AddObserver(IObserver* observer) override;
        void RemoveObserver(IObserver* observer) override;
        void Notify(Event event) override;
    private:
        std::string m_Name;
        int m_Score = 0;
        std::vector<IObserver*> m_Observers;

    };
}

