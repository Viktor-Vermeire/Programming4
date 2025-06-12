#pragma once
#include "SoundSystem.h"
#include "memory"
#include <thread>
#include <SDL_mixer.h> 
#include "vector"
#include "filesystem"
#include "queue"

namespace dae {
	class SDLSoundSystemImpl;
	class SDLSoundSystem : public SoundSystem
	{
	public:
		SDLSoundSystem(const std::filesystem::path& dataPath);
		
		void AddEffects(std::vector<const char*> effects) override;
		void AddSongs(std::vector<const char*> effects) override;
		void PlayEffect(const unsigned short id, const float volume) override;
		void PlayMusic(const unsigned short id, const float volume) override;
		
	private:
		std::unique_ptr<SDLSoundSystemImpl> m_SDLSoundSystemImpl;
		

	};
}

