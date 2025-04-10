#pragma once
#include "Singleton.h"
#include "Command.h"
#include "vector"
#include "memory"

namespace dae
{
	class Gamepad;
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddGamepad(Gamepad* gamepad);
		void AddGameActor(GameObject* gameActor);
		int GetGameActorSize();

		template <typename CommandType, typename... Args>
		void AddCommand(Args&&... args) {
			static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
			m_Commands.push_back(std::make_unique<CommandType>(std::forward<Args>(args)...));
		}
	private:
		std::vector<std::unique_ptr<Command>> m_Commands;
		std::vector<Gamepad*> m_GamePads;
		std::vector<GameObject*> m_GameActors;
	};


}
