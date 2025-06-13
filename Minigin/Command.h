#pragma once
namespace dae {


class GameObject;
class Command
{
public:
	Command(unsigned int inputValue, bool usingGamepad);
	int GetInputValue();
	bool IsUsingGamepad();
	void SetInputValue(int value);
	void SetUsingGamepad(bool usingGamepad);
	virtual ~Command() = default;
	virtual void execute(GameObject& gameObject) = 0;
private:
	unsigned int m_InputValue;
	bool m_UsingGamepad;
};
}

