#pragma once
#include "BaseComponent.h"
#include <vector>
namespace dae {
	class ThrashTheCacheComponent : public BaseComponent
	{
	public:
		
		ThrashTheCacheComponent(GameObject& gameObject);
		void Update() override;
		void Render() override;
	private:
		enum struct VectorType { Int, Standard, Alt };
		void ThrashTheCacheInt();
		void ThrashTheCache();
		void ThrashTheCacheAlt();
		void SetAverageVector(const std::vector<std::vector<float>>& runResults, VectorType isAverage);
		int m_NumberOfExercise3Runs{1};
		int m_NumberOfExercise2Runs{ 1 };
		std::vector<float> m_Steps;
		std::vector<float> m_IntAverage;
		std::vector<float> m_StandardAverage;
		std::vector<float> m_AltAverage;
		
	};
}

