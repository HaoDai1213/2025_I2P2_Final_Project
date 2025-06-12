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
    int appearTiming;
    int hitTiming;
    int type;   // 1 for red, 2 for blue
    int line;   // 0 for top, 1 for down
    float speed;
    float speedMult;
    PlayScene *getPlayScene();
    virtual void OnExplode();

public:
    static int judgement_pf;
    static int judgement_gr;
    static int judgement_ms;
    explicit Note(std::string img, int appearTiming, int hitTimung, int line, int type, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection);
    void Update(float deltaTime) override;
    void Draw() const override;
    int getHitTiming();
    int getType();
};

#endif  // NOTE_HPP