#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Note/Note.hpp"
#include "Scene/PlayScene.hpp"
int Note::judgement_pf = 41;
int Note::judgement_gr = 108;
int Note::judgement_ms = 125;

PlayScene *Note::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Note::OnExplode() {
}

Note::Note(std::string img, int appearTiming, int hitTiming, int line, int type, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection) : Sprite(img, position.x, position.y, 110, 110, 0.5, 0.5), appearTiming(appearTiming), hitTiming(hitTiming), type(type), speed(speed), speedMult(speedMult) {
    Velocity = forwardDirection.Normalize() * speed * speedMult;
}

void Note::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    // the judgement here?

    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize())) {
        Engine::LOG(Engine::INFO) << "miss! ";
        getPlayScene()->NoteGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("miss.mp3");
    }
}

void Note::Draw() const {
    Sprite::Draw();
}

int Note::getHitTiming() {
    return hitTiming;
}

int Note::getType() {
    return type;
}