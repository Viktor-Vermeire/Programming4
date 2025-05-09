#include "FiniteStateMachineComponent.h"
#include "State.h"
#include "Condition.h"
#include "map"
using namespace dae;
dae::FiniteStateMachineComponent::FiniteStateMachineComponent(GameObject& gameObject): BaseComponent(gameObject)
{
}
void dae::FiniteStateMachineComponent::AddTransition(State* fromState, State* toState, Condition* condition)
{
	m_Transitions[fromState].emplace_back(std::pair<Condition*, State*>{ condition, toState });
}

void dae::FiniteStateMachineComponent::RemoveTransition(State* fromState, State* toState, Condition* condition)
{
	
	m_Transitions[fromState].erase(
		std::remove_if(m_Transitions[fromState].begin(), m_Transitions[fromState].end(),
			[toState, condition](const std::pair<Condition*, State*>& element) {
				return element.first == condition && element.second == toState;
			}),
		m_Transitions[fromState].end());
}

std::vector<std::pair<dae::Condition*,dae::State*>> dae::FiniteStateMachineComponent::GetToStates(State* fromState)
{
	return m_Transitions[fromState];
}

std::vector<dae::State*> dae::FiniteStateMachineComponent::GetStates()
{
	/*std::vector<dae::State*> states;
	for (unsigned int stateCounter{ 0 }; stateCounter < m_States.size(); ++stateCounter) {
		states.emplace_back(m_States[stateCounter].get());
	}
	return states;*/
	return {};
}

/*void dae::FiniteStateMachineComponent::AddState(std::string name, State& state)
{
	m_States[name] = (std::make_unique<State>(state));
}*/

/*void dae::FiniteStateMachineComponent::AddCondition(std::string name, Condition& condition)
{
	m_Conditions[name] = (std::make_unique<Condition>(condition));
}*/

void dae::FiniteStateMachineComponent::SetCurrentState(State* state)
{
	m_CurrentState = state;
}

State* dae::FiniteStateMachineComponent::GetState(std::string name)
{
	return m_States[name].get();
}

Condition* dae::FiniteStateMachineComponent::GetCondition(std::string name)
{
	return m_Conditions[name].get();
}

void dae::FiniteStateMachineComponent::RemoveState(std::string name)
{
	m_States.erase(name);
}

void dae::FiniteStateMachineComponent::RemoveCondition(std::string name)
{
	m_Conditions.erase(name);
}

void dae::FiniteStateMachineComponent::Update()
{
	m_CurrentState->Execute(GetOwner());
	std::vector<std::pair<dae::Condition*, dae::State*>> options = GetToStates(m_CurrentState);
	for (auto option : options) {
		
		if (option.first->IsMet(GetOwner())) {
			m_CurrentState->Exit(GetOwner());
			option.second->Enter(GetOwner());
			m_CurrentState = option.second;
			break;
		}
	}
}
