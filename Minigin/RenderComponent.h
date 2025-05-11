#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include <SDL_ttf.h>
namespace dae {
    class Texture2D;
    class RenderComponent : public BaseComponent
    {
    public:
        enum Direction {
            RIGHT, LEFT, UP, DOWN
        };
        RenderComponent(GameObject& gameObject);
        void SetTexture(const std::string& filename);
        void SetTexture(const std::string& filename, SDL_Rect box);
        SDL_Rect GetBox();
        void SetBox(SDL_Rect box);
        void Render() override;
        void SetDirection(Direction direction);
    private:
        std::shared_ptr<Texture2D> m_Texture{};
        SDL_Rect m_Box;
        Direction m_Direction;

    };
}

