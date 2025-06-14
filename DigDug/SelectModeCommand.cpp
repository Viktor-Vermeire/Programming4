#include "SelectModeCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ServiceLocator.h"

dae::SelectModeCommand::SelectModeCommand(unsigned int inputValue, bool usingGamepad, const std::string& sceneName, int songIndex): Command(inputValue, usingGamepad),
m_SceneName{sceneName}, m_SongIndex{static_cast<unsigned short>(songIndex)}
{
}

void dae::SelectModeCommand::execute(GameObject& )
{
	ServiceLocator::get_SoundSystem().PlayMusic(m_SongIndex, 2.f);
	dae::SceneManager::GetInstance().SetToSwitchScene(*dae::SceneManager::GetInstance().GetScene(m_SceneName));
}
