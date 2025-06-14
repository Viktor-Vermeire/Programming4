#pragma once
#include "memory"
#include "SoundSystem.h"
namespace dae {
	/*class null_sound_system final : public SoundSystem
	{
		void play(const unsigned short, const float) override {}
	};*/
	class NullSoundSystem final : public SoundSystem {
	public:
		//NullSoundSystem() {};
		void PlayEffect(const unsigned short, const float) override{};
		void PlayMusic(const unsigned short, const float) override {};
		void AddEffects(std::vector<const char*>) override{};
		void AddSongs(std::vector<const char*>) override {};
		void ToggleMute() override {};
	};
	class ServiceLocator final
	{
		
		static std::unique_ptr<SoundSystem> _ss_instance;
	public:
		static SoundSystem& get_SoundSystem() { 
				return *_ss_instance;
		}
		static void register_SoundSystem(std::unique_ptr<SoundSystem>&& ss) {
			_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
		};
	};
}