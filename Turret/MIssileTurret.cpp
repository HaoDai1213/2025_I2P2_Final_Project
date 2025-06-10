#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/Missile.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "MissileTurret.hpp"
#include "Scene/PlayScene.hpp"

const int MissileTurret::Price = 400;
MissileTurret::MissileTurret(float x, float y)
    : Turret("play/tower-base.png", "play/turret-6.png", x, y, 200, Price, 2) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void MissileTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new Missile(Position + normalized * 36, diff, rotation, this));
    AudioHelper::PlayAudio("missile.wav");
}