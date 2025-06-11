#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "Engine/Sprite.hpp"

class Player;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Bullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    Turret *parent;
    PlayScene *getPlayScene();
    virtual void OnExplode(Player *player);

public:
    Player *Target;
    explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation);
    void Update(float deltaTime) override;
};
#endif   // BULLET_HPP
