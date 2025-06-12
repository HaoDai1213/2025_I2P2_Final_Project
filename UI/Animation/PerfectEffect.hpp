#ifndef PERFECTEffect_HPP
#define PERFECTEffect_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"

class PlayScene;

class PerfectEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();
    float timeTicks;
    float timeSpan = 0.2;

public:
    PerfectEffect(float x, float y);
    void Update(float deltaTime) override;
};
#endif   // PERFECTEffect_HPP
