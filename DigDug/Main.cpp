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
#include "HasMovementInput.h"
#include "HandleInput.h"
#include "Running.h"
#include "IsDoneRunning.h"
#include "Floating.h"
#include "WantsToFloat.h"
namespace fs = std::filesystem;

void InitializeCommands(dae::InputManager& input)
{
	input.AddCommand<dae::Move>(1, true, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>(2, true, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>(4, true, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>(8, true, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Move>(SDL_SCANCODE_W, false, dae::RenderComponent::UP);
	input.AddCommand<dae::Move>(SDL_SCANCODE_S, false, dae::RenderComponent::DOWN);
	input.AddCommand<dae::Move>(SDL_SCANCODE_A, false, dae::RenderComponent::LEFT);
	input.AddCommand<dae::Move>(SDL_SCANCODE_D, false, dae::RenderComponent::RIGHT);
	input.AddCommand<dae::Suicide>(32768, true);
	input.AddCommand<dae::Suicide>(SDL_SCANCODE_C, false);
	input.AddCommand<dae::Pickup>(16384, true, 10);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_Z, false, 10);
	input.AddCommand<dae::Pickup>(8192, true, 100);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_X, false, 100);
	input.AddCommand<dae::PlaySound>(SDL_SCANCODE_F, false, 0);
	input.AddCommand<dae::PlaySound>(4096, true, 0);
}

void SetupPlayer(std::string texturePath, SDL_Rect textureSrcRect, int startPos[2], std::string name, bool usesGamePad,int controllerIndex,dae::InputManager& input, dae::Scene& scene, std::shared_ptr<dae::Font> font, std::shared_ptr<dae::GameObject> scoreBoardGo, dae::GameObject* hallways) {
	auto go = std::make_shared<dae::GameObject>();

	auto PlayerLivesGo = std::make_shared<dae::GameObject>();
	PlayerLivesGo->SetParent(scoreBoardGo.get());
	PlayerLivesGo->AddComponent<dae::PlayerInfoComponent>(*PlayerLivesGo.get());
	PlayerLivesGo->SetPosition(0.f, 30.f);
	auto PlayerScoreGo = std::make_shared<dae::GameObject>();
	PlayerScoreGo->SetParent(scoreBoardGo.get());
	PlayerScoreGo->AddComponent<dae::PlayerInfoComponent>(*PlayerScoreGo.get());
	PlayerScoreGo->SetPosition(0.f, 40.f);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(*go.get());
	go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
	auto t = std::make_unique<dae::Gamepad>(controllerIndex);
	t->SetUsed(usesGamePad);
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::HandleInput>("HandleInput", *t);
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::Running>("Running");
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::HasMovementInput>("HasMovementInput");
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsDoneRunning>("IsDoneRunning");

	go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("HasMovementInput"));

	go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsDoneRunning"));
	go->GetComponent<dae::FiniteStateMachineComponent>()->SetCurrentState(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput")
	);

	//MovementComponent setup
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->GetComponent<dae::MovementComponent>()->SetHallways(hallways);

	go->GetComponent<dae::RenderComponent>()->SetTexture(texturePath, textureSrcRect);
	go->SetPosition(startPos[0], startPos[1]);


	/*std::unique_ptr gamepad = std::make_unique<dae::Gamepad>(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(std::move(gamepad));*/


	go->AddComponent<dae::PlayerComponent>(*go.get(), name);
	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8);
	auto playerComp = go->GetComponent<dae::PlayerComponent>();
	auto healthComp = go->GetComponent<dae::HealthComponent>();
	auto livesInfoComp = PlayerLivesGo->GetComponent<dae::PlayerInfoComponent>();
	auto scoreInfoComp = PlayerScoreGo->GetComponent<dae::PlayerInfoComponent>();
	if (playerComp && healthComp && livesInfoComp && scoreInfoComp) {
		PlayerLivesGo->AddComponent<dae::TextComponent>(*PlayerLivesGo, "# lives: " + std::to_string(healthComp->GetLives()), font);
		PlayerScoreGo->AddComponent<dae::TextComponent>(*PlayerScoreGo, "Score: " + std::to_string(playerComp->GetScore()), font);
		playerComp->AddObserver(scoreInfoComp);
		healthComp->AddObserver(livesInfoComp);
	}
	//go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	scene.Add(go);
	input.AddGameActor(go.get());
	scene.Add(PlayerLivesGo);
	scene.Add(PlayerScoreGo);

}

void SetupEnemy(std::string texturePath, SDL_Rect textureSrcRect, int startPos[2], std::vector<SDL_Rect> animLocations, dae::Scene& scene, dae::GameObject* hallways) {
	auto go = std::make_shared<dae::GameObject>();

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(*go.get());
	go->AddComponent<dae::FiniteStateMachineComponent>(*go.get());
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::FloatingToPlayer>("FloatingToPlayer", animLocations, 0.3f);
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::FloatingToPlayer>("FloatingToGrid", animLocations, 0.3f);
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddState<dae::Running>("Running");
	//go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::HasMovementInput>("HasMovementInput");
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::IsDoneRunning>("IsDoneRunning");
	go->GetComponent<dae::FiniteStateMachineComponent>()->AddCondition<dae::WantsToFloat>("WantsToFloat", 5.0f);

	/*go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("HasMovementInput"));

	go->GetComponent<dae::FiniteStateMachineComponent>()->AddTransition(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("Running"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput"),
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetCondition("IsDoneRunning"));
	go->GetComponent<dae::FiniteStateMachineComponent>()->SetCurrentState(
		go->GetComponent<dae::FiniteStateMachineComponent>()->GetState("HandleInput")
	);*/

	//MovementComponent setup
	go->AddComponent<dae::MovementComponent>(*go.get());
	go->GetComponent<dae::MovementComponent>()->SetDistancePerMove(16); //Remove these magic numbers
	go->GetComponent<dae::MovementComponent>()->SetTimePerMove(0.5f);
	go->GetComponent<dae::MovementComponent>()->SetHallways(hallways);

	go->GetComponent<dae::RenderComponent>()->SetTexture(texturePath, textureSrcRect);
	go->SetPosition(startPos[0], startPos[1]);


	/*std::unique_ptr gamepad = std::make_unique<dae::Gamepad>(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(std::move(gamepad));*/


	go->AddComponent<dae::HealthComponent>(*go.get(), 1, 8);
	auto healthComp = go->GetComponent<dae::HealthComponent>();
	//go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	scene.Add(go);
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
}

dae::GameObject* SetupLevelAndReturnHallwaysObject(dae::Scene& scene) {
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
			GroundObject->SetPosition(colPos, rowPos);
			scene.Add(GroundObject);
		}
	}

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::HallwaysComponent>(*go.get(), width, height);
	SetupHallwaySources(go.get());
	go->GetComponent<dae::HallwaysComponent>()->ClearSky();
	scene.Add(go);
	return go.get();
}

void SetupSound() {
	auto& soundsystem = dae::ServiceLocator::get_SoundSystem();
	soundsystem.AddEffect({ "fireball.mp3" });
}


void load()
{
	//BackGround
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	auto& input = dae::InputManager::GetInstance();

	InitializeCommands(input);

	//Parent of player info
	auto ScoreBoardParentGo = std::make_shared<dae::GameObject>();
	ScoreBoardParentGo->SetPosition(0.f, 60.f);
	scene.Add(ScoreBoardParentGo);
	auto infoGo = std::make_shared<dae::GameObject>();
	infoGo->SetParent(ScoreBoardParentGo.get());
	infoGo->AddComponent<dae::TextComponent>(*infoGo.get(), "Use the D-Pad to move DigDug, Y to inflict damage, X and B to pick up points", smallFont);
	scene.Add(infoGo);
	infoGo = std::make_shared<dae::GameObject>();
	infoGo->SetParent(ScoreBoardParentGo.get());
	infoGo->AddComponent<dae::TextComponent>(*infoGo.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to pick up points", smallFont);
	infoGo->SetPosition(0.f, 10.f);
	scene.Add(infoGo);
	infoGo = std::make_shared<dae::GameObject>();
	infoGo->SetParent(ScoreBoardParentGo.get());
	infoGo->AddComponent<dae::TextComponent>(*infoGo.get(), "Press F or A on controller to play sound.", smallFont);
	infoGo->SetPosition(0.f, -44.f);
	scene.Add(infoGo);
	infoGo = std::make_shared<dae::GameObject>();
	infoGo->SetParent(ScoreBoardParentGo.get());
	infoGo->AddComponent<dae::TextComponent>(*infoGo.get(), "(Currently doesn't play sounds due to soundfile not being found in SDL_LOADWAV.", smallFont);
	infoGo->SetPosition(0.f, -32.f);
	scene.Add(infoGo);
	infoGo = std::make_shared<dae::GameObject>();
	infoGo->SetParent(ScoreBoardParentGo.get());
	infoGo->AddComponent<dae::TextComponent>(*infoGo.get(), "However Threads are created and closed accordingly.)", smallFont);
	infoGo->SetPosition(0.f, -20.f);
	scene.Add(infoGo);

	
	auto hallways = SetupLevelAndReturnHallwaysObject(scene);
	//Player setup
	//int pos[2]{ 81,81 };
	//SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(1, 0, 14, 15), pos, "Player 1", true, input, scene, smallFont, ScoreBoardParentGo, hallways);
	int pos2[2]{ 161,81 };
	SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), pos2,"Player 2", false, 0,input, scene, smallFont, ScoreBoardParentGo, hallways);

	//Gameobject for the fps
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
	go->AddComponent<dae::FPSComponent>(*go.get());
	go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
	scene.Add(go);
	SetupSound();

}

int main(int, char* []) {
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}