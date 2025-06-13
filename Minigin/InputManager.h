#pragma once
#include "Singleton.h"
#include "Command.h"
#include "vector"
#include "memory"
#include "Gamepad.h"
#include "map"

namespace dae
{
	class GameObject;
	class InputManager final : public Singleton<InputManager>
	{
	public:
		//bool ProcessInput();
		bool ProcessPlayerInput(GameObject* player, Gamepad* gamepad, std::vector<Command*> commands);
		void AddGamepad(std::string key, std::unique_ptr<Gamepad> gamepad);
		void AddGameActor(std::string key,std::shared_ptr<GameObject> gameActor);
		//Command* GetCommand(std::string key);
		int GetGameActorSize();
		Command* GetCommand(std::string key);
		Gamepad* GetGamePad(std::string key);
		std::shared_ptr<GameObject> GetGameActor(std::string key);

		bool CheckExit();

		template <typename CommandType, typename... Args>
		void AddCommand(std::string key,Args&&... args) {
			static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
			//m_Commands.push_back(std::make_unique<CommandType>(std::forward<Args>(args)...));
			m_Commands[key] = (std::make_unique<CommandType>(std::forward<Args>(args)...));
		}
	private:
		//std::vector<std::unique_ptr<Command>> m_Commands;
		std::map<std::string,std::unique_ptr<Gamepad>> m_GamePads;
		std::map<std::string ,std::unique_ptr<Command>> m_Commands;
		std::map<std::string, std::shared_ptr<GameObject>> m_GameActors;
	};


}
