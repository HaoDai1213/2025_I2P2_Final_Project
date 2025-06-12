#ifndef GREATEFFECT_HPP
#define GREATEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"

class PlayScene;

class GreatEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();
    float timeTicks;
    float timeSpan = 0.2;

public:
    GreatEffect(float x, float y);
    void Update(float deltaTime) override;
};
#endif   // GREATEFFECT_HPP
