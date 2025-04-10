#include "ThrashTheCacheComponent.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>

#include <iostream>
#include <chrono>
#include <cmath>
#include "../3rdParty/imgui-plot-master/include/imgui_plot.h"

struct Transform
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1 };
};

class GameObject3DAlt
{
public:
    Transform* transform;
    int ID;
};

class GameObject3D
{
public:
    Transform transform;
    int ID;
};

dae::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject& gameObject):
	BaseComponent(gameObject)
{

}

void dae::ThrashTheCacheComponent::Update()
{
	
}

void dae::ThrashTheCacheComponent::Render()
{
    ImGui::Begin("Exercise 2");

    // Text field to input numerical value
    ImGui::InputInt("Value", &m_NumberOfExercise2Runs);

    // Buttons to run methods
    if (ImGui::Button("Thrash the cache")) {
        ThrashTheCacheInt();
    }
    if (!m_IntAverage.empty()) {
        ImGui::PlotConfig config;
        config.values.xs = m_Steps.data();
        config.values.ys = m_IntAverage.data();
        config.values.count = static_cast<int>(m_IntAverage.size());
        config.scale.min = 0.f;
        config.scale.max = m_IntAverage.at(0);
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(200, 200);
        config.line_thickness = 2.f;
        config.values.color = ImColor(0, 255, 0);
        ImGui::Plot("Int Plot", config);
    }
    ImGui::End();
    // Create a new ImGui window
    ImGui::Begin("Exercise 3");

    // Text field to input numerical value
    ImGui::InputInt("Value", &m_NumberOfExercise3Runs);

    // Buttons to run methods
    if (ImGui::Button("Thrash the cache with GameObject3D")) {
        ThrashTheCache();
    }
    if (!m_StandardAverage.empty()) {
        ImGui::PlotConfig config;
        config.values.xs = m_Steps.data();
        config.values.ys = m_StandardAverage.data();
        config.values.count = static_cast<int>(m_StandardAverage.size());
        config.scale.min = 0.f;
        config.scale.max = m_StandardAverage.at(0);
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(200, 200);
        config.line_thickness = 2.f;
        config.values.color = ImColor(0, 255, 0);
        ImGui::Plot("Standard Plot", config);
    }
    if (ImGui::Button("Thrash the cache with GameObject3DAlt")) {
        ThrashTheCacheAlt();
    }    
    if (!m_AltAverage.empty()) {
        ImGui::PlotConfig config;
        config.values.xs = m_Steps.data();
        config.values.ys = m_AltAverage.data();
        config.values.count = static_cast<int>(m_AltAverage.size());
        config.scale.min = 0;
        config.scale.max = m_AltAverage.at(0);
        config.tooltip.show = true;
        config.tooltip.format = "x=%.2f, y=%.2f";
        config.grid_x.show = true;
        config.grid_y.show = true;
        config.frame_size = ImVec2(200, 200);
        config.line_thickness = 2.f;
        config.values.color = ImColor(0, 255, 0);
        ImGui::Plot("Alt Plot", config);
    }
    if (!m_StandardAverage.empty() && !m_AltAverage.empty()) {
        ImGui::PlotConfig conf;
        conf.values.xs = m_Steps.data();
        conf.values.count = static_cast<int>(m_Steps.size());
        const float* y_Data[] = { m_StandardAverage.data(), m_AltAverage.data() };
        conf.values.ys_list = y_Data; // use ys_list to draw several lines simultaneously
        conf.values.ys_count = 2;
        ImU32 colors[2] = { ImColor(0, 255, 0), ImColor(255, 0, 0)};
        conf.values.colors = colors;
        conf.scale.min = 0;
        conf.scale.max = m_StandardAverage.at(0);
        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_x.size = 128;
        conf.grid_x.subticks = 4;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.grid_y.subticks = 5;
        conf.frame_size = ImVec2(200, 200);
        ImGui::Plot("Combined Plot", conf);
    }
    ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void dae::ThrashTheCacheComponent::ThrashTheCacheInt()
{
    m_IntAverage.clear();
    m_Steps.clear();
    std::vector<std::vector<float>> intResults;
    for (int sampleLooper = 0; sampleLooper < m_NumberOfExercise2Runs; ++sampleLooper) {
        std::vector<int> arr(static_cast<int>(std::pow(2, 26)));
        std::vector<float> results;
        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            if (sampleLooper == 0) {
                m_Steps.emplace_back(static_cast<float>(stepsize));
            }
            auto stepStartTime = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr.size(); i += stepsize)
            {
                arr[i] *= 2;
            }
            auto stepEndTime = std::chrono::high_resolution_clock::now();
            results.emplace_back(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(stepEndTime - stepStartTime).count()));
        }
        intResults.emplace_back(results);
    }
    SetAverageVector(intResults, dae::ThrashTheCacheComponent::VectorType::Int);
}

void dae::ThrashTheCacheComponent::ThrashTheCache()
{
    m_StandardAverage.clear();
    m_Steps.clear();
    std::vector<std::vector<float>> standardResults;
    for (int sampleLooper = 0; sampleLooper < m_NumberOfExercise3Runs; ++sampleLooper) {
        std::vector<GameObject3DAlt> arr(static_cast<int>(std::pow(2, 26)));
        std::vector<float> results;
        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            if (sampleLooper == 0) {
                m_Steps.emplace_back(static_cast<float>(stepsize));
            }
            auto stepStartTime = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr.size(); i += stepsize)
            {
                arr[i].ID *= 2;
            }
            auto stepEndTime = std::chrono::high_resolution_clock::now();
            results.emplace_back(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(stepEndTime - stepStartTime).count()));
        }
        standardResults.emplace_back(results);
    }
    SetAverageVector(standardResults, dae::ThrashTheCacheComponent::VectorType::Standard);
}

void dae::ThrashTheCacheComponent::ThrashTheCacheAlt()
{
    m_AltAverage.clear();
    m_Steps.clear();
    std::vector<std::vector<float>> altResults;
    for (int sampleLooper = 0; sampleLooper < m_NumberOfExercise3Runs; ++sampleLooper) {
        std::vector<GameObject3DAlt> arr(static_cast<int>(std::pow(2, 26)));
        std::vector<float> results;
        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            if (sampleLooper == 0) {
                m_Steps.emplace_back(static_cast<float>(stepsize));
            }
            auto stepStartTime = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr.size(); i += stepsize)
            {
                arr[i].ID *= 2;
            }
            auto stepEndTime = std::chrono::high_resolution_clock::now();
            results.emplace_back(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(stepEndTime - stepStartTime).count()));
        }
        altResults.emplace_back(results);
    }
    SetAverageVector(altResults, dae::ThrashTheCacheComponent::VectorType::Alt);
}

void dae::ThrashTheCacheComponent::SetAverageVector(const std::vector<std::vector<float>>& runResults, VectorType type)
{
    for (int i = 0; i < static_cast<int>(runResults.at(0).size()); ++i) {
        float sum = 0.0;
        switch (type) {
        case dae::ThrashTheCacheComponent::VectorType::Int:
            // Sum the values at index i from all vectors
            for (int j = 0; j < m_NumberOfExercise2Runs; ++j) {
                sum += runResults[j][i];
            }
            // Calculate the average and store it in the result vector
            m_IntAverage.emplace_back(sum / m_NumberOfExercise2Runs);
            break;
        case dae::ThrashTheCacheComponent::VectorType::Standard:
            // Sum the values at index i from all vectors
            for (int j = 0; j < m_NumberOfExercise3Runs; ++j) {
                sum += runResults[j][i];
            }
            // Calculate the average and store it in the result vector
            m_StandardAverage.emplace_back(sum / m_NumberOfExercise3Runs);
            break;
        case dae::ThrashTheCacheComponent::VectorType::Alt:
            // Sum the values at index i from all vectors
            for (int j = 0; j < m_NumberOfExercise3Runs; ++j) {
                sum += runResults[j][i];
            }
            // Calculate the average and store it in the result vector
            m_AltAverage.emplace_back(sum / m_NumberOfExercise3Runs);
            break;
        }
    }
}
