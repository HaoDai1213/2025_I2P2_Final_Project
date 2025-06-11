#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <map>
#include <tuple>

#include "Bullet/Bullet.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Player/Player.hpp"
#include "PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Component/Label.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 25, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(3, 7);
std::map<char, bool> keyState = {{'W', false}, {'A', false}, {'S', false}, {'D', false}, {'R', false}, {'B', false}};

Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

int PlayScene::gamescore;   // static
int rep;

void PlayScene::Initialize() {
    keyState.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 0;
    money = 150;
    SpeedMult = 1;
    count = 0;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    ReadBullet();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    UIGroup->AddNewObject(imgTarget);

    // Things related to player 
    player = new Player("play/player.png", SpawnGridPoint.x * BlockSize, SpawnGridPoint.y * BlockSize);
    playerSpeed = player->getSpeed();
    lives = player->getHp();

    ConstructUI();
    // Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    bgmId = AudioHelper::PlayBGM("song" + std::to_string(MapId) + ".mp3");
}
void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    for (int i = 0; i < SpeedMult; i++) {
        IScene::Update(deltaTime);
        // Check if we should create new enemy.
        ticks += deltaTime;
        // auto current = enemyWaveData.front();
        // if (ticks < current.second)
        //     continue;
        // ticks -= current.second;
        // enemyWaveData.pop_front();
        // const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
        // Enemy *enemy;
        // switch (current.first) {
        //     case 1:
        //         EnemyGroup->AddNewObject(enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
        //         break;
        //     default:
        //         continue;
        // }
        // // Compensate the time lost.
        // enemy->Update(ticks);
        auto current = bulletData.front();
        int cur_t = std::get<0> (current);
        rep = 0;
        if (ticks * 1000 >= cur_t) {    // ticks counts in S and bullet.txt in MS
            for (auto &b : bulletData) {    // only when the first bullet matches the timing then we do the search
                auto [t, p, s] = b;
                if (t <= ticks * 1000) {
                    BulletGroup->AddNewObject(new FireBullet(Engine::Point(24 * BlockSize, p), Engine::Point(-1, 0), 0.0, s));
                    rep++;
                }
            }

            for (i = rep; i > 0; i--) {
                bulletData.pop_front();
            }
        }
        lives = player->getHp();
        UILives->Text = std::string("Life ") + std::to_string(lives);
        if (player->alive == false)
            Engine::GameEngine::GetInstance().ChangeScene("lose");
    }

    // player
    // the boundary would be changed after the slider of rg part is done
    if (keyState['W'] && player->Position.y - playerSpeed >= 20) {
        player->Position.y -= playerSpeed;
    }
    if (keyState['A'] && player->Position.x - playerSpeed >= 20) {
        player->Position.x -= playerSpeed;
    }
    if (keyState['S'] && player->Position.y + playerSpeed <= 13 * BlockSize - 20) {
        player->Position.y += playerSpeed;
    }
    if (keyState['D'] && player->Position.x + playerSpeed <= 20 * BlockSize - 20) {
        player->Position.x += playerSpeed;
    }
}
void PlayScene::Draw() const {
    IScene::Draw();
    player->Draw();
    if (DebugMode) {

    }
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (!imgTarget->Visible)
        return;
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (button & 1) {

    }
}
void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    /*
    Keybinds:
      W               O
    A S D (F G H J) K L ;
    */
    Engine::LOG(Engine::INFO) << keyCode << " is down";
    if (keyCode == ALLEGRO_KEY_W || keyCode == ALLEGRO_KEY_O) { 
        keyState['W'] = true;
    }   
    else if (keyCode == ALLEGRO_KEY_A || keyCode == ALLEGRO_KEY_K) {
        keyState['A'] = true;
    }
    else if (keyCode == ALLEGRO_KEY_S || keyCode == ALLEGRO_KEY_L) {
        keyState['S'] = true;
    }
    else if (keyCode == ALLEGRO_KEY_D || keyCode == ALLEGRO_KEY_SEMICOLON) {
        keyState['D'] = true;
    }
}

void PlayScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);
    Engine::LOG(Engine::INFO) << keyCode << " is up";
    if (keyCode == ALLEGRO_KEY_W || keyCode == ALLEGRO_KEY_O) { 
        keyState['W'] = false;
    }   
    else if (keyCode == ALLEGRO_KEY_A || keyCode == ALLEGRO_KEY_K) {
        keyState['A'] = false;
    }
    else if (keyCode == ALLEGRO_KEY_S || keyCode == ALLEGRO_KEY_L) {
        keyState['S'] = false;
    }
    else if (keyCode == ALLEGRO_KEY_D || keyCode == ALLEGRO_KEY_SEMICOLON) {\
        keyState['D'] = false;
    }
}

void PlayScene::Hit() {
    lives--;
    UILives->Text = std::string("Life ") + std::to_string(lives);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
}

void PlayScene::ReadBullet() {
    std::string filename = std::string("Resource/bullet") + std::to_string(MapId) + ".txt";
    float timing, pos, speed;
    bulletData.clear();
    std::ifstream fin(filename);
    while (fin >> timing >> pos >> speed) {
        bulletData.push_back({timing, pos, speed});
        Engine::LOG(Engine::INFO) << "Bullet loaded now: " << timing << ", " << pos << ", " << speed;
    }
    fin.close();
}

void PlayScene::ConstructUI() {
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88, 255, 255, 255));

}

