#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "MechEnemy.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
MechEnemy::MechEnemy(int x, int y, int stage) : Enemy("play/enemy-" + std::to_string(7 - stage) + ".png", x, y, 20, 30, 100 - stage * 30, 125 - stage * 25) {
    curStage = stage;
}

void MechEnemy::OnExplode() {
    Enemy::OnExplode();
    if (curStage < 3) {
        Enemy *enemy;
        getPlayScene()->EnemyGroup->AddNewObject(enemy = new MechEnemy(Position.x, Position.y, curStage + 1));
        enemy->UpdatePath(getPlayScene()->mapDistance);
        enemy->Update(1);
        Engine::LOG(Engine::INFO) << curStage << "stage MechEnemy summoned";
    }
}