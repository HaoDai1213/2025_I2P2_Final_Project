#include <allegro5/base.h>
#include <random>
#include <string>

#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "FireBullet.hpp"
#include "Player/Player.hpp"
#include "Scene/PlayScene.hpp"

FireBullet::FireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, float speed) : Bullet("play/bullet-7.png", speed, 10, position, forwardDirection, rotation - ALLEGRO_PI / 2) {
}
void FireBullet::OnExplode(Player *player) {
}
