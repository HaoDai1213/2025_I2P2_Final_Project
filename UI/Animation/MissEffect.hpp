#ifndef MISSEFFECT_HPP
#define MISSEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"

class PlayScene;

class MissEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();
    float timeTicks;
    float timeSpan = 0.2;

public:
    MissEffect(float x, float y);
    void Update(float deltaTime) override;
};
#endif   // MISSEFFECT_HPP
