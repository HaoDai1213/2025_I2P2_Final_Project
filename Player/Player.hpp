#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class PlayScene;

// take Enemy.hpp as a referencce
class Player : public Engine::Sprite {
protected:
    float speed;
    float hp;
    PlayScene *getPlayScene();
    bool isHit;

public:
    Player(std::string img, float x, float y);
    void Hit(float damage);
    void Update(float deltaTime) override;
    void Draw() const override;
    int getSpeed();
};


#endif   // PLAYER_HPP