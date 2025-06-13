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
	input.AddCommand<dae::Move>("GamepadUp", 1, true, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>("GamepadDown", 2, true, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>("GamepadLeft", 4, true, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>("GamepadRight", 8, true, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Move>("KeyboardUp", SDL_SCANCODE_W, false, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>("keyboardDown", SDL_SCANCODE_S, false, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>("KeyboardLeft", SDL_SCANCODE_A, false, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>("KeyboardRight", SDL_SCANCODE_D, false, dae::RenderComponent::RIGHT);
	
	input.AddCommand<dae::Pump>("KeyboardPump", SDL_SCANCODE_E, false);
	input.AddCommand<dae::Pump>("GamepadPump", 4096, true);
	input.AddCommand<dae::DigDugAttackCommand>("KeyboardShootTether",SDL_SCANCODE_E, false);
	input.AddCommand<dae::DigDugAttackCommand>("GamepadShootTether", 4096, true);
	input.AddCommand<dae::FygarAttackCommand>("GamepadFygarAttack", 4096, true);
	input.AddCommand<dae::FygarAttackCommand>("KeyboardFygarAttack", SDL_SCANCODE_E, false);

	input.AddCommand<dae::Suicide>("GamepadSuicide", 32768, true);
	input.AddCommand<dae::Suicide>("KeyboardSuicide", SDL_SCANCODE_C, false);
	input.AddCommand<dae::Pickup>("GamepadPickupSmall", 16384, true, 10);
	input.AddCommand<dae::Pickup>("KeyboardPickupSmall", SDL_SCANCODE_Z, false, 10);
	input.AddCommand<dae::Pickup>("GamepadPickupLarge", 8192, true, 100);
	input.AddCommand<dae::Pickup>("KeyboardPickupLarge", SDL_SCANCODE_X, false, 100);
	input.AddCommand<dae::PlaySound>("KeyboardPlaySound", SDL_SCANCODE_F, false, 0);
	input.AddCommand<dae::PlaySound>("GamepadPlaySound", 4096, true, 0);

	input.AddCommand<dae::SelectModeCommand>("SelectModeSingle", 4096, true, "Single", 1);
	input.AddCommand<dae::SelectModeCommand>("SelectModeCoop", 8192, true, "Coop", 2);
	input.AddCommand<dae::SelectModeCommand>("SelectModePvP", 16384, true, "PvP", 1);

	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardUp", 1, true, std::pair{ 0.f,-1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardDown", 2, true, std::pair{ 0.f,1.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardLeft", 4, true, std::pair{ -1.f,0.f });
	input.AddCommand<dae::KeyboardMoveCommand>("GamepadKeyboardRight", 8, true, std::pair{ 1.f,0.f });
	input.AddCommand<dae::KeyboardConfirmCommand>("GamepadKeyboardConfirm", 4096, true);

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
struct PlayerInfoTemp {
	std::string texturePath;
	SDL_Rect textureSrcRect;
	std::vector<SDL_Rect> runningAnimLocations;
	int startPos[2];
	std::string name;
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
struct ControllerInfo {
	std::string gamepadName;
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


void SetupPlayer(const PlayerInfo& playerInfo, const ControllerInfo& controllerInfo, const ScoreInfo&) {

	/*auto PlayerLivesGo = std::make_shared<dae::GameObject>();
	PlayerLivesGo->SetParent(scoreInfo.scoreBoardGo.get());
	PlayerLivesGo->AddComponent<dae::PlayerInfoComponent>(*PlayerLivesGo.get());
	PlayerLivesGo->SetPosition(0.f, 30.f);
	auto PlayerScoreGo = std::make_shared<dae::GameObject>();
	PlayerScoreGo->SetParent(scoreInfo.scoreBoardGo.get());
	PlayerScoreGo->AddComponent<dae::PlayerInfoComponent>(*PlayerScoreGo.get());
	PlayerScoreGo->SetPosition(0.f, 40.f);*/

	auto go = std::make_shared<dae::GameObject>();
	go->SetParent(playerInfo.hallways);
	go->AddComponent<dae::RenderComponent>(*go.get());
	/*go->AddComponent<dae::StateHandlerComponent>(*go.get());
	{
		go->GetComponent<dae::StateHandlerComponent>()->AddPlayerState<dae::PlayerIdleState>("idle");
		go->GetComponent<dae::StateHandlerComponent>()->GetPlayerState<dae::PlayerIdleState>()->AddCommand<dae::Move>(static_cast<dae::Move*>(controllerInfo.input.GetCommand("KeyboardLeft")));
		go->GetComponent<dae::StateHandlerComponent>()->GetPlayerState<dae::PlayerIdleState>()->AddCommand<dae::Move>(static_cast<dae::Move*>(controllerInfo.input.GetCommand("KeyboardRight")));
		go->GetComponent<dae::StateHandlerComponent>()->GetPlayerState<dae::PlayerIdleState>()->AddCommand<dae::Move>(static_cast<dae::Move*>(controllerInfo.input.GetCommand("KeyboardUp")));
		go->GetComponent<dae::StateHandlerComponent>()->GetPlayerState<dae::PlayerIdleState>()->AddCommand<dae::Move>(static_cast<dae::Move*>(controllerInfo.input.GetCommand("KeyboardDown")));

	}*/
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
	/*std::unique_ptr gamepad = std::make_unique<dae::Gamepad>(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(std::move(gamepad));*/


	go->AddComponent<dae::PlayerComponent>(*go.get(), playerInfo.name);
	go->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetScoreContainer());
	go->GetComponent<dae::PlayerComponent>()->AddObserver(playerInfo.gameMaster->GetComponent<dae::GameMasterComponent>()->GetEnemyPresenceObserver());
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8,1.f);
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.gameMaster->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
	/*auto playerComp = go->GetComponent<dae::PlayerComponent>();
	auto healthComp = go->GetComponent<dae::HealthComponent>();
	auto livesInfoComp = PlayerLivesGo->GetComponent<dae::PlayerInfoComponent>();
	auto scoreInfoComp = PlayerScoreGo->GetComponent<dae::PlayerInfoComponent>();*/
	/*if (playerComp && healthComp && livesInfoComp && scoreInfoComp) {
		//PlayerLivesGo->AddComponent<dae::TextComponent>(*PlayerLivesGo, "# lives: " + std::to_string(healthComp->GetLives()), scoreInfo.font);
		//PlayerScoreGo->AddComponent<dae::TextComponent>(*PlayerScoreGo, "Score: " + std::to_string(playerComp->GetScore()), scoreInfo.font);
		playerComp->AddObserver(scoreInfoComp);
		healthComp->AddObserver(livesInfoComp);
	}*/
	//go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	playerInfo.scene.Add(go);
	controllerInfo.input.AddGameActor(go.get());
	//playerInfo.scene.Add(PlayerLivesGo);
	//playerInfo.scene.Add(PlayerScoreGo);

}

void SetupFygarPlayer(const FygarPlayerInfo& playerInfo, const ControllerInfo& controllerInfo, const ScoreInfo&) {
	auto go = std::make_shared<dae::GameObject>();
	go->SetParent(playerInfo.hallways);
	go->AddComponent<dae::RenderComponent>(*go.get());

	{

		go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
		auto FiniteComp = go->GetComponent<dae::FiniteStateMachineComponent>();
		{
			FiniteComp->AddState<dae::HandleInput>("HandleInput", controllerInfo.input.GetGamePad(controllerInfo.gamepadName));

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
				handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadFygarAttack"));
				handleInputState->AddCommand(controllerInfo.input.GetCommand("KeyboardFygarAttack"));
				//handleInputState->AddCommand(controllerInfo.input.GetCommand("GamepadShootTether"));
			}
			FiniteComp->AddState<dae::Running>("Running", playerInfo.runningAnimLocations, 0.3f);
			FiniteComp->AddState<dae::FygarAttack>("FygarAttacking");
			//FiniteComp->AddState<dae::DigDugAttack>("DigDugAttacking", controllerInfo.input.GetGamePad(playerInfo.name));

			//static_cast<dae::DigDugAttack*>(FiniteComp->GetState("DigDugAttacking"))->AddCommand(controllerInfo.input.GetCommand("KeyboardPump"));
			//static_cast<dae::DigDugAttack*>(FiniteComp->GetState("DigDugAttacking"))->AddCommand(controllerInfo.input.GetCommand("GamepadPump"));
			FiniteComp->AddCondition<dae::HasMovementInput>("HasMovementInput");
			FiniteComp->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
			/*FiniteComp->AddCondition < dae::WantsToAttack>("WantsToAttack");
			FiniteComp->AddCondition<dae::FinishedAttack>("FinishedAttack*/
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

	/*std::unique_ptr gamepad = std::make_unique<dae::Gamepad>(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(std::move(gamepad));*/


	go->AddComponent<dae::EnemyComponent>(*go.get(), 4, 100,3.f);
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8, 1.f);
	go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
	/*auto playerComp = go->GetComponent<dae::PlayerComponent>();
	auto healthComp = go->GetComponent<dae::HealthComponent>();
	auto livesInfoComp = PlayerLivesGo->GetComponent<dae::PlayerInfoComponent>();
	auto scoreInfoComp = PlayerScoreGo->GetComponent<dae::PlayerInfoComponent>();*/
	/*if (playerComp && healthComp && livesInfoComp && scoreInfoComp) {
		//PlayerLivesGo->AddComponent<dae::TextComponent>(*PlayerLivesGo, "# lives: " + std::to_string(healthComp->GetLives()), scoreInfo.font);
		//PlayerScoreGo->AddComponent<dae::TextComponent>(*PlayerScoreGo, "Score: " + std::to_string(playerComp->GetScore()), scoreInfo.font);
		playerComp->AddObserver(scoreInfoComp);
		healthComp->AddObserver(livesInfoComp);
	}*/
	//go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	playerInfo.scene.Add(go);
	controllerInfo.input.AddGameActor(go.get());
	//playerInfo.scene.Add(PlayerLivesGo);
	//playerInfo.scene.Add(PlayerScoreGo);
}

void SetupEnemy(EnemyInfo enemyInfo) {
	auto go = std::make_shared<dae::GameObject>();

	go = std::make_shared<dae::GameObject>();
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
	enemyInfo.scene.Add(go);
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
	auto keyboardParent = std::make_shared<dae::GameObject>();
	keyboardParent->AddComponent<dae::KeyboardComponent>(*keyboardParent.get(), 0.5f);
	auto keyboardComp = keyboardParent->GetComponent<dae::KeyboardComponent>();
	keyboardComp->SetHighLightSize({ 36,36 });
	keyboardParent->AddComponent<dae::TextComponent>(*keyboardParent.get(), "", font);
	auto offset = std::pair<float, float>(20.f, -30.f);
	keyboardParent->GetComponent<dae::TextComponent>()->SetOffset(offset);
	keyboardParent->AddComponent<dae::FiniteStateMachineComponent>(*keyboardParent.get());
	auto finiteComp = keyboardParent->GetComponent<dae::FiniteStateMachineComponent>();
	finiteComp->AddState<dae::HandleInput>("HandleInput", input.GetGamePad("GamepadPlayer1"));
	auto handleInput = static_cast<dae::HandleInput*>(finiteComp->GetState("HandleInput"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardUp"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardDown"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardLeft"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardRight"));
	handleInput->AddCommand(input.GetCommand("GamepadKeyboardConfirm"));
	finiteComp->SetCurrentState(handleInput);

	scene.Add(keyboardParent);
	std::vector<std::string> letters = { "A", "B","C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
	"O", "P","Q", "R", "S", "V", "U", "W", "T", "X", "Y", "Z" };
	auto numberOflines = ((letters.size() - std::fmodf(static_cast<float>(letters.size()), static_cast<float>(lettersPerLine))) / lettersPerLine) + 1;
	keyboardParent->SetPosition((width - (36.f * lettersPerLine)) / 2.f, height - (numberOflines * 36.f));
	int letterCount{ 0 };
	int RowCount{ 0 };
	while (letterCount < letters.size()){
		for (int collumnLooper{ 0 }; collumnLooper < lettersPerLine; ++collumnLooper) {
			auto letterObject = std::make_shared<dae::GameObject>();
			letterObject->SetParent(keyboardParent.get());
			letterObject->AddComponent<dae::TextComponent>(*letterObject.get(), letters[letterCount], font);
			letterObject->SetPosition(collumnLooper * 36.f, RowCount * 36.f);
			if (collumnLooper == 0) {
				keyboardComp->AddKeyLocation({ letterObject->GetWorldTransform().GetPosition().x, letterObject->GetWorldTransform().GetPosition().y, letters[letterCount]});
			}
			else {
				keyboardComp->AddKeyLocation(RowCount, { letterObject->GetWorldTransform().GetPosition().x, letterObject->GetWorldTransform().GetPosition().y, letters[letterCount] });
			}
			scene.Add(letterObject);
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
			std::shared_ptr<dae::GameObject> GroundObject = std::make_shared<dae::GameObject>();
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
			scene.Add(GroundObject);
		}
	}

	if (wantHallways) {
		auto go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::HallwaysComponent>(*go.get(), width, height);
		SetupHallwaySources(go.get());
		go->GetComponent<dae::HallwaysComponent>()->ClearSky();
		scene.Add(go);
		return go.get();
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
	/* {
		PlayerInfoTemp playerInfo{"DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},{161,81},
			"Single Player 1" };
		ControllerInfo controllerInfo{ "GamepadPlayer1",input };
		//ScoreInfo scoreInfo{ smallFont, ScoreBoardParentGo };

		auto go = std::make_shared<dae::GameObject>();
		//go->SetParent(playerInfo.hallways); //on scene startup
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
		go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8, 1.f);
		go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.playerStatDisplay->GetComponent<dae::PlayerInfoComponent>()->GetHealthContainer());
		go->GetComponent<dae::HealthComponent>()->AddObserver(playerInfo.gameMaster->GetComponent<dae::GameMasterComponent>()->GetPlayerPresenceObserver());
		
		playerInfo.scene.Add(go);
		controllerInfo.input.AddGameActor(go.get());

	}*/
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
		auto mainMenuBackground = std::make_shared<dae::GameObject>();
		mainMenuBackground->SetPosition(xOffset, yOffset/2);
		mainMenuBackground->AddComponent<dae::RenderComponent>(*mainMenuBackground.get());
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png");
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetBox(SDL_Rect{ 1,183,titleInfo.second.first,titleInfo.second.second });
		mainMenuBackground->GetComponent<dae::RenderComponent>()->SetDstBox(SDL_Rect{ 1,183,titleInfo.first * titleInfo.second.first,titleInfo.first * titleInfo.second.second });
		mainMenuBackground->AddComponent<dae::FiniteStateMachineComponent>(*mainMenuBackground.get());
		auto fini = mainMenuBackground->GetComponent<dae::FiniteStateMachineComponent>();
		fini->AddState<dae::HandleInput>("HandleInput", input.GetGamePad("GamepadPlayer1"));// alter
		auto handleInput = static_cast<dae::HandleInput*>(fini->GetState("HandleInput"));
		handleInput->AddCommand(input.GetCommand("SelectModeSingle"));
		handleInput->AddCommand(input.GetCommand("SelectModeCoop"));
		handleInput->AddCommand(input.GetCommand("SelectModePvP"));

		fini->SetCurrentState(handleInput);
		menu.Add(mainMenuBackground);

		auto singleModeInfo = std::make_shared<dae::GameObject>();
		singleModeInfo->SetParent(mainMenuBackground.get());
		singleModeInfo->SetPosition(0.f, static_cast<float>(titleInfo.first * titleInfo.second.second));
		singleModeInfo->AddComponent<dae::TextComponent>(*singleModeInfo.get(),"Press A For Singleplayer", font);
		menu.Add(singleModeInfo);
		auto coopModeInfo = std::make_shared<dae::GameObject>();
		coopModeInfo->SetParent(singleModeInfo.get());
		coopModeInfo->SetPosition(0.f, static_cast<float>(largeFontSize));
		coopModeInfo->AddComponent<dae::TextComponent>(*coopModeInfo.get(), "Press B For Coop", font);
		menu.Add(coopModeInfo);
		auto versusModeInfo = std::make_shared<dae::GameObject>();
		versusModeInfo->SetParent(coopModeInfo.get());
		versusModeInfo->SetPosition(0.f, static_cast<float>(largeFontSize));
		versusModeInfo->AddComponent<dae::TextComponent>(*versusModeInfo.get(), "Press X For PvP", font);
		menu.Add(versusModeInfo);

		
	}
	//GameStuff
	auto& singleScene = dae::SceneManager::GetInstance().CreateScene("Single");
	{
		//Parent of player info
		std::shared_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_shared<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			singleScene.Add(ScoreBoardParentGo);
			auto controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			singleScene.Add(controllsInfoGo);
			controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			singleScene.Add(controllsInfoGo);
			
			playerStatsDisplayGo = std::make_shared<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGo.get());
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			singleScene.Add(playerStatsDisplayGo);
		}
		auto gameMaster = std::make_shared<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "MainMenu");
		singleScene.Add(gameMaster);
		auto hallways = SetupLevelAndReturnHallwaysObject(singleScene, true);
		PlayerInfo playerInfo{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},{161,81}, 
			"Single Player 1", singleScene, hallways, playerStatsDisplayGo.get(), gameMaster.get()};
		ControllerInfo controllerInfo{"GamepadPlayer1",input};
		ScoreInfo scoreInfo{ smallFont, ScoreBoardParentGo };
		SetupPlayer(playerInfo, controllerInfo, scoreInfo);
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 15, 14),{ 160,160 },
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene, hallways };
		
		SetupEnemy(enemyInfo);
		EnemyInfo enemyInfo2{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { 288,160 },
			{ SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, singleScene, hallways, 
			{SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)}};
		SetupEnemy(enemyInfo2);
		//Gameobject for the fps
		auto go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		singleScene.Add(go);
		gameMaster->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(singleScene);
	}
	auto& coopScene = dae::SceneManager::GetInstance().CreateScene("Coop");
	{
		//Parent of player info
		std::shared_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_shared<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			coopScene.Add(ScoreBoardParentGo);
			auto controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			coopScene.Add(controllsInfoGo);
			controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			coopScene.Add(controllsInfoGo);

			playerStatsDisplayGo = std::make_shared<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGo.get());
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			coopScene.Add(playerStatsDisplayGo);
		}
		auto gameMaster = std::make_shared<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "MainMenu");
		coopScene.Add(gameMaster);
		auto hallways = SetupLevelAndReturnHallwaysObject(coopScene, true);
		//Player setup
		PlayerInfo playerInfo1{"DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},
			{81,81}, "Coop Player 1", coopScene, hallways, playerStatsDisplayGo.get(), gameMaster.get()};
		ControllerInfo controllerInfo1{ "KeyboardPlayer1",input};
		ScoreInfo scoreInfo1{ smallFont, ScoreBoardParentGo };
		SetupPlayer(playerInfo1, controllerInfo1, scoreInfo1);
		//int pos[2]{ 81,81 };
		//SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(1, 0, 14, 15), pos, "Player 1", true, input, scene, smallFont, ScoreBoardParentGo, hallways);
		//int pos2[2]{ 161,81 };
		PlayerInfo playerInfo2{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},
			{ 161,81 }, "Coop Player 2", coopScene, hallways, playerStatsDisplayGo.get(), gameMaster.get()};
		ControllerInfo controllerInfo2{ "GamepadPlayer1",input};
		ScoreInfo scoreInfo2{ smallFont, ScoreBoardParentGo };
		SetupPlayer(playerInfo2, controllerInfo2, scoreInfo2);
		SetupTunnels(hallways);
		EnemyInfo enemyInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), { 160,160 }, 
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, coopScene, hallways };
		SetupEnemy(enemyInfo);
		//Gameobject for the fps
		auto go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		coopScene.Add(go);
		gameMaster->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(coopScene);
		
	}
	auto& pvpScene = dae::SceneManager::GetInstance().CreateScene("PvP");
	{
		//Parent of player info
		std::shared_ptr<dae::GameObject> playerStatsDisplayGo;
		auto ScoreBoardParentGo = std::make_shared<dae::GameObject>();
		{
			ScoreBoardParentGo->SetPosition(0.f, 10.f);
			pvpScene.Add(ScoreBoardParentGo);
			auto controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
			pvpScene.Add(controllsInfoGo);
			controllsInfoGo = std::make_shared<dae::GameObject>();
			controllsInfoGo->SetParent(ScoreBoardParentGo.get());
			controllsInfoGo->AddComponent<dae::TextComponent>(*controllsInfoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
			controllsInfoGo->SetPosition(0.f, 0.f);
			pvpScene.Add(controllsInfoGo);

			playerStatsDisplayGo = std::make_shared<dae::GameObject>();
			playerStatsDisplayGo->SetParent(ScoreBoardParentGo.get());
			playerStatsDisplayGo->SetPosition(0.f, 20.f);
			playerStatsDisplayGo->AddComponent<dae::PlayerInfoComponent>(*playerStatsDisplayGo.get());
			playerStatsDisplayGo->AddComponent<dae::TextComponent>(*playerStatsDisplayGo.get(), "", smallFont);
			pvpScene.Add(playerStatsDisplayGo);
		}
		auto gameMaster = std::make_shared<dae::GameObject>();
		gameMaster->AddComponent<dae::GameMasterComponent>(*gameMaster.get(), "End", "MainMenu");
		pvpScene.Add(gameMaster);
		auto hallways = SetupLevelAndReturnHallwaysObject(pvpScene, true);
		//Player setup
		PlayerInfo playerInfo1{ "DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), {SDL_Rect(0, 15, 14, 15), SDL_Rect(16, 15, 14, 15)},
			{81,81}, "PvP Player 1", pvpScene, hallways, playerStatsDisplayGo.get(), gameMaster.get()};
		ControllerInfo controllerInfo1{ "KeyboardPlayer1",input};
		ScoreInfo scoreInfo1{ smallFont, ScoreBoardParentGo };
		SetupPlayer(playerInfo1, controllerInfo1, scoreInfo1);
		//int pos[2]{ 81,81 };
		//SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(1, 0, 14, 15), pos, "Player 1", true, input, scene, smallFont, ScoreBoardParentGo, hallways);
		//int pos2[2]{ 161,81 };
		FygarPlayerInfo fygarPlayerInfo{ "DigDug_General_Sprites.png", SDL_Rect(0, 241, 14, 15), { SDL_Rect(0, 241, 14, 15), SDL_Rect(16, 241, 14, 15) },
			{ 161,81 }, "PvP Player 2", pvpScene, hallways, playerStatsDisplayGo.get(), {SDL_Rect(0,272,16,17), SDL_Rect(16,272,32,17), SDL_Rect(48,272,48,17)} };

		ControllerInfo controllerInfo2{ "GamepadPlayer1",input};
		ScoreInfo scoreInfo2{ smallFont, ScoreBoardParentGo };
		SetupFygarPlayer(fygarPlayerInfo, controllerInfo2, scoreInfo2);
		SetupTunnels(hallways);
		/*EnemyInfo enemyInfo{"DigDug_General_Sprites.png", SDL_Rect(0, 145, 14, 15), {160,160},
			{ SDL_Rect(0, 145, 15, 14), SDL_Rect(16, 145, 15, 14) },{SDL_Rect(98, 243, 13, 10) , SDL_Rect(114, 243, 13, 11)}, pvpScene, hallways };
		SetupEnemy(enemyInfo);*/
		//Gameobject for the fps
		auto go = std::make_shared<dae::GameObject>();
		go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
		go->AddComponent<dae::FPSComponent>(*go.get());
		go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
		pvpScene.Add(go);
		gameMaster->GetComponent<dae::GameMasterComponent>()->PrepGameMaster(pvpScene);
	}
	auto& endScene = dae::SceneManager::GetInstance().CreateScene("End");
	{
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