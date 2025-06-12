#pragma once
#include <IObserver.h>
namespace dae {
    class GameObject;
    class ScoreObserver :
        public IObserver
    {
    public:
        ScoreObserver();
        void onNotify(GameObject& entity, Event event) override;
        //bool m_Triggered;
        int GetScore();
    private:
        int m_Score;
    };
}

