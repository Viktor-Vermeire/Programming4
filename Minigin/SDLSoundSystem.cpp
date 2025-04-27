#include "SDLSoundSystem.h"
#include <SDL_mixer.h> 
#include "vector"
#include "memory"
#include <iostream>
#include <mutex>
#include "filesystem"

std::mutex mtx0, mtx1;

namespace dae {
	class SDLSoundSystemImpl {
	public:
		SDLSoundSystemImpl(const std::filesystem::path& dataPath);

		void AddEffect(std::vector<const char*> effects);
		void play(const unsigned short id, const float volume);
		void CleanupSoundThreads();
	private:
		void playSound(const unsigned short id, const float volume);
		std::vector<Mix_Chunk*> m_Effects;
		std::vector<std::jthread> threads;
		std::filesystem::path m_DataPath;
	};
	SDLSoundSystem::SDLSoundSystem(const std::filesystem::path& dataPath): m_SDLSoundSystemImpl{std::make_unique<SDLSoundSystemImpl>(dataPath)}
	{
	}

	void SDLSoundSystem::AddEffect(std::vector<const char*> effects)
	{
		m_SDLSoundSystemImpl->AddEffect(effects);
	}

	void SDLSoundSystem::play(const unsigned short id, const float volume)
	{
		m_SDLSoundSystemImpl->play(id, volume);
	}

	void SDLSoundSystem::CleanupSoundThreads()
	{
		m_SDLSoundSystemImpl->CleanupSoundThreads();
	}


	void SDLSoundSystemImpl::play(const unsigned short id, const float volume)
	{
		auto volume1 = volume;
		auto id2 = id;
		volume1 = volume1 + id2;
		threads.emplace_back(std::jthread(&SDLSoundSystemImpl::playSound,this, id, volume)); //This might cause issues down the line
	}
	void SDLSoundSystemImpl::CleanupSoundThreads()
	{
		threads.erase(
			std::remove_if(threads.begin(), threads.end(),
				[](const std::jthread& thread) { return !thread.joinable(); }),
			threads.end());
	}
	void SDLSoundSystemImpl::playSound(const unsigned short id, const float volume)
	{
		std::scoped_lock lock(mtx0, mtx1);
		/*int channel = */ Mix_PlayChannel(-1, m_Effects.at(id), 0);
		volume;
		//Mix_Volume(channel, static_cast<int>(volume));
	}

	SDLSoundSystemImpl::SDLSoundSystemImpl(const std::filesystem::path& dataPath): m_DataPath{dataPath}
	{
	}

	void SDLSoundSystemImpl::AddEffect(std::vector<const char*> effects)
	{
		
		for (int looper{0}; looper < effects.size(); ++looper) {
			const auto fullPath = m_DataPath / effects.at(looper);
			 //HERE IS THE ISSUE NO FILE IS FOUND DESPITE IT'S PRESENCE
			m_Effects.push_back(Mix_LoadWAV(fullPath.generic_string().c_str()));
		}
	}
}