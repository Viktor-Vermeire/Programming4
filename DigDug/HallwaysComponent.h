#pragma once
#include <BaseComponent.h>
#include <SDL_ttf.h>
#include "vector"

namespace dae {
    class Texture2D;
    class GameObject;
    class HallwaysComponent :
        public BaseComponent
    {
    public:
        HallwaysComponent(GameObject& gameObject, int width, int height);
        void SetTexture(const std::string& filename, SDL_Rect box);
        void AddPlayer(GameObject* player);
        void Update() override;
        void Render() override;
    private:
        std::shared_ptr<Texture2D> m_Texture{};
        SDL_Rect m_Box;
        std::vector<std::vector<bool>> m_HasBeenDug;
        std::vector<GameObject*>  m_Players;
    };
}