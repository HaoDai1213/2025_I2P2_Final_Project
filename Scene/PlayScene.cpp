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
#include <cstdio>

#include "Bullet/Bullet.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Note/Note.hpp"
#include "Note/RedNote.hpp"
#include "Note/BlueNote.hpp"
#include "Player/Player.hpp"
#include "PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/MissEffect.hpp"
#include "UI/Animation/GreatEffect.hpp"
#include "UI/Animation/PerfectEffect.hpp"
#include "UI/Component/Label.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 25, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(3, 7);
const int sliderSize = 1500;
const int noteSize = 110;
const int offset = -60; // ?
std::map<char, bool> keyState = {{'W', false}, {'A', false}, {'S', false}, {'D', false}};

Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

int PlayScene::PFcount;
int PlayScene::GRcount;
int PlayScene::MScount;
int PlayScene::gamescore;   // static
float PlayScene::accuracy;
int rep;
char scoreBuf[30];
char accBuf[30];

void PlayScene::Initialize() {
    keyState.clear();
    gamescore = 0;
    accuracy = 0;
    noteCount = 0;
    hitCount = 0;
    combo = 0;
    ticks = 0;
    endTick = 0;
    lives = 0;
    SpeedMult = 1;
    isRedHit = false;
    isBlueHit = false;
    PFcount = 0;
    GRcount = 0;
    MScount = 0;
    // Add groups from bottom to top.
    AddNewControlObject(UIGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    AddNewObject(NoteGroup = new Group());

    ReadBullet();
    ReadNote();
    lastBullet = std::get<0> (bulletData.back()) + 1000 * (1600 / std::get<2> (bulletData.back()));
    lastNote = std::get<0> (noteData.back());
    endTick = (lastBullet > lastNote) ? lastBullet + 3000 : lastNote + 3000;
    Engine::LOG(Engine::INFO) << "LB, LN: " << lastBullet << ", " << lastNote << ", ET: " << endTick ;

    // Things related to player 
    player = new Player("play/player.png", SpawnGridPoint.x * BlockSize, SpawnGridPoint.y * BlockSize);
    playerSpeed = player->getSpeed();
    lives = player->getHp();

    ConstructUI();
    // Preload Lose Scene
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    bgmId = AudioHelper::PlayBGM("song" + std::to_string(MapId) + ".mp3");
}
void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (SpeedMult == 0) {

    }
    for (int i = 0; i < SpeedMult; i++) {
        IScene::Update(deltaTime);
        // Check if we should create new enemy.
        ticks += deltaTime;

        // bullet
        auto cur_bullet = bulletData.front();
        int cur_bt = std::get<0> (cur_bullet);
        rep = 0;
        if (ticks * 1000 >= cur_bt) {    // ticks counts in second and bullet.txt in millisecond so *1000 here
            for (auto &b : bulletData) {    // only when the first bullet matches the timing then we do the search
                auto [bTiming, bPos, bSpeed] = b;
                if (bTiming <= ticks * 1000) {
                    BulletGroup->AddNewObject(new FireBullet(Engine::Point(24 * BlockSize, bPos), Engine::Point(-1, 0), 0.0, bSpeed));
                    rep++;
                }
                else break;
            }

            for (i = rep; i > 0; i--) {
                bulletData.pop_front();
            }
        }
        if (player->alive == false)
            Engine::GameEngine::GetInstance().ChangeScene("lose");

        // note
        if (!noteData.empty()) {
            auto cur_note = noteData.front();
            auto [nTiming, nLine, nType, nMult] = cur_note;
            int aprTiming = nTiming - duration * 1000 / nMult;
            if (ticks * 1000 >= aprTiming) {
                Engine::LOG(Engine::INFO) << "current timing, appear timing: " << ticks * 1000 << ", " << aprTiming;
                noteData.pop_front();
                // adjust the delay of note creation
                float aprDiff = (ticks * 1000 - aprTiming) * ((sliderSize - noteSize) / duration) * nMult / 1000;
                switch (nType) {
                    case 1:
                        NoteGroup->AddNewObject(new RedNote(aprTiming, nTiming, nLine, (sliderSize - noteSize) / duration, nMult, \
                                                            Engine::Point(24 * BlockSize - aprDiff, 200 + nLine * 450 + 75), Engine::Point(-1, 0)));
                        break;
                    case 2:
                        NoteGroup->AddNewObject(new BlueNote(aprTiming * nMult, nTiming, nLine, (sliderSize - noteSize) / duration, nMult, \
                                                            Engine::Point(24 * BlockSize - aprDiff, 200 + nLine * 450 + 75), Engine::Point(-1, 0)));
                        break;
                    default:
                        continue;
                }
            }
        }

        if (isRedHit) {
            HitObject(ticks * 1000 + offset, 1);
            AudioHelper::PlayAudio("note1.wav");
            isRedHit = false;
            std::sprintf(scoreBuf, "Score %08d", gamescore);
            UIScore->Text = scoreBuf;
        }
        if (isBlueHit) {
            HitObject(ticks * 1000 + offset, 2);
            AudioHelper::PlayAudio("note2.wav");
            isBlueHit = false;
            std::sprintf(scoreBuf, "Score %08d", gamescore);
            UIScore->Text = scoreBuf;
        }
        if (noteCount != 0) accuracy = hitCount / noteCount;
        std::sprintf(accBuf, "Accuracy %.2f%%", accuracy * 100);
        UIAcc->Text = accBuf;

        UICombo->Text = std::string("combo ") + std::to_string(combo);
        
        if (ticks * 1000 > endTick) {
            Engine::GameEngine::GetInstance().ChangeScene("win");
        }
    }

    // player
    if (keyState['W'] && player->Position.y - playerSpeed >= 350 + 20) {
        player->Position.y -= playerSpeed;
    }
    if (keyState['A'] && player->Position.x - playerSpeed >= 20) {
        player->Position.x -= playerSpeed;
    }
    if (keyState['S'] && player->Position.y + playerSpeed <= 650 - 20) {
        player->Position.y += playerSpeed;
    }
    if (keyState['D'] && player->Position.x + playerSpeed <= 25 * BlockSize - 20) {
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
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
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
    // Engine::LOG(Engine::INFO) << keyCode << " is down";
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
    else if (keyCode == ALLEGRO_KEY_G || keyCode == ALLEGRO_KEY_H) {
        isRedHit = true;
    }
    else if (keyCode == ALLEGRO_KEY_F || keyCode == ALLEGRO_KEY_J) {
        isBlueHit = true;
    }
}

void PlayScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);
    // Engine::LOG(Engine::INFO) << keyCode << " is up";
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

void PlayScene::ReadBullet() {
    std::string filename = std::string("Resource/bullet") + std::to_string(MapId) + ".txt";
    float timing, pos, speed;
    bulletData.clear();
    std::ifstream fin(filename);
    while (fin >> timing >> pos >> speed) {
        if (pos > 300) pos = 300;
        bulletData.push_back({timing, pos + 350, speed});
        // Engine::LOG(Engine::INFO) << "Bullet loaded now: " << timing << ", " << pos << ", " << speed;
    }
    fin.close();
}

void PlayScene::ReadNote() {
    std::string filename = std::string("Resource/note") + std::to_string(MapId) + ".txt";
    float timing, line, type, mult;
    noteData.clear();
    std::ifstream fin(filename);
    fin >> MapBPM;
    duration = 60 / (MapBPM / 4);
    while (fin >> timing >> line >> type >> mult) {
        noteData.push_back({timing, line, type, mult});
        // Engine::LOG(Engine::INFO) << "Note loaded now: " << timing << ", " << type << ", " << mult;
    }
}

void PlayScene::ConstructUI() {
    // Text
    UIGroup->AddNewObject(UIScore = new Engine::Label(std::string("Score 00000000"), "pirulen.ttf", 32, 12, 12, 255, 255, 255));
    UIGroup->AddNewObject(UIAcc = new Engine::Label(std::string("Accuracy ") + std::to_string(accuracy * 100), "pirulen.ttf", 24, 12, 56, 255, 255, 255));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 12, 100, 255, 255, 255));
    UIGroup->AddNewObject(UICombo = new Engine::Label(std::string("combo ") + std::to_string(combo), "pirulen.ttf", 32, 800 - 100, 200 - 20, 255, 255, 255, 255, 0, 1));
    // Image
    UIGroup->AddNewObject(new Engine::Image("Play/bar.png", 0, 200, 1600, 150));
    UIGroup->AddNewObject(new Engine::Image("Play/bar.png", 0, 650, 1600, 150));
    UIGroup->AddNewObject(new Engine::Image("Play/judge_area.png", 110, 200 + 75, 120, 120, 0.5, 0.5));
    UIGroup->AddNewObject(new Engine::Image("Play/judge_area.png", 110, 650 + 75, 120, 120, 0.5, 0.5));
}

void PlayScene::HitObject(int curTiming, int type) {
    Note *FirstNote = nullptr;
    int hitDiff = INT_MAX;

    if (NoteGroup->GetObjects().size()) {
        auto obj = NoteGroup->GetObjects().front();
        FirstNote = dynamic_cast<Note*>(obj);
        hitDiff = std::abs(FirstNote->getHitTiming() - curTiming);
    }

    if (FirstNote) {
        if (hitDiff <= Note::judgement_ms) {
            if (type == FirstNote->getType()) {
                if (hitDiff <= Note::judgement_pf) {
                    noteCount++;
                    hitCount++;
                    combo++;
                    PFcount++;
                    EffectGroup->AddNewObject(new PerfectEffect(110, 200 + 75));
                    EffectGroup->AddNewObject(new PerfectEffect(110, 650 + 75));
                    // Engine::LOG(Engine::INFO) << "perfect! " << curTiming << ", " << FirstNote->getHitTiming();
                    NoteGroup->RemoveObject(FirstNote->GetObjectIterator());
                    gamescore += 300 + 300 * (combo / 10);
                }
                else if (hitDiff <= Note::judgement_gr) {
                    noteCount++;
                    hitCount += 0.5;
                    combo++;
                    GRcount++;
                    EffectGroup->AddNewObject(new GreatEffect(110, 200 + 75));
                    EffectGroup->AddNewObject(new GreatEffect(110, 650 + 75));
                    // Engine::LOG(Engine::INFO) << "great! " << curTiming << ", " << FirstNote->getHitTiming();
                    NoteGroup->RemoveObject(FirstNote->GetObjectIterator());
                    gamescore += 150 + 150 * (combo / 10);
                }
                else {
                    noteCount++;
                    combo = 0;
                    MScount++;
                    EffectGroup->AddNewObject(new MissEffect(110, 200 + 75));
                    EffectGroup->AddNewObject(new MissEffect(110, 650 + 75));
                    // Engine::LOG(Engine::INFO) << "miss (late)! " << curTiming << ", " << FirstNote->getHitTiming();
                    NoteGroup->RemoveObject(FirstNote->GetObjectIterator());
                    AudioHelper::PlayAudio("miss.mp3");
                }
            }
            else {
                noteCount++;
                combo = 0;
                MScount++;
                EffectGroup->AddNewObject(new MissEffect(110, 200 + 75));
                EffectGroup->AddNewObject(new MissEffect(110, 650 + 75));
                // Engine::LOG(Engine::INFO) << "miss! " << curTiming << ", " << FirstNote->getHitTiming();
                NoteGroup->RemoveObject(FirstNote->GetObjectIterator());
                AudioHelper::PlayAudio("miss.mp3");
            }
        }
    }
}