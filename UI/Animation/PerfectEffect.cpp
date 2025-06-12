#include <cmath>
#include <string>

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "PerfectEffect.hpp"
#include "Scene/PlayScene.hpp"

PlayScene *PerfectEffect::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
PerfectEffect::PerfectEffect(float x, float y) : Sprite("play/perfect.png", x, y), timeTicks(0) {
}
void PerfectEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= timeSpan) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
}
