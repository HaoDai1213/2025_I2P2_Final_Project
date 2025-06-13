#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Player.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"


PlayScene *Player::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Player::Player(std::string img, float x, float y) : Engine::Sprite(img, x, y) {
    speed = 8;
    hp = 100;
    CollisionRadius = 24;
    alive = true;
}

void Player::Hit(float damage) {
    hp -= damage;
    Engine::LOG(Engine::INFO) << "Player is hit: " << hp << " hp left";
    AudioHelper::PlayAudio("playerhit.wav");
    if (hp <= 0) {
        alive = false;
    }
}

void Player::Update(float deltaTime) {
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}

void Player::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
}

int Player::getSpeed() {
    return speed;
}

int Player::getHp() {
    return hp;
}