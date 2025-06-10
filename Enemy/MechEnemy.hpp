#ifndef MECHENEMY_HPP
#define MECHENEMY_HPP
#include "Enemy.hpp"

class MechEnemy : public Enemy {
private:
    int curStage;
public:
    MechEnemy(int x, int y, int stage);
    void OnExplode() override;
};
#endif   // MECHENEMY_HPP
