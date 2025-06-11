#ifndef FIREBULLET_HPP
#define FIREBULLET_HPP
#include "Bullet.hpp"

class Player;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class FireBullet : public Bullet {
public:
    explicit FireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, float speed);
    void OnExplode(Player *player) override;
};
#endif   // FIREBULLET_HPP
