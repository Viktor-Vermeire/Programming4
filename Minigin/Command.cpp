#include "Command.h"

dae::Command::Command(int inputValue, bool usingGamepad): m_InputValue{inputValue}, m_UsingGamepad{usingGamepad}
{
}

int dae::Command::GetInputValue()
{
	return m_InputValue;
}

bool dae::Command::IsUsingGamepad()
{
	return m_UsingGamepad;
}

void dae::Command::SetInputValue(int value)
{
	m_InputValue = value;
}

void dae::Command::SetUsingGamepad(bool usingGamepad)
{
	m_UsingGamepad = usingGamepad;
}
