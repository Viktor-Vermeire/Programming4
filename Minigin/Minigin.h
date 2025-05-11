#pragma once
#include <string>
#include <functional>
#include <chrono>
#include "filesystem"
#include <random>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
		static float DELTATIME;
		static std::mt19937 RANDOM;
		
	private:
		int m_RefreshRate;
		float m_PassedTime;
		void UpdateDeltaTime();
		std::chrono::system_clock::time_point m_CurrentTime;
	};
}