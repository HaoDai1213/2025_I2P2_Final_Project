#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>

#include "Engine/Sprite.hpp"

class PlayScene;
namespace Engine {
    struct Point;
}   // namespace Engine

class Note : public Engine::Sprite {
protected:
    int timing;
    int type;   // 1 for red, 2 for blue
    int line;   // 0 for top, 1 for down
    float speed;
    float speedMult;
    PlayScene *getPlayScene();
    virtual void OnExplode();

public:
    int judgement_pf;
    int judgement_gr;
    int judgement_ms;
    explicit Note(std::string img, int timing, int line, int type, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection);
    void Update(float deltaTime) override;
    void Draw() const override;
    void HitNote(int timing);
};

#endif  // NOTE_HPP