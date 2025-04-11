#pragma once
#include <BaseComponent.h>
#include <SDL_ttf.h>
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
            LEFTTOPCORNER, RIGHTTOPCORNER, LEFTBOTTOMCORNER, RIGHTBOTTOMCORNER, CLEARED, FILLED
        };
        HallwaysComponent(GameObject& gameObject, int width, int height);
        void SetTexture(const std::string& filename);
        void Dig(const SDL_Rect fromlocation, const SDL_Rect tolocation);
        void AddSource(HallwayType type, SDL_Rect source);
        void Update() override;
        void Render() override;
    private:
        void DigRight(const SDL_Rect fromLocation, const SDL_Rect tolocation);
        void DigLeft(const SDL_Rect fromLocation, const SDL_Rect tolocation);
        void DigUp(const SDL_Rect fromLocation, const SDL_Rect tolocation);
        void DigDown(const SDL_Rect fromLocation, const SDL_Rect tolocation);
        std::shared_ptr<Texture2D> m_Texture{};
        std::vector<std::vector<HallwayType>> m_Hallways;
        std::map<HallwayType, SDL_Rect> m_Sources;
    };
}