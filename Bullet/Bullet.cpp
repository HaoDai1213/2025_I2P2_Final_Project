#include "Bullet.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Player/Player.hpp"
#include "Scene/PlayScene.hpp"

PlayScene *Bullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Bullet::OnExplode(Player *player) {
}
Bullet::Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation) : Sprite(img, position.x, position.y), speed(speed), damage(damage) {
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 4;
}
void Bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    Player *player = scene->player;
    if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, player->Position, player->CollisionRadius)) {
        OnExplode(player);
        player->Hit(damage);
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }

    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize())) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
    }
}
