#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Shovel.hpp"
#include "Scene/PlayScene.hpp"

const int Shovel::Price = 0;
Shovel::Shovel(float x, float y)
    : Turret("play/null.png", "play/shovel.png", x, y, 75, Price, 0) {
    Anchor.y += 8.0f / GetBitmapHeight();
}
void Shovel::CreateBullet() {}