#pragma once
#include "map"
#include "vector"
#include "BaseComponent.h"
namespace dae {
	class State;
	class Condition;
	class FiniteStateMachineComponent : public BaseComponent
	{
	public:
		template <typename StateType, typename... Args>
		void AddState(std::string key, Args&&... args) {
			static_assert(std::is_base_of<State, StateType>::value, "StateType must be derived from State");
			m_States[key] = (std::make_unique<StateType>(std::forward<Args>(args)...));
		}

		template <typename ConditionType, typename... Args>
		void AddCondition(std::string key, Args&&... args) {
			static_assert(std::is_base_of<Condition, ConditionType>::value, "ConditionType must be derived from State");
			m_Conditions[key] = (std::make_unique<ConditionType>(std::forward<Args>(args)...));
		}
		FiniteStateMachineComponent(GameObject& gameObject);
		void AddTransition(State* fromState, State* toState, Condition* condition);
		void RemoveTransition(State* fromState, State* toState, Condition* condition);
		std::vector<std::pair<Condition*, State*>> GetToStates(State* fromState);
		std::vector<State*> GetStates();
		//void AddState(std::string name, State& state);
		//void AddCondition(std::string name, Condition& condition);
		void SetCurrentState(State* state);
		State* GetState(std::string name);
		Condition* GetCondition(std::string name);
		void RemoveState(std::string name);
		void RemoveCondition(std::string name);
		void Update() override;
	private:
		State* m_CurrentState;
		std::map<std::string,std::unique_ptr<State>> m_States;
		std::map<std::string, std::unique_ptr<Condition>> m_Conditions;
		std::map<State*, std::vector<std::pair<Condition*,State*>>> m_Transitions;
	};
}

