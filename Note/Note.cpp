#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Note/Note.hpp"
#include "Scene/PlayScene.hpp"
int judgement_pf = 25;
int judgement_gr = 75;
int judgement_ms = 108;

PlayScene *Note::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Note::OnExplode() {
}

Note::Note(std::string img, int timing, int line, int type, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection) : Sprite(img, position.x, position.y), timing(timing), type(type), speed(speed), speedMult(speedMult) {
    Velocity = forwardDirection.Normalize() * speed * speedMult;
}

void Note::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    // the judgement here?
}

void Note::Draw() const {
    Sprite::Draw();
}

void Note::HitNote(int hitTime) {   // ?
    if (hitTime >= timing - judgement_ms && hitTime <= timing + judgement_ms) {
        if (hitTime >= timing - judgement_pf && hitTime <= timing + judgement_pf) {

        }
        else if (hitTime >= timing - judgement_gr && hitTime <= timing + judgement_gr) {

        }
        else {
            
        }
    }
}