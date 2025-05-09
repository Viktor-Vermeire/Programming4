#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "filesystem"
#include "iostream"

SDL_Window* g_window{};

float dae::Minigin::DELTATIME = 0.f;

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::filesystem::path &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! " << Mix_GetError() << std::endl;
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	HDC screen = GetDC(nullptr);
	m_RefreshRate = GetDeviceCaps(screen, VREFRESH);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
	ServiceLocator::register_SoundSystem(std::make_unique<SDLSoundSystem>(dataPath));
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	
	load();
	m_CurrentTime = std::chrono::system_clock::now();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	

	//input.InitializeCommands();
	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		UpdateDeltaTime();
		if (DELTATIME < 1.f / m_RefreshRate) {
			Sleep(static_cast<DWORD>(1.f / m_RefreshRate - DELTATIME));
			DELTATIME += 1.f / static_cast<float>(m_RefreshRate) - DELTATIME;
		}
		doContinue = input.CheckExit();
		sceneManager.Update();
		ServiceLocator::get_SoundSystem().CleanupSoundThreads();
		renderer.Render();
		DELTATIME = 0;
	}
}

void dae::Minigin::UpdateDeltaTime()
{
	auto tempTime = std::chrono::system_clock::now();
	//std::cout << static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(tempTime - m_CurrentTime).count()) / 1000.f << "\n";
	DELTATIME += static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(tempTime - m_CurrentTime).count()) / 1000.f;
	m_CurrentTime = std::move(tempTime);
}
