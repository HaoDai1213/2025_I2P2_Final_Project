#include <functional>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <cstdio>

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
char resultScoreBuf[30];
char resultAccBuf[30];

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
    Acc = PlayScene::accuracy * 100;
    std::sprintf(resultScoreBuf, "%08d", Score);
    std::sprintf(resultAccBuf, "%.2f%%", Acc);

    keyStrokes.clear();

    AddNewObject(new Engine::Label("RESULT", "pirulen.ttf", 60, halfW, halfH / 4 - 10, 127, 255, 212, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label("Score", "pirulen.ttf", 48, halfW - 600, halfH / 2, 255, 255, 255, 255, 0, 0.5));
    AddNewObject(new Engine::Label(resultScoreBuf, "pirulen.ttf", 48, halfW - 20, halfH / 2, 224, 255, 255, 255, 1, 0.5));
    AddNewObject(new Engine::Label("Perfect", "pirulen.ttf", 40, halfW - 600, halfH / 2  + 90, 255, 255, 255, 255, 0, 0.5));
    AddNewObject(new Engine::Label(std::to_string(PlayScene::PFcount) + "x", "pirulen.ttf", 40, halfW - 20, halfH / 2  + 90, 255, 215, 0, 255, 1, 0.5));
    AddNewObject(new Engine::Label("Great", "pirulen.ttf", 40, halfW - 600, halfH / 2 + 180, 255, 255, 255, 255, 0, 0.5));
    AddNewObject(new Engine::Label(std::to_string(PlayScene::GRcount) + "x", "pirulen.ttf", 40, halfW - 20, halfH / 2  + 180, 192, 192, 192, 255, 1, 0.5));
    AddNewObject(new Engine::Label("Miss", "pirulen.ttf", 40, halfW - 600, halfH / 2 + 270, 255, 255, 255, 255, 0, 0.5));
    AddNewObject(new Engine::Label(std::to_string(PlayScene::MScount) + "x", "pirulen.ttf", 40, halfW - 20, halfH / 2  + 270, 220, 20, 60, 255, 1, 0.5));
    AddNewObject(new Engine::Label("Accuracy", "pirulen.ttf", 40, halfW - 600, halfH / 2 + 360, 255, 255, 255, 255, 0, 0.5));
    AddNewObject(new Engine::Label(resultAccBuf, "pirulen.ttf", 40, halfW - 20, halfH / 2  + 360, 152, 251, 152, 255, 1, 0.5));
    
    if (Acc == 100) {
        AddNewObject(new Engine::Label("SS", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 215, 219, 221, 255, 0.5, 0.5));
    }
    else if (Acc >= 95) {
        AddNewObject(new Engine::Label("S", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 247, 220, 111, 255, 0.5, 0.5));
    }
    else if (Acc >= 90) {
        AddNewObject(new Engine::Label("A", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 130, 224, 170, 255, 0.5, 0.5));
    }
    else if (Acc >= 80) {
        AddNewObject(new Engine::Label("B", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 133, 193, 233, 255, 0.5, 0.5));
    }
    else if (Acc >= 70) {
        AddNewObject(new Engine::Label("C", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 195, 155, 211, 255, 0.5, 0.5));
    }
    else {
        AddNewObject(new Engine::Label("D", "pirulen.ttf", 180, w * 3 / 4, halfH * 3 / 4 + 75, 230, 176, 170, 255, 0.5, 0.5));
    }
    // nametag
    // AddNewObject(new Engine::Image("play/sand.png", halfW, h * 3 / 4 - 30, 400, 100, 0.5, 0.5));
    // nameLabel = new Engine::Label(keyStrokes, "pirulen.ttf", 36, halfW - 180, h * 3 / 4 - 30, 0, 0, 0, 255, 0, 0.5);
    // AddNewObject(nameLabel);

    bgmId = AudioHelper::PlayBGM("applause.wav");
}

void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}

void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    // ?
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
    }
}

void WinScene::OnKeyDown(int keyCode) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    IScene::OnKeyDown(keyCode);
    /*
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
    */
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
    if (keyStrokes.empty()) keyStrokes = "GUEST";
    out << playDate << " " << playTime << " " << keyStrokes << " " << Score << std::endl;
    out.flush();
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
