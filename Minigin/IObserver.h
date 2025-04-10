#pragma once
namespace dae {
	class GameObject;
	enum Event {DEATH, SCORE};
	class IObserver
	{
	public:
		virtual ~IObserver() {};
		virtual void onNotify(GameObject& entity, Event event) = 0;
	};

	
}

