#include <functional>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/LOG.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

int entering, count;
time_t now;
tm *localnow;
std::string tmyr, tmmo, tmdy, tmhr, tmmn, tmsc;

void WinScene::Initialize() {
    std::ifstream in;
    entering = 1;
    ticks = 0;
    count = 0;
    playDate.clear();
    playTime.clear();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Score = PlayScene::gamescore;
    keyStrokes.clear();

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH - 100, 300, 300, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label("Score: ", "pirulen.ttf", 36, halfW - 100, h * 3 / 4 - 130, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label(std::to_string(Score), "pirulen.ttf", 36, halfW + 100, h * 3 / 4 - 130, 255, 255, 255, 255, 0.5, 0.5));

    AddNewObject(new Engine::Image("play/sand.png", halfW, h * 3 / 4 - 30, 400, 100, 0.5, 0.5));
    nameLabel = new Engine::Label(keyStrokes, "pirulen.ttf", 36, halfW - 180, h * 3 / 4 - 30, 0, 0, 0, 255, 0, 0.5);
    AddNewObject(nameLabel);

    bgmId = AudioHelper::PlayAudio("win.wav");
}

void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}

void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}

void WinScene::OnKeyDown(int keyCode) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    IScene::OnKeyDown(keyCode);
    if (entering) {
        if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z && keyStrokes.size() < 10) {
            keyStrokes.push_back(keyCode - 1 + 'A');
            count++;
            nameLabel->Text = keyStrokes;
            Engine::LOG(Engine::INFO) << keyCode << " is clicked";
        }
        else if (keyCode == ALLEGRO_KEY_BACKSPACE && !keyStrokes.empty()) {
            keyStrokes.pop_back();
            count--;
            nameLabel->Text = keyStrokes;
        }
    }
}

void WinScene::BackOnClick(int stage) {
    // Get current time
    now = time(0);
    localnow = localtime(&now);
    tmyr = std::to_string(1900 + localnow->tm_year);
    tmmo = std::to_string(1 + localnow->tm_mon);
    tmdy = std::to_string(localnow->tm_mday);
    tmhr = std::to_string(localnow->tm_hour);
    if (tmhr.size() == 1) tmhr = "0" + tmhr;
    tmmn = std::to_string(localnow->tm_min);
    if (tmmn.size() == 1) tmmn = "0" + tmmn;
    tmsc = std::to_string(localnow->tm_sec);
    if (tmsc.size() == 1) tmsc = "0" + tmsc;
    playDate = tmyr + '/' + tmmo + '/' + tmdy;
    playTime = tmhr + ':' + tmmn + ':' + tmsc;
    // Output to the file
    std::ofstream out("C:/Users/barry/Desktop/2025_I2P2_TowerDefense-main/Resource/scoreboard.txt", std::ios::app);
    if (keyStrokes.empty()) keyStrokes = "NULL";
    out << playDate << " " << playTime << " " << keyStrokes << " " << Score << std::endl;
    out.flush();
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
