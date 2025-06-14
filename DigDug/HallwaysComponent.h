#pragma once
#include <BaseComponent.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include "vector"
#include "map"

namespace dae {
    class Texture2D;
    class GameObject;
    class HallwaysComponent :
        public BaseComponent
    {
    public:
        enum HallwayType {
            TOPCLOSED, BOTTOMCLOSED, LEFTCLOSED, RIGHTCLOSED, VERTICALTHROUGH, HORIZONTALTHROUGH,
            LEFTTOPCORNER, RIGHTTOPCORNER, LEFTBOTTOMCORNER, RIGHTBOTTOMCORNER, CLEARED, FILLED, SKY
        };
        HallwaysComponent(GameObject& gameObject, int width, int height);
        void ClearSky();
        void SetTexture(const std::string& filename);
        void Dig(const glm::vec3& fromlocation, const glm::vec3& tolocation);
        void AddSource(HallwayType type, const SDL_Rect& source);
        const HallwayType& GetHallwayType(const glm::vec3& location);
        bool IsValidHallway(const glm::vec3& location);
        glm::vec3 GetFreeHallwayLocation(const glm::vec3& location);
        void SetHallwayType(const std::pair<int, int>& index, HallwayType desiredType);
        void Update() override;
        void Render() override;
    private:
        void DigRight(const glm::vec3& fromLocation, const glm::vec3& tolocation);
        void DigLeft(const glm::vec3& fromLocation, const glm::vec3& tolocation);
        void DigUp(const glm::vec3& fromLocation, const glm::vec3& tolocation);
        void DigDown(const glm::vec3& fromLocation, const glm::vec3& tolocation);
        std::shared_ptr<Texture2D> m_Texture{};
        std::vector<std::vector<HallwayType>> m_Hallways;
        std::map<HallwayType, SDL_Rect> m_Sources;
    };
}