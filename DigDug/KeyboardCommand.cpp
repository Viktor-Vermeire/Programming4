#include "KeyboardCommand.h"
#include "GameObject.h"
#include "KeyboardComponent.h"

dae::KeyboardMoveCommand::KeyboardMoveCommand(unsigned int inputValue, bool usingGamepad, std::pair<float, float> change) : Command(inputValue, usingGamepad),
m_Change{change}
{
}

void dae::KeyboardMoveCommand::execute(GameObject& gameObject)
{
	auto keyboardComp = gameObject.GetComponent<dae::KeyboardComponent>();
	if (keyboardComp)
		keyboardComp->ChangeHighlightedLetter(m_Change);
}

dae::KeyboardConfirmCommand::KeyboardConfirmCommand(unsigned int inputValue, bool usingGamepad): Command(inputValue, usingGamepad)
{
}

void dae::KeyboardConfirmCommand::execute(GameObject& gameObject)
{
	auto keyboardComp = gameObject.GetComponent<dae::KeyboardComponent>();
	if (keyboardComp)
		keyboardComp->ConfirmLetter();
}

dae::SaveScoreCommand::SaveScoreCommand(unsigned int inputValue, bool usingGamepad) : Command(inputValue, usingGamepad)
{
}

void dae::SaveScoreCommand::execute(GameObject& gameObject)
{
	auto keyboardComp = gameObject.GetComponent<dae::KeyboardComponent>();
	if (keyboardComp)
		keyboardComp->SaveScore();
}
