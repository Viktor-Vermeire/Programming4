#include "SkipCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameMasterComponent.h"

dae::SkipCommand::SkipCommand(unsigned int inputValue, bool usingGamepad): Command(inputValue, usingGamepad)
{
}

void dae::SkipCommand::execute(GameObject&)
{
	auto results = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<GameMasterComponent>();
	if (results[0])
		results[0]->GetComponent<dae::GameMasterComponent>()->SetToBeSkipped(true);
}
