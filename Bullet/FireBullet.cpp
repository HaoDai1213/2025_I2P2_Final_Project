#include <allegro5/base.h>
#include <random>
#include <string>

#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "FireBullet.hpp"
#include "Player/Player.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

FireBullet::FireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, float speed) : Bullet("play/bullet-7.png", speed, 10, position, forwardDirection, rotation - ALLEGRO_PI / 2) {
}
void FireBullet::OnExplode(Player *player) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), player->Position.x, player->Position.y));
}
