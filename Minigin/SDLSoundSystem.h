#pragma once
#include "SoundSystem.h"
#include "memory"
#include <thread>
#include <SDL_mixer.h> 
#include "vector"
#include "filesystem"

namespace dae {
	class SDLSoundSystemImpl;
	class SDLSoundSystem : public SoundSystem
	{
	public:
		SDLSoundSystem(const std::filesystem::path& dataPath);
		
		void AddEffect(std::vector<const char*> effects) override;
		void play(const unsigned short id, const float volume) override;
		void CleanupSoundThreads() override;
		
	private:
		std::unique_ptr<SDLSoundSystemImpl> m_SDLSoundSystemImpl;
		

	};
}

