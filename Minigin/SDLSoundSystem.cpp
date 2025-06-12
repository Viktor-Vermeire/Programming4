#include "SDLSoundSystem.h"
#include <SDL_mixer.h> 
#include "vector"
#include "memory"
#include <iostream>
#include <mutex>
#include "filesystem"

namespace dae {
	enum SoundType {
		EFFECT,MUSIC
	};
	struct SoundInfo {
		int index;
		float volume;
		SoundType soundType;
	};
	class SDLSoundSystemImpl {
	public:
		~SDLSoundSystemImpl();
		SDLSoundSystemImpl(const std::filesystem::path& dataPath);

		void AddEffects(std::vector<const char*> effects);
		void AddSongs(std::vector<const char*> songs);
		void PlayEffect(const unsigned short id, const float volume);
		void PlayMusic(const unsigned short id, const float volume);
	private:
		void HandleSoundQueue();
		std::mutex mtx;
		std::vector<Mix_Chunk*> m_Effects;
		std::vector<Mix_Music*> m_Songs;
		std::condition_variable m_CondiVar;
		std::jthread m_SoundThread;
		std::filesystem::path m_DataPath;
		bool m_ThreadLifeTime{ true };
		std::queue <SoundInfo> m_SoundQueue;
	};
	SDLSoundSystem::SDLSoundSystem(const std::filesystem::path& dataPath): m_SDLSoundSystemImpl{std::make_unique<SDLSoundSystemImpl>(dataPath)}
	{
	}

	void SDLSoundSystem::AddEffects(std::vector<const char*> effects)
	{
		m_SDLSoundSystemImpl->AddEffects(effects);
	}
	void SDLSoundSystem::AddSongs(std::vector<const char*> songs)
	{
		m_SDLSoundSystemImpl->AddSongs(songs);
	}

	void SDLSoundSystem::PlayEffect(const unsigned short id, const float volume)
	{
		m_SDLSoundSystemImpl->PlayEffect(id, volume);
	}
	void SDLSoundSystem::PlayMusic(const unsigned short id, const float volume)
	{
		m_SDLSoundSystemImpl->PlayMusic(id, volume);
	}

	void SDLSoundSystemImpl::PlayEffect(const unsigned short id, const float volume)
	{
		m_SoundQueue.push(SoundInfo{id,volume, SoundType::EFFECT});
		std::lock_guard<std::mutex> lock(mtx);
		m_CondiVar.notify_all();
	}

	void SDLSoundSystemImpl::PlayMusic(const unsigned short id, const float volume)
	{
		m_SoundQueue.push(SoundInfo{ id,volume, SoundType::MUSIC });
		std::lock_guard<std::mutex> lock(mtx);
		m_CondiVar.notify_all();
	}

	void SDLSoundSystemImpl::HandleSoundQueue()
	{
		while (m_ThreadLifeTime) {
			std::unique_lock<std::mutex> lock(mtx);
			m_CondiVar.wait(lock, [&] {return !m_SoundQueue.empty() || !m_ThreadLifeTime;});
			if (!m_ThreadLifeTime)
				break;
			
			auto soundInfo = m_SoundQueue.front();
			switch (soundInfo.soundType) {
			case EFFECT:
				Mix_PlayChannel(-1, m_Effects[soundInfo.index], 0);
				break;
			case MUSIC:
				Mix_FadeInMusic(m_Songs[soundInfo.index], -1, 10);
				break;
			}
			
			m_SoundQueue.pop();

		}
	}

	SDLSoundSystemImpl::~SDLSoundSystemImpl()
	{
		std::lock_guard<std::mutex> lock(mtx);
		m_ThreadLifeTime = false;
		m_SoundQueue.push({ 1,1 });
		m_CondiVar.notify_all();
	}

	SDLSoundSystemImpl::SDLSoundSystemImpl(const std::filesystem::path& dataPath): m_DataPath{dataPath}
	{
		m_SoundThread = std::jthread(&SDLSoundSystemImpl::HandleSoundQueue, this);
	}

	void SDLSoundSystemImpl::AddEffects(std::vector<const char*> effects)
	{
		for (int looper{0}; looper < effects.size(); ++looper) {
			const auto fullPath = m_DataPath / effects.at(looper);
			 //HERE IS THE ISSUE NO FILE IS FOUND DESPITE IT'S 
			auto* thing = Mix_LoadWAV(fullPath.generic_string().c_str());
			m_Effects.push_back(thing);
		}
	}
	void SDLSoundSystemImpl::AddSongs(std::vector<const char*> songs)
	{
		for (int looper{ 0 }; looper < songs.size(); ++looper) {
			const auto fullPath = m_DataPath / songs.at(looper);
			//HERE IS THE ISSUE NO FILE IS FOUND DESPITE IT'S 
			auto* thing = Mix_LoadMUS(fullPath.generic_string().c_str());
			m_Songs.push_back(thing);
		}
	}
}