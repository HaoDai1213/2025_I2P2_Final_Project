#ifndef MISSILE_HPP
#define MISSILE_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Missile : public Bullet {
private:
    int ExplosionRadius;
public:
    explicit Missile(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
    void Update(float deltaTime) override;
};
#endif   // MISSILE_HPP
