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
#include "DiggingComponent.h"
#include "RockComponent.h"
#include "HallwaysComponent.h"

void InitializeCommands(dae::InputManager& input)
{
	input.AddCommand<dae::Move>(1, true, std::pair<float, float>(0.f, -1.f));
	input.AddCommand<dae::Move>(2, true, std::pair<float, float>(0.f, 1.f));
	input.AddCommand<dae::Move>(4, true, std::pair<float, float>(-1.f, 0.f));
	input.AddCommand<dae::Move>(8, true, std::pair<float, float>(1.f, 0.f));
	input.AddCommand<dae::Move>(SDL_SCANCODE_W, false, std::pair<float, float>(0.f, -2.f));
	input.AddCommand<dae::Move>(SDL_SCANCODE_S, false, std::pair<float, float>(0.f, 2.f));
	input.AddCommand<dae::Move>(SDL_SCANCODE_A, false, std::pair<float, float>(-2.f, 0.f));
	input.AddCommand<dae::Move>(SDL_SCANCODE_D, false, std::pair<float, float>(2.f, 0.f));
	input.AddCommand<dae::Suicide>(32768, true);
	input.AddCommand<dae::Suicide>(SDL_SCANCODE_C, false);
	input.AddCommand<dae::Pickup>(16384, true, 10);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_Z, false, 10);
	input.AddCommand<dae::Pickup>(8192, true, 100);
	input.AddCommand<dae::Pickup>(SDL_SCANCODE_X, false, 100);
}

void SetupPlayer(std::string texturePath, SDL_Rect textureSrcRect, int startPos[2], std::string name, bool usesGamePad,dae::InputManager& input, dae::Scene& scene, std::shared_ptr<dae::Font> font, std::shared_ptr<dae::GameObject> scoreBoardGo, dae::GameObject* hallways) {
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
	go->GetComponent<dae::RenderComponent>()->SetTexture(texturePath, textureSrcRect);
	go->SetPosition(startPos[0], startPos[1]);
	dae::Gamepad* gamepad = new dae::Gamepad(input.GetGameActorSize());
	gamepad->SetUsed(usesGamePad);
	input.AddGamepad(gamepad);
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
	go->AddComponent<dae::DiggingComponent>(*go.get(), hallways->GetComponent<dae::HallwaysComponent>());
	//go->SetParent(nullptr); //This was a test for resetting a gameobject to the root
	scene.Add(go);
	input.AddGameActor(go.get());
	scene.Add(PlayerLivesGo);
	scene.Add(PlayerScoreGo);
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
			std::shared_ptr<dae::GameObject> go = std::make_shared<dae::GameObject>();
			go->AddComponent<dae::RenderComponent>(*go.get());
			if (rowPos < layerthickness)
				go->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(73, 14, 8, 8));
			else if (rowPos < layerthickness * 2)
				go->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(73, 32, 8, 8));
			else if (rowPos < layerthickness * 3)
				go->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 14, 8, 8));
			else if (rowPos < layerthickness * 4)
				go->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 23, 8, 8));
			else
				go->GetComponent<dae::RenderComponent>()->SetTexture("DigDug_Tiles_Logos_Text.png", SDL_Rect(64, 32, 8, 8));
			
			go->AddComponent<dae::RockComponent>(*go.get());
			go->SetPosition(colPos, rowPos);
			scene.Add(go);
		}
	}

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::HallwaysComponent>(*go.get(), width, height);
	SetupHallwaySources(go.get());
	scene.Add(go);
	return go.get();
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

	
	auto hallways = SetupLevelAndReturnHallwaysObject(scene);
	//Player setup
	int pos[2]{ 170,130 };
	SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(1, 0, 14, 15), pos, "Player 1", false, input, scene, smallFont, ScoreBoardParentGo, hallways);
	//int pos2[2]{ 140,100 };
	//SetupPlayer("DigDug_General_Sprites.png", SDL_Rect(16, 15, 14, 15), pos2,"Player 2", false, input, scene, smallFont, ScoreBoardParentGo);

	//Gameobject for the fps
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>(*go.get(), "here", smallFont);
	go->AddComponent<dae::FPSComponent>(*go.get());
	go->GetComponent<dae::FPSComponent>()->SetToWriteComponent(go->GetComponent<dae::TextComponent>());
	scene.Add(go);

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}