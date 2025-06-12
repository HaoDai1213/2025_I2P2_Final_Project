#include <cmath>
#include <string>

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "MissEffect.hpp"
#include "Scene/PlayScene.hpp"

PlayScene *MissEffect::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
MissEffect::MissEffect(float x, float y) : Sprite("play/miss.png", x, y, 120, 120, 0.5, 0.5), timeTicks(0) {
}
void MissEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= timeSpan) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
}
