#pragma once
#include "memory"
#include "SoundSystem.h"
namespace dae {
	/*class null_sound_system final : public SoundSystem
	{
		void play(const unsigned short, const float) override {}
	};*/

	class ServiceLocator final
	{
		static inline std::unique_ptr<SoundSystem> _ss_instance = nullptr;
	public:
		static SoundSystem& get_SoundSystem() { return *_ss_instance; }
		static void register_SoundSystem(std::unique_ptr<SoundSystem>&& ss) {
			//_ss_instance = ss == nullptr ? std::make_unique<null_sound_system>() : std::move(ss);
			_ss_instance = std::move(ss);
		};
	};
}