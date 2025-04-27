#include "SDLSoundSystem.h"
#include <SDL_mixer.h> 
#include "vector"
#include "memory"
#include <iostream>
#include <mutex>

std::mutex mtx0, mtx1;

namespace dae {
	void SDLSoundSystem::play(const unsigned short id, const float volume)
	{
		auto volume1 = volume;
		auto id2 = id;
		volume1 = volume1 + id2;
		threads.emplace_back(std::jthread(&SDLSoundSystem::playSound,this, id, volume)); //This might cause issues down the line
	}
	void SDLSoundSystem::CleanupSoundThreads()
	{
		threads.erase(
			std::remove_if(threads.begin(), threads.end(),
				[](const std::jthread& thread) { return !thread.joinable(); }),
			threads.end());
	}
	void SDLSoundSystem::playSound(const unsigned short id, const float volume)
	{
		std::scoped_lock lock(mtx0, mtx1);
		int channel = Mix_PlayChannel(-1, m_Effects.at(id).get(), 0);
		Mix_Volume(channel, static_cast<int>(volume));
	}
	void SDLSoundSystem::AddEffect(std::vector<const char*> effects)
	{
		for (int looper{0}; looper < effects.size(); ++looper) {
			//auto path = effects[looper];
			Mix_Chunk* thing = Mix_LoadWAV("fireball.mp3"); //HERE IS THE ISSUE
			m_Effects.push_back(std::make_unique<Mix_Chunk>(*thing));
		}
	}
}