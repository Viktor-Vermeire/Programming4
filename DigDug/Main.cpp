#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "RotatorComponent.h"
#include "PlayerComponent.h"
#include "PlayerInfoComponent.h"
#include "HealthComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "Gamepad.h"
#include "ThrashTheCacheComponent.h"
#include "iostream"
#include "Move.h"
#include "Suicide.h"
#include "Pickup.h"
#include "PlaySound.h"
#include "DiggingComponent.h"
#include "RockComponent.h"
#include "HallwaysComponent.h"
#include "ServiceLocator.h"
#include "AnimationComponent.h"
#include "FiniteStateMachineComponent.h"
#include "HandleInput.h"
#include "States.h"
#include "Conditions.h"
#include "FloatingComponent.h"
#include "DigDugAttackComponent.h"
#include "DigDugAttackCommand.h"
#include "EnemyComponent.h"
#include "Pump.h"
#include "SelectModeCommand.h"
#include "SDLSoundSystem.h"
#include "FygarAttackComponent.h"
#include "FygarAttackCommand.h"
#include "GameMasterComponent.h"
#include "KeyboardComponent.h"
#include "KeyboardCommand.h"

namespace fs = std::filesystem;

void InitializeCommands(dae::InputManager& input)
{
	input.AddCommand<dae::Move>("GamepadUp", 1u, true, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>("GamepadDown", 2u, true, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>("GamepadLeft", 4u, true, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>("GamepadRight", 8u, true, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Move>("KeyboardUp", SDL_SCANCODE_W, false, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>("keyboardDown", SDL_SCANCODE_S, false, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>("KeyboardLeft", SDL_SCANCODE_A, false, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>("KeyboardRight", SDL_SCANCODE_D, false, dae::RenderComponent::RIGHT);
	
	input.AddCommand<dae::Pump>("KeyboardPump", SDL_SCANCODE_E, false);
	input.AddCommand<dae::Pump>("GamepadPump", 4096u, true);
	input.AddCommand<dae::DigDugAttackCommand>("KeyboardShootTether",SDL_SCANCODE_E, false);
	input.AddCommand<dae::DigDugAttackCommand>("GamepadShootTether", 4096u, true);
	input.AddCommand<dae::FygarAttackCommand>("GamepadFygarAttack", 4096u, true);
	input.AddCommand<dae::FygarAttackCommand>("KeyboardFygarAttack", SDL_SCANCODE_E, false);

	input.AddCommand<dae::Suicide>("GamepadSuicide", 32768u, true);
	input.AddCommand<dae::Suicide>("KeyboardSuicide", SDL_SCANCODE_C, false);
	input.AddCommand<dae::Pickup>("GamepadPickupSmall", 16384u, true, 10);
	input.AddCommand<dae::Pickup>("KeyboardPickupSmall", SDL_SCANCODE_Z, false, 10);
	input.AddCommand<dae::Pickup>("GamepadPickupLarge", 8192u, true, 100);
	input.AddCommand<dae::Pickup>("KeyboardPickupLarge", SDL_SCANCODE_X, false, 100);
	input.AddCommand<dae::PlaySound>("KeyboardPlaySound", SDL_SCANCODE_F, false, 0);
	input.AddCommand<dae::PlaySound>("GamepadPlaySound", 4096u, true, 0);

	input.AddCommand<dae::SelectModeCommand>("SelectModeSingle", 4096u, true, "Single", 1);
	input.AddCommand<dae::SelectModeCommand>("SelectModeCoop", 8192u, true, "Coop", 2);
	input.AddCommand<dae::SelectModeCommand>("SelectModePvP", 16384u, true, "PvP", 1);

	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardUp", 1u, true, std::pair{ 0.f,-1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardDown", 2u, true, std::pair{ 0.f,1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardLeft", 4u, true, std::pair{ -1.f,0.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardRight", 8u, true, std::pair{ 1.f,0.f });
	input.AddCommand<dae::KeyboardConfirmCommand>("GamepadKeyboardConfirm", 4096u, true);
	input.AddCommand<dae::SaveScoreCommand>("GamepadKeyboardSave", 16u, true);

	input.AddCommand<dae::KeyboardMoveCommand>("KeyboardKeyboardUp", SDL_SCANCODE_W, false, std::pair{ 0.f,-1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("KeyboardKeyboardDown", SDL_SCANCODE_S, false, std::pair{ 0.f,1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("KeyboardKeyboardLeft", SDL_SCANCODE_A, false, std::pair{ -1.f,0.f });
	input.AddCommand<dae::KeyboardMoveCommand>("KeyboardKeyboardRight", SDL_SCANCODE_D, false, std::pair{ 1.f,0.f });
	input.AddCommand<dae::KeyboardConfirmCommand>("KeyboardKeyboardConfirm", SDL_SCANCODE_E, false);
	input.AddCommand<dae::SaveScoreCommand>("KeyboardKeyboardSave", SDL_SCANCODE_Q, false);

	/*input.AddCommand<dae::Move>(1, true, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>(2, true, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>(4, true, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>(8, true, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Move>(SDL_SCANCODE_W, false, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>(SDL_SCANCODE_S, false, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>(SDL_SCANCODE_A, false, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>(SDL_SCANCODE_D, false, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Pump>(SDL_SCANCODE_E, false);

	input.AddCommand<dae::DigDugAttackCommand>(SDL_SCANCODE_E, false);
	input.AddCommand<dae::Suicide>(32768, true);
	input.AddCommand<dae::Suicide>(SDL_SCANCODE_C, false);
	input.AddCommand<dae::Pickup>(16384, true, 10);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_Z, false, 10);
	input.AddCommand<dae::Pickup>(8192, true, 100);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_X, false, 100);
	input.AddCommand<dae::PlaySound>(SDL_SCANCODE_F, false, 0);
	input.AddCommand<dae::PlaySound>(4096, true, 0);*/
}
struct PlayerInfo {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	std::vector<SDL_Rect> runningAnimLocations;
	int startPos[2];
	std::string name;
	dae::Scene& scene; 
	dae::GameObject* hallways;
	dae::GameObject* playerStatDisplay;
	dae::GameObject* gameMaster;
};
struct FygarPlayerInfo {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	std::vector<SDL_Rect> runningAnimLocations;
	int startPos[2];
	std::string name;
	dae::Scene& scene;
	dae::GameObject* hallways;
	dae::GameObject* playerStatDisplay;
	std::vector<SDL_Rect> fygarFlameRects;
};
struct PlayerInfoTemp {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	std::vector<SDL_Rect> runningAnimLocations;
	int startPos[2];
	std::string name;
};
struct FygarPlayerInfoTemp {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	std::vector<SDL_Rect> runningAnimLocations;
	int startPos[2];
	std::string name;
	std::vector<SDL_Rect> fygarFlameRects;
};
struct ControllerInfo {
	std::vector<std::string> gamepadNames;
	dae::InputManager& input;
};
struct ScoreInfo {
	std::shared_ptr<dae::Font> font; 
	std::shared_ptr<dae::GameObject> scoreBoardGo;
};
struct EnemyInfo {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	int startPos[2];
	std::vector<SDL_Rect> runningAnimLocations;
	std::vector<SDL_Rect> floatingAnimLocations;
	dae::Scene& scene;
	dae::GameObject* hallways;
	std::vector<SDL_Rect> fygarFlameRects;
};

/*
void SetupPlayer(const PlayerInfo& playerInfo, const ControllerInfo& controllerInfo, const ScoreInfo&) {
	auto go = std::make_shared<dae::GameObject>();
	go->SetParent(playerInfo.hallways);
	go->AddComponent<dae::RenderComponent>(*go.get());
	
	//FiniteStateMachineSetup
	{
		go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
		auto FiniteComp = go->GetComponent<dae::FiniteStateMachineComponent>();
		{
			dae::Gamepad* gamepad = controllerInfo.input.GetGamePad(controllerInfo.gamepadName);
			FiniteComp->AddState<dae::HandleInput>("HandleInput", gamepad);
			{
				auto handleInputState = static_cast<dae::HandleInput*>(FiniteComp->GetState("HandleInput"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("keyboardDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardShootTether"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadShootTether"));
			}
			FiniteComp->AddState<dae::Running>("Running", playerInfo.runningAnimLocations, 0.3f);
			FiniteComp->AddState<dae::DigDugAttack>("DigDugAttacking", controllerInfo.input.GetGamePad(controllerInfo.gamepadName));

			static_cast<dae::DigDugAttack*>(FiniteComp->GetState("DigDugAttacking"))->AddCommand(controllerInfo.input.GetCommand("KeyboardPump"));
			static_cast<dae::DigDugAttack*>(FiniteComp->GetState("DigDugAttacking"))->AddCommand(controllerInfo.input.GetCommand("GamepadPump"));
			FiniteComp->AddCondition<dae::HasMovementInput>("HasMovementInput");
			FiniteComp->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
			FiniteComp->AddCondition < dae::WantsToAttack>("WantsToAttack");
			FiniteComp->AddCondition<dae::FinishedAttack>("FinishedAttack");

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("Running"),
				FiniteComp->GetCondition("HasMovementInput"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("Running"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("IsDoneRunning"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("DigDugAttacking"),
				FiniteComp->GetCondition("WantsToAttack"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("DigDugAttacking"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("FinishedAttack"));

			FiniteComp->SetCurrentState(
				FiniteComp->GetState("HandleInput")
			);
		}
	}
	
	//MovementComponent setup
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->GetComponent<dae::MovementComponent>()->SetIsDigger(true);

	go->GetComponent<dae::RenderComponent>()->SetTexture(playerInfo.texturePath, playerInfo.textureSrcRect);
	go->SetPosition(static_cast<float>(playerInfo.startPos[0]), static_cast<float>(playerInfo.startPos[1]));

	go->AddComponent<dae::DigDugAttackComponent>(*go.get(), SDL_Rect(32, 56, 32, 5), playerInfo.texturePath, 0.3f, 0.2f);

	go->AddComponent<dae::PlayerComponent>(*go.get(), playerInfo.name);
	go->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
	go->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo.gameMaster->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8,1.f);
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.gameMaster->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
	
	playerInfo.scene.Add(go);
	controllerInfo.input.AddGameActor(playerInfo.name,go.get());
	
}*/
void SetupGeneralPlayer(const PlayerInfoTemp& playerInfo, const ControllerInfo& controllerInfo) {
		auto player = std::make_unique<dae::GameObject>();

	player->AddComponent<dae::RenderComponent>(*player.get());
	//FiniteStateMachineSetup
	{
		player->AddComponent<dae::FiniteStateMachineComponent>(*player.get());
		auto FiniteComp = player->GetComponent<dae::FiniteStateMachineComponent>();
		{
			
			
			FiniteComp->AddState<dae::HandleInput>("HandleInput");
			
			{
				auto handleInputState = static_cast<dae::HandleInput*>(FiniteComp->GetState("HandleInput"));
				for (auto name : controllerInfo.gamepadNames) {
					dae::Gamepad* gamepad = controllerInfo.input.GetGamePad(name);
					handleInputState->AddGamepad(gamepad);
				}

				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("keyboardDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardShootTether"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadShootTether"));
			}
			FiniteComp->AddState<dae::Running>("Running", playerInfo.runningAnimLocations, 0.3f);
			FiniteComp->AddState<dae::DigDugAttack>("DigDugAttacking");
			auto digDugAttack = static_cast<dae::DigDugAttack*>(FiniteComp->GetState("DigDugAttacking"));
			for (auto name : controllerInfo.gamepadNames) {
				dae::Gamepad* gamepad = controllerInfo.input.GetGamePad(name);
				digDugAttack->AddGamepad(gamepad);
			}
			digDugAttack->AddCommand(controllerInfo.input.GetCommand("KeyboardPump"));
			digDugAttack->AddCommand(controllerInfo.input.GetCommand("GamepadPump"));
			FiniteComp->AddCondition<dae::HasMovementInput>("HasMovementInput");
			FiniteComp->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
			FiniteComp->AddCondition < dae::WantsToAttack>("WantsToAttack");
			FiniteComp->AddCondition<dae::FinishedAttack>("FinishedAttack");

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("Running"),
				FiniteComp->GetCondition("HasMovementInput"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("Running"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("IsDoneRunning"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("DigDugAttacking"),
				FiniteComp->GetCondition("WantsToAttack"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("DigDugAttacking"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("FinishedAttack"));

			FiniteComp->SetCurrentState(
				FiniteComp->GetState("HandleInput")
			);
		}
	}
	//MovementComponent setup
	player->AddComponent<dae::MovementComponent>(*player.get());
	player->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	player->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	player->GetComponent<dae::MovementComponent>()->SetIsDigger(true);

	player->GetComponent<dae::RenderComponent>()->SetTexture(playerInfo.texturePath, playerInfo.textureSrcRect);
	player->SetPosition(static_cast<float>(playerInfo.startPos[0]), static_cast<float>(playerInfo.startPos[1]));

	player->AddComponent<dae::DigDugAttackComponent>(*player.get(), SDL_Rect(32, 56, 32, 5), playerInfo.texturePath, 0.3f, 0.2f);


	player->AddComponent<dae::PlayerComponent>(*player.get(), playerInfo.name);
	player->AddComponent<dae::HealthComponent>(*player.get(), 1, 8, 1.f);

	controllerInfo.input.AddGameActor(playerInfo.name, std::move(player));
}
/*
void SetupFygarPlayer(const FygarPlayerInfo& playerInfo, const ControllerInfo& controllerInfo, const ScoreInfo&) {
	auto go = std::make_shared<dae::GameObject>();
	go->SetParent(playerInfo.hallways);
	go->AddComponent<dae::RenderComponent>(*go.get());
	{
		go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
		auto FiniteComp = go->GetComponent<dae::FiniteStateMachineComponent>();
		{
			FiniteComp->AddState<dae::HandleInput>("HandleInput");
			{
				auto handleInputState = static_cast<dae::HandleInput*>(FiniteComp->GetState("HandleInput"));
				for (auto name : controllerInfo.gamepadNames) {
					handleInputState->AddGamepad(controllerInfo.input.GetGamePad(name));
				}
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("keyboardDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardShootTether"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadFygarAttack"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardFygarAttack"));
			}
			FiniteComp->AddState<dae::Running>("Running", playerInfo.runningAnimLocations, 0.3f);
			FiniteComp->AddState<dae::FygarAttack>("FygarAttacking");
			
			FiniteComp->AddCondition<dae::HasMovementInput>("HasMovementInput");
			FiniteComp->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
			FiniteComp->AddCondition<dae::FygarWantsToAttack>("FygarWantsToAttack");
			FiniteComp->AddCondition<dae::FygarFinishedAttack>("FygarFinishedAttack");

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("Running"),
				FiniteComp->GetCondition("HasMovementInput"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("Running"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("IsDoneRunning"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("FygarAttacking"),
				FiniteComp->GetCondition("FygarWantsToAttack"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("FygarAttacking"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("FygarFinishedAttack"));

			FiniteComp->SetCurrentState(
				FiniteComp->GetState("HandleInput")
			);
		}
	}
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->GetComponent<dae::MovementComponent>()->SetIsDigger(false);

	

	go->GetComponent<dae::RenderComponent>()->SetTexture(playerInfo.texturePath, playerInfo.textureSrcRect);
	go->AddComponent<dae::FygarAttackComponent>(*go.get(), playerInfo.fygarFlameRects, playerInfo.texturePath, 1.f);
	go->SetPosition(static_cast<float>(playerInfo.startPos[0]), static_cast<float>(playerInfo.startPos[1]));

	go->AddComponent<dae::EnemyComponent>(*go.get(), 4, 100,3.f);
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8, 1.f);
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
	
	playerInfo.scene.Add(go);
	controllerInfo.input.AddGameActor(playerInfo.name,go.get());
}*/
void SetupGeneralFygarPlayer(const FygarPlayerInfoTemp& playerInfo, const ControllerInfo& controllerInfo) {
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(*go.get());
	{
		go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
		auto FiniteComp = go->GetComponent<dae::FiniteStateMachineComponent>();
		{
			FiniteComp->AddState<dae::HandleInput>("HandleInput");
			{
				auto handleInputState = static_cast<dae::HandleInput*>(FiniteComp->GetState("HandleInput"));
				for (auto name : controllerInfo.gamepadNames) {
					handleInputState->AddGamepad(controllerInfo.input.GetGamePad(name));
				}
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("keyboardDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardShootTether"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadUp"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadDown"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadLeft"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadRight"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadFygarAttack"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardFygarAttack"));
			}
			FiniteComp->AddState<dae::Running>("Running", playerInfo.runningAnimLocations, 0.3f);
			FiniteComp->AddState<dae::FygarAttack>("FygarAttacking");

			FiniteComp->AddCondition<dae::HasValidDirectionAndWantsToMove>("HasMovementInput");
			FiniteComp->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
			FiniteComp->AddCondition<dae::FygarWantsToAttack>("FygarWantsToAttack");
			FiniteComp->AddCondition<dae::FygarFinishedAttack>("FygarFinishedAttack");

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("Running"),
				FiniteComp->GetCondition("HasMovementInput"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("Running"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("IsDoneRunning"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetState("FygarAttacking"),
				FiniteComp->GetCondition("FygarWantsToAttack"));

			FiniteComp->AddTransition(
				FiniteComp->GetState("FygarAttacking"),
				FiniteComp->GetState("HandleInput"),
				FiniteComp->GetCondition("FygarFinishedAttack"));

			FiniteComp->SetCurrentState(
				FiniteComp->GetState("HandleInput")
			);
		}
	}
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->GetComponent<dae::MovementComponent>()->SetIsDigger(false);



	go->GetComponent<dae::RenderComponent>()->SetTexture(playerInfo.texturePath, playerInfo.textureSrcRect);
	go->AddComponent<dae::FygarAttackComponent>(*go.get(), playerInfo.fygarFlameRects, playerInfo.texturePath, 1.f);
	go->SetPosition(static_cast<float>(playerInfo.startPos[0]), static_cast<float>(playerInfo.startPos[1]));

	go->AddComponent<dae::EnemyComponent>(*go.get(), 4, 100, 3.f);
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8, 1.f);

	controllerInfo.input.AddGameActor(playerInfo.name, std::move(go));
}
void SetupEnemy(EnemyInfo enemyInfo) {
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(*go.get());
	//SetupFiniteStateMachine (maybe seperate method later)
	{
		go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::FloatingToPlayer>("FloatingToPlayer", enemyInfo.floatingAnimLocations, 0.3f);
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::FloatingToGrid>("FloatingToGrid", enemyInfo.floatingAnimLocations, 0.3f);
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::Running>("Running", enemyInfo.runningAnimLocations, 0.3f);
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::Idle>("Idle");
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::Tethered>("Tethered");

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::WantsToFloat>("WantsToFloat", 5.0f); // reset later
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::HasNearbyHallway>("HasNearbyHallway", 2.0f);
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::HasValidDirection>("HasValidDirection");
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsInGrid>("IsInGrid");
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsTethered>("IsTethered");
		go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsUntethered>("IsUntethered");

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("HasValidDirection"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsDoneRunning"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("FloatingToPlayer"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("WantsToFloat"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("FloatingToPlayer"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("FloatingToGrid"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("HasNearbyHallway"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("FloatingToGrid"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsInGrid"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Tethered"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsTethered"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Tethered"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsUntethered"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Tethered"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsUntethered"));

		go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Tethered"),
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsTethered"));

		


		go->GetComponent<dae::FiniteStateMachineComponent>()->SetCurrentState(
			go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Idle"));
	}
	//MovementComponent setup
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->AddComponent<dae::FloatingComponent>(*go.get(), 30.f);

	if (!enemyInfo.fygarFlameRects.empty()) {
		go->AddComponent<dae::FygarAttackComponent>(*go.get(), enemyInfo.fygarFlameRects, enemyInfo.texturePath, 1.f);
	}
	go->SetParent(enemyInfo.hallways);


	go->GetComponent<dae::RenderComponent>()->SetTexture(enemyInfo.texturePath, enemyInfo.textureSrcRect);
	go->SetPosition(static_cast<float>(enemyInfo.startPos[0]), static_cast<float>(enemyInfo.startPos[1]));


	/*std::unique_ptr gamepad = std::make_unique<dae::Gamepad>(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(std::move(gamepad));*/


	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8,1.f);
	go->AddComponent<dae::EnemyComponent>(*go.get(), 3, 100, 3.f);
	//auto healthComp = go->GetComponent<dae::HealthComponent>();
	//go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	enemyInfo.scene.Add(std::move(go));
}

void SetupHallwaySources(dae::GameObject* go) {
	go->GetComponent<dae::HallwaysComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png");
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::TOPCLOSED, SDL_Rect(1, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::BOTTOMCLOSED, SDL_Rect(19, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::LEFTCLOSED, SDL_Rect(37, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::RIGHTCLOSED, SDL_Rect(55, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::VERTICALTHROUGH, SDL_Rect(73, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::HORIZONTALTHROUGH, SDL_Rect(91, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::LEFTTOPCORNER, SDL_Rect(109, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::RIGHTTOPCORNER, SDL_Rect(127, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::LEFTBOTTOMCORNER, SDL_Rect(145, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::RIGHTBOTTOMCORNER, SDL_Rect(163, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::CLEARED, SDL_Rect(163, 117, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::FILLED, SDL_Rect(181, 99, 16, 16));
	go->GetComponent<dae::HallwaysComponent>()->AddSource(dae::HallwaysComponent::SKY, SDL_Rect(181, 99, 0, 0));
}

void SetupTunnels(dae::GameObject* go) {
	auto comp = go->GetComponent<dae::HallwaysComponent>();
	if (comp) {
		comp->SetHallwayType(std::pair<int, int>(10, 10), dae::HallwaysComponent::LEFTTOPCORNER);
		comp->SetHallwayType(std::pair<int, int>(11, 10), dae::HallwaysComponent::VERTICALTHROUGH);
		comp->SetHallwayType(std::pair<int, int>(12, 10), dae::HallwaysComponent::BOTTOMCLOSED);
		comp->SetHallwayType(std::pair<int, int>(10, 11), dae::HallwaysComponent::RIGHTCLOSED);
		comp->SetHallwayType(std::pair<int, int>(10, 18), dae::HallwaysComponent::LEFTTOPCORNER);
		comp->SetHallwayType(std::pair<int, int>(11, 18), dae::HallwaysComponent::VERTICALTHROUGH);
		comp->SetHallwayType(std::pair<int, int>(12, 18), dae::HallwaysComponent::BOTTOMCLOSED);
		comp->SetHallwayType(std::pair<int, int>(10, 19), dae::HallwaysComponent::RIGHTCLOSED);
	}
}

void SetupKeyboard(dae::Scene& scene, std::shared_ptr<dae::Font> font, int lettersPerLine, dae::InputManager& input) {
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(dae::Renderer::GetInstance().GetSDLWindow(), &width, &height);
	auto keyboardParent = std::make_unique<dae::GameObject>();
	keyboardParent->AddComponent<dae::KeyboardComponent>(*keyboardParent.get(), 0.2f);
	auto keyboardComp = keyboardParent->GetComponent<dae::KeyboardComponent>();
	keyboardComp->SetHighLightSize({ 36,36 });
	keyboardComp->SetFileName("../Data/HighScoreList.bin");
	keyboardParent->AddComponent<dae::TextComponent>(*keyboardParent.get(), "", font);
	auto offset = std::pair<float, float>(20.f, -30.f);
	keyboardParent->GetComponent<dae::TextComponent>()->SetOffset(offset);
	keyboardParent->AddComponent<dae::FiniteStateMachineComponent>(*keyboardParent.get());
	auto finiteComp = keyboardParent->GetComponent<dae::FiniteStateMachineComponent>();
	finiteComp->AddState<dae::HandleInput>("HandleInput");
	auto handleInput = static_cast<dae::HandleInput*>(finiteComp->GetState("HandleInput"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardUp"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardDown"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardLeft"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardRight"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardConfirm"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardSave"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardUp"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardDown"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardLeft"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardRight"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardConfirm"));
	handleInput->AddCommand(input.GetCommand("KeyboardKeyboardSave"));
	handleInput->AddGamepad(input.GetGamePad("GamepadPlayer1"));
	handleInput->AddGamepad(input.GetGamePad("KeyboardPlayer1"));
	finiteComp->SetCurrentState(handleInput);
	auto keyboardParentPtr = keyboardParent.get();
	scene.Add(std::move(keyboardParent));
	std::vector<std::string> letters = { "A", "B","C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
	"O", "P","Q", "R", "S", "V", "U", "W", "T", "X", "Y", "Z" };
	auto numberOflines = ((letters.size() - std::fmodf(static_cast<float>(letters.size()), static_cast<float>(lettersPerLine))) / lettersPerLine) + 1;
	keyboardParentPtr->SetPosition((width - (36.f * lettersPerLine)) / 2.f, height - (numberOflines * 36.f));
	int letterCount{ 0 };
	int RowCount{ 0 };
	while (letterCount < letters.size()){
		for (int collumnLooper{ 0 }; collumnLooper < lettersPerLine; ++collumnLooper) {
			auto letterObject = std::make_unique<dae::GameObject>();
			letterObject->SetParent(keyboardParentPtr);
			letterObject->AddComponent<dae::TextComponent>(*letterObject.get(), letters[letterCount], font);
			letterObject->SetPosition(collumnLooper * 36.f, RowCount * 36.f);
			if (collumnLooper == 0) {
				keyboardComp->AddKeyLocation({ letterObject->GetWorldTransform().GetPosition().x, letterObject->GetWorldTransform().GetPosition().y, letters[letterCount]});
			}
			else {
				keyboardComp->AddKeyLocation(RowCount, { letterObject->GetWorldTransform().GetPosition().x, letterObject->GetWorldTransform().GetPosition().y, letters[letterCount] });
			}
			scene.Add(std::move(letterObject));
			++letterCount;
			if (letterCount == letters.size()) break;
		}
		++RowCount;
	}
}

dae::GameObject* SetupLevelAndReturnHallwaysObject(dae::Scene& scene, bool wantHallways) {
	int width, height;
	SDL_GetWindowSize(dae::Renderer::GetInstance().GetSDLWindow(), &width, &height);
	int layerthickness = height / 5;
	for (int rowPos{}; rowPos < height; rowPos += 8) {
		for (int colPos{}; colPos < width; colPos += 8) {
			auto GroundObject = std::make_unique<dae::GameObject>();
			GroundObject->AddComponent<dae::RenderComponent>(*GroundObject.get());
			if (rowPos < layerthickness)
				GroundObject->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(73, 14, 8, 8));
			else if (rowPos < layerthickness * 2)
				GroundObject->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(73, 32, 8, 8));
			else if (rowPos < layerthickness * 3)
				GroundObject->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 14, 8, 8));
			else if (rowPos < layerthickness * 4)
				GroundObject->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 23, 8, 8));
			else
				GroundObject->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 32, 8, 8));
			
			GroundObject->AddComponent<dae::RockComponent>(*GroundObject.get());
			GroundObject->SetPosition(static_cast<float>(colPos), static_cast<float>(rowPos));
			scene.Add(std::move(GroundObject));
		}
	}

	if (wantHallways) {
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::HallwaysComponent>(*go.get(), width, height);
		SetupHallwaySources(go.get());
		go->GetComponent<dae::HallwaysComponent>()->ClearSky();
		auto hallWayPtr = go.get();
		scene.Add(std::move(go));
		return hallWayPtr;
	}
	return nullptr;
	
}

void SetupSound() {
	auto& soundsystem = dae::ServiceLocator::get_SoundSystem();
	soundsystem.AddSongs({"DigDug_SoundEffects/All_Clear.mp3", "DigDug_SoundEffects/Mother_Nature.mp3", "DigDug_SoundEffects/Spurt.mp3" });
	soundsystem.AddEffects({ "DigDug_SoundEffects/fireball.wav", "DigDug_SoundEffects/pump.wav" });
}


void load()
{
	int largeFontSize = 36;
	int smallFontSize = 12;
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", static_cast<uint8_t>(largeFontSize));
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", static_cast<uint8_t>(smallFontSize));
	auto& input = dae::InputManager::GetInstance();
	InitializeCommands(input);
	//Controller Setup
	{
		auto keyboard1 = std::make_unique<dae::Gamepad>(0);
		keyboard1->SetUsed(false);
		input.AddGamepad("KeyboardPlayer1", std::move(keyboard1));

		auto gamepad1 = std::make_unique<dae::Gamepad>(0);
		gamepad1->SetUsed(true);
		input.AddGamepad("GamepadPlayer1", std::move(gamepad1));

		auto gamepad2 = std::make_unique<dae::Gamepad>(1);
		gamepad2->SetUsed(true);
		input.AddGamepad("GamepadPlayer2", std::move(gamepad2));
	}
	//Player Setup
	{
		PlayerInfoTemp playerInfo{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},{161,81},
				"SinglePlayer1" };
		ControllerInfo controllerInfo{ {"GamepadPlayer1", "KeyboardPlayer1"},input };
		SetupGeneralPlayer(playerInfo, controllerInfo);

		PlayerInfoTemp playerInfo2{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},{161,81},
				"CoopPlayer1" };
		ControllerInfo controllerInfo2{ {"GamepadPlayer2", "KeyboardPlayer1"},input };
		SetupGeneralPlayer(playerInfo2, controllerInfo2);

		PlayerInfoTemp playerInfo3{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},{161,81},
				"CoopPlayer2" };
		ControllerInfo controllerInfo3{ {"GamepadPlayer1"},input };
		SetupGeneralPlayer(playerInfo3, controllerInfo3);

		FygarPlayerInfoTemp fygarPlayerInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },
			{ 161,81 }, "PvPPlayer2", {SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)} };

		SetupGeneralFygarPlayer(fygarPlayerInfo, controllerInfo3);
	}

	//StartMenu stuff
	auto& menu = sceneManager.CreateScene("MainMenu");
	{
		SetupLevelAndReturnHallwaysObject(menu, false);
		/*auto gamePad = std::make_unique<dae::Gamepad>(0);
		gamePad->SetUsed(true);
		input.AddGamepad("TitleScreen", std::move(gamePad));*/
		int width = 0;
		int height = 0;
		SDL_GetWindowSize(dae::Renderer::GetInstance().GetSDLWindow(), &width, &height);
		std::pair<int, std::pair<int, int>> titleInfo{ 2,{160,48} };
		float xOffset = static_cast<float>((width - titleInfo.first * titleInfo.second.first) / 2);
		float yOffset = static_cast<float>((height - titleInfo.first * titleInfo.second.second) / 2);
		auto mainMenuBackground = std::make_unique<dae::GameObject>();
		mainMenuBackground->SetPosition(xOffset, yOffset/2);
		mainMenuBackground->AddComponent<dae::RenderComponent>(*mainMenuBackground.get());
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png");
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetBox(SDL_Rect{ 1,183,titleInfo.second.first,titleInfo.second.second });
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetDstBox(SDL_Rect{ 1,183,titleInfo.first * titleInfo.second.first,titleInfo.first * titleInfo.second.second });
		mainMenuBackground->AddComponent<dae::FiniteStateMachineComponent>(*mainMenuBackground.get());
		auto fini = mainMenuBackground->GetComponent<dae::FiniteStateMachineComponent>();
		fini->AddState<dae::HandleInput>("HandleInput");// alter
		auto handleInput = static_cast<dae::HandleInput*>(fini->GetState("HandleInput"));
		handleInput->AddCommand(input.GetCommand("SelectModeSingle"));
		handleInput->AddCommand(input.GetCommand("SelectModeCoop"));
		handleInput->AddCommand(input.GetCommand("SelectModePvP"));
		handleInput->AddGamepad(input.GetGamePad("GamepadPlayer1"));
		fini->SetCurrentState(handleInput);
		auto mainMenuPtr = mainMenuBackground.get();
		menu.Add(std::move(mainMenuBackground));

		auto singleModeInfo = std::make_unique<dae::GameObject>();
		singleModeInfo->SetParent(mainMenuPtr);
		singleModeInfo->SetPosition(0.f, static_cast<float>(titleInfo.first * titleInfo.second.second));
		singleModeInfo->AddComponent<dae::TextComponent>(*singleModeInfo.get(),"Press A For Singleplayer", font);
		auto singleModeInfoPtr = singleModeInfo.get();
		menu.Add(std::move(singleModeInfo));
		auto coopModeInfo = std::make_unique<dae::GameObject>();
		coopModeInfo->SetParent(singleModeInfoPtr);
		coopModeInfo->SetPosition(0.f, static_cast<float>(largeFontSize));
		coopModeInfo->AddComponent<dae::TextComponent>(*coopModeInfo.get(), "Press B For Coop", font);
		auto coopModeInfoPtr = coopModeInfo.get();
		menu.Add(std::move(coopModeInfo));
		auto versusModeInfo = std::make_unique<dae::GameObject>();
		versusModeInfo->SetParent(coopModeInfoPtr);
		versusModeInfo->SetPosition(0.f, static_cast<float>(largeFontSize));
		versusModeInfo->AddComponent<dae::TextComponent>(*versusModeInfo.get(), "Press X For PvP", font);
		menu.Add(std::move(versusModeInfo));

		
	}
	//GameStuff
	auto& singleScene = dae::SceneManager::GetInstance().CreateScene("Single");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto scoreBoardParentGoPtr = ScoreBoardParentGo.get();
			singleScene.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			singleScene.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			singleScene.Add(std::move(controllsInfoGo));
			
			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(scoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			singleScene.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "Single2", "End");
		auto gameMasterPtr = gameMaster.get();
		singleScene.Add(std::move(gameMaster));


		dae::StartUpInfo startUpInfo{{{161.f,81.f}},};
		auto functor = std::function([&singleScene](dae::Scene*) {
			auto player = dae::InputManager::GetInstance().GetGameActor("SinglePlayer1");
			player->SetPosition(singleScene.GetStartUpInfo().PlayerPositions[0].first, singleScene.GetStartUpInfo().PlayerPositions[0].second);
			singleScene.Add(player);
			auto hallways = singleScene.findGameObjectsWithComponent<dae::HallwaysComponent>();
			auto gameMaster = singleScene.findGameObjectsWithComponent<dae::GameMasterComponent>();
			auto playerInfo = singleScene.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
			if (hallways[0] && gameMaster[0] && playerInfo[0]) {
				player->SetParent(hallways[0]);
				player->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
				player->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
				gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene);
				player->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
				player->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
			}
			});
		singleScene.SetStartUpFunctor(&functor, startUpInfo);


		auto hallways = SetupLevelAndReturnHallwaysObject(singleScene, true);
		
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 15, 14),{ 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene, hallways };
		
		SetupEnemy(enemyInfo);
		EnemyInfo enemyInfo2{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { 288,160 },
			{ SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene, hallways, 
			{SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)}};
		SetupEnemy(enemyInfo2);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		singleScene.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene);
	}
	auto& singleScene2 = dae::SceneManager::GetInstance().CreateScene("Single2");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto scoreBoardParentGoPtr = ScoreBoardParentGo.get();
			singleScene2.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			singleScene2.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			singleScene2.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(scoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			singleScene2.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "Single3", "End");
		auto gameMasterPtr = gameMaster.get();
		singleScene2.Add(std::move(gameMaster));


		dae::StartUpInfo startUpInfo{ {{81.f,81.f}}, };
		auto functor = std::function([&singleScene2](dae::Scene* scene) {
			auto players = scene->findActiveGameObjectsWithComponent<dae::PlayerComponent>();
			singleScene2.Add(players[0]);
			auto hallways = singleScene2.findGameObjectsWithComponent<dae::HallwaysComponent>();
			auto gameMaster = singleScene2.findGameObjectsWithComponent<dae::GameMasterComponent>();
			auto playerInfo = singleScene2.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
			if (hallways[0] && gameMaster[0] && playerInfo[0]) {
				players[0]->SetParent(hallways[0]);
				players[0]->SetPosition(singleScene2.GetStartUpInfo().PlayerPositions[0].first, singleScene2.GetStartUpInfo().PlayerPositions[0].second);
				players[0]->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
				players[0]->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
				gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene2);
				players[0]->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
				players[0]->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
			}
			});
		singleScene2.SetStartUpFunctor(&functor, startUpInfo);


		auto hallways = SetupLevelAndReturnHallwaysObject(singleScene2, true);

		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 15, 14),{ 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene2, hallways };

		SetupEnemy(enemyInfo);
		EnemyInfo enemyInfo2{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { 288,160 },
			{ SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene2, hallways,
			{SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)} };
		SetupEnemy(enemyInfo2);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		singleScene2.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene2);
	}
	auto& singleScene3 = dae::SceneManager::GetInstance().CreateScene("Single3");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto scoreBoardParentGoPtr = ScoreBoardParentGo.get();
			singleScene3.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			singleScene3.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(scoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			singleScene3.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(scoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			singleScene3.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "End");
		auto gameMasterPtr = gameMaster.get();
		singleScene3.Add(std::move(gameMaster));


		dae::StartUpInfo startUpInfo{ {{81.f,81.f}}, };
		auto functor = std::function([&singleScene3](dae::Scene* scene) {
			auto players = scene->findActiveGameObjectsWithComponent<dae::PlayerComponent>();
			singleScene3.Add(players[0]);
			auto hallways = singleScene3.findGameObjectsWithComponent<dae::HallwaysComponent>();
			auto gameMaster = singleScene3.findGameObjectsWithComponent<dae::GameMasterComponent>();
			auto playerInfo = singleScene3.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
			if (hallways[0] && gameMaster[0] && playerInfo[0]) {
				players[0]->SetParent(hallways[0]);
				players[0]->SetPosition(singleScene3.GetStartUpInfo().PlayerPositions[0].first, singleScene3.GetStartUpInfo().PlayerPositions[0].second);
				players[0]->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
				players[0]->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
				gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene3);
				players[0]->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
				players[0]->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
			}
			});
		singleScene3.SetStartUpFunctor(&functor, startUpInfo);


		auto hallways = SetupLevelAndReturnHallwaysObject(singleScene3, true);

		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 15, 14),{ 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene3, hallways };

		SetupEnemy(enemyInfo);
		EnemyInfo enemyInfo2{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { 288,160 },
			{ SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene3, hallways,
			{SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)} };
		SetupEnemy(enemyInfo2);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		singleScene3.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene3);
	}
	auto& coopScene = dae::SceneManager::GetInstance().CreateScene("Coop");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();	
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto ScoreBoardParentGoPtr = ScoreBoardParentGo.get();
			coopScene.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			coopScene.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			coopScene.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			coopScene.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "Coop2", "End");
		auto gameMasterPtr = gameMaster.get();
		coopScene.Add(std::move(gameMaster));
		auto hallways = SetupLevelAndReturnHallwaysObject(coopScene, true);

		dae::StartUpInfo startUpInfo{{{161.f,81.f}, {81.f, 81.f}}};
		auto functor = std::function<void(dae::Scene*)>([&coopScene](dae::Scene*) {
			auto lambda = [&](std::vector<dae::GameObject*> players, const std::vector<std::pair<float, float>> positions) {
				for (int looper{ 0 }; looper < players.size(); ++looper) {
					players[looper]->SetPosition(coopScene.GetStartUpInfo().PlayerPositions[looper].first, 
						coopScene.GetStartUpInfo().PlayerPositions[looper].second);
					coopScene.Add(players[looper]);
					auto hallways = coopScene.findGameObjectsWithComponent<dae::HallwaysComponent>();
					auto gameMaster = coopScene.findGameObjectsWithComponent<dae::GameMasterComponent>();
					auto playerInfo = coopScene.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
					if (hallways[0] && gameMaster[0] && playerInfo[0]) {
						players[looper]->SetParent(hallways[0]);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
						gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
					}
				}
				};
			std::vector<dae::GameObject*> players = {
					dae::InputManager::GetInstance().GetGameActor("CoopPlayer1") ,
				dae::InputManager::GetInstance().GetGameActor("CoopPlayer2") };
			lambda(players, coopScene.GetStartUpInfo().PlayerPositions);
			 });
		coopScene.SetStartUpFunctor(&functor, startUpInfo);
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), { 160,160 }, 
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, coopScene, hallways };
		SetupEnemy(enemyInfo);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		coopScene.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene);
		
	}
	auto& coopScene2 = dae::SceneManager::GetInstance().CreateScene("Coop2");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto ScoreBoardParentGoPtr = ScoreBoardParentGo.get();
			coopScene2.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			coopScene2.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			coopScene2.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			coopScene2.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "Coop3", "End");
		auto gameMasterPtr = gameMaster.get();
		coopScene2.Add(std::move(gameMaster));
		auto hallways = SetupLevelAndReturnHallwaysObject(coopScene2, true);

		dae::StartUpInfo startUpInfo{ {{241.f,81.f}, {81.f, 81.f}} };
		auto functor = std::function<void(dae::Scene*)>([&coopScene2](dae::Scene* scene) {
			auto lambda = [&](std::vector<dae::GameObject*> players, const std::vector<std::pair<float, float>> positions) {
				for (int looper{ 0 }; looper < players.size() && looper < positions.size(); ++looper) {
					players[looper]->SetPosition(coopScene2.GetStartUpInfo().PlayerPositions[looper].first,
						coopScene2.GetStartUpInfo().PlayerPositions[looper].second);
					coopScene2.Add(players[looper]);
					auto hallways = coopScene2.findGameObjectsWithComponent<dae::HallwaysComponent>();
					auto gameMaster = coopScene2.findGameObjectsWithComponent<dae::GameMasterComponent>();
					auto playerInfo = coopScene2.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
					if (hallways[0] && gameMaster[0] && playerInfo[0]) {
						players[looper]->SetParent(hallways[0]);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
						gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene2);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
					}
				}
				};

			std::vector<dae::GameObject*> players = scene->findActiveGameObjectsWithComponent<dae::PlayerComponent>();
			lambda(players, coopScene2.GetStartUpInfo().PlayerPositions);
			});
		coopScene2.SetStartUpFunctor(&functor, startUpInfo);
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), { 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, coopScene2, hallways };
		SetupEnemy(enemyInfo);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		coopScene2.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene2);

	}
	auto& coopScene3 = dae::SceneManager::GetInstance().CreateScene("Coop3");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto ScoreBoardParentGoPtr = ScoreBoardParentGo.get();
			coopScene3.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			coopScene3.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			coopScene3.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			coopScene3.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "End");
		auto gameMasterPtr = gameMaster.get();
		coopScene3.Add(std::move(gameMaster));
		auto hallways = SetupLevelAndReturnHallwaysObject(coopScene3, true);

		dae::StartUpInfo startUpInfo{ {{241.f,81.f}, {81.f, 81.f}} };
		auto functor = std::function<void(dae::Scene*)>([&coopScene3](dae::Scene* scene) {
			auto lambda = [&](std::vector<dae::GameObject*> players, const std::vector<std::pair<float, float>> positions) {
				for (int looper{ 0 }; looper < players.size() && looper < positions.size(); ++looper) {
					players[looper]->SetPosition(coopScene3.GetStartUpInfo().PlayerPositions[looper].first,
						coopScene3.GetStartUpInfo().PlayerPositions[looper].second);
					coopScene3.Add(players[looper]);
					auto hallways = coopScene3.findGameObjectsWithComponent<dae::HallwaysComponent>();
					auto gameMaster = coopScene3.findGameObjectsWithComponent<dae::GameMasterComponent>();
					auto playerInfo = coopScene3.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
					if (hallways[0] && gameMaster[0] && playerInfo[0]) {
						players[looper]->SetParent(hallways[0]);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
						gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene3);
						players[looper]->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
						players[looper]->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
					}
				}
				};

			std::vector<dae::GameObject*> players = scene->findActiveGameObjectsWithComponent<dae::PlayerComponent>();
			lambda(players, coopScene3.GetStartUpInfo().PlayerPositions);
			});
		coopScene3.SetStartUpFunctor(&functor, startUpInfo);
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), { 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, coopScene3, hallways };
		SetupEnemy(enemyInfo);
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		coopScene3.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene3);

	}
	auto& pvpScene = dae::SceneManager::GetInstance().CreateScene("PvP");
	{
		//Parent of player info
		std::unique_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_unique<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			auto ScoreBoardParentGoPtr = ScoreBoardParentGo.get();
			pvpScene.Add(std::move(ScoreBoardParentGo));
			auto controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			pvpScene.Add(std::move(controllsInfoGo));
			controllsInfoGo = std::make_unique<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGoPtr);
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			pvpScene.Add(std::move(controllsInfoGo));

			playerStatsDisplayGo = std::make_unique<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGoPtr);
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			pvpScene.Add(std::move(playerStatsDisplayGo));
		}
		auto gameMaster = std::make_unique<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "End");
		auto gameMasterPtr = gameMaster.get();
		pvpScene.Add(std::move(gameMaster));
		auto hallways = SetupLevelAndReturnHallwaysObject(pvpScene, true);

		dae::StartUpInfo startUpInfo{ {{81.f,81.f}, { 161.f,81.f },} };
		auto functor = std::function([&pvpScene](dae::Scene*) {
			auto player = dae::InputManager::GetInstance().GetGameActor("CoopPlayer1");
			auto enemyPlayer = dae::InputManager::GetInstance().GetGameActor("PvPPlayer2");
			player->SetPosition(pvpScene.GetStartUpInfo().PlayerPositions[0].first, pvpScene.GetStartUpInfo().PlayerPositions[0].second);
			enemyPlayer->SetPosition(pvpScene.GetStartUpInfo().PlayerPositions[1].first, pvpScene.GetStartUpInfo().PlayerPositions[1].second);
			pvpScene.Add(player);
			pvpScene.Add(enemyPlayer);
			auto hallways = pvpScene.findGameObjectsWithComponent<dae::HallwaysComponent>();
			auto gameMaster = pvpScene.findGameObjectsWithComponent<dae::GameMasterComponent>();
			auto playerInfo = pvpScene.findGameObjectsWithComponent<dae::PlayerInfoComponent>();
			if (hallways[0] && gameMaster[0] && playerInfo[0]) {
				player->SetParent(hallways[0]);
				enemyPlayer->SetParent(hallways[0]);
				player->GetComponent<dae::PlayerComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
				player->GetComponent<dae::HealthComponent>()->AddObserver(gameMaster[0]->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
				gameMaster[0]->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(pvpScene);
				player->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
				player->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo[0]->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
			}
			});
		pvpScene.SetStartUpFunctor(&functor, startUpInfo);

		//Player setup
		/*PlayerInfo playerInfo1{"DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},
			{81,81}, "PvP Player 1", pvpScene, hallways, playerStatsDisplayGo.get(), gameMaster.get()};
		ControllerInfo controllerInfo1{ "KeyboardPlayer1",input};
		ScoreInfo scoreInfo1{ smallFont, ScoreBoardParentGo };
		SetupPlayer(playerInfo1, controllerInfo1, scoreInfo1);*/
		//int pos[2]{ 81,81 };
		//SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(1, 0, 14, 15), pos, "Player 1", true, input, scene, smallFont, ScoreBoardParentGo, hallways);
		//int pos2[2]{ 161,81 };
		/*FygarPlayerInfo fygarPlayerInfo{"DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), {SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15)},
			{ 161,81 }, "PvP Player 2", pvpScene, hallways, playerStatsDisplayGo.get(), {SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)} };

		ControllerInfo controllerInfo2{ "GamepadPlayer1",input};
		ScoreInfo scoreInfo2{ smallFont, ScoreBoardParentGo };
		SetupFygarPlayer(fygarPlayerInfo, controllerInfo2, scoreInfo2);*/
		SetupTunnels(hallways);
		/*EnemyInfo enemyInfo{"DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), {160,160},
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, pvpScene, hallways };
		SetupEnemy(enemyInfo);*/
		//Gameobject for the fps
		auto go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		pvpScene.Add(std::move(go));
		gameMasterPtr->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(pvpScene);
	}
	auto& endScene = dae::SceneManager::GetInstance().CreateScene("End");
	{
		dae::StartUpInfo startUpInfo{ {{161.f,81.f}}, };
		auto functor = std::function([&endScene](dae::Scene* previousScene) {
			auto players = previousScene->findGameObjectsWithComponent<dae::PlayerComponent>();
			if (!players[0]) return;
			//players[0]->SetPosition(endScene.GetStartUpInfo().PlayerPositions[0].first, endScene.GetStartUpInfo().PlayerPositions[0].second);
			for (auto gameObject : players) {
				gameObject->m_Active = false;
				endScene.Add(gameObject);
			}
			});
		endScene.SetStartUpFunctor(&functor, startUpInfo);
		SetupLevelAndReturnHallwaysObject(endScene, false);
		SetupKeyboard(endScene, font,9, input);
	}
	SetupSound();
	dae::ServiceLocator::get_SoundSystem().PlayMusic(0, 2.f);
	dae::SceneManager::GetInstance().SetActiveScene(*sceneManager.GetScene("MainMenu"));
}

int main(int, char* []) {
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}