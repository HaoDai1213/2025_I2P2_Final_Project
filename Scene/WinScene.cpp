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
#include "StartScene.h"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

int entering, count;
char resultScoreBuf[30];
char resultAccBuf[30];

void WinScene::Initialize() {
    std::ifstream in;
    entering = 1;
    ticks = 0;
    count = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Score = PlayScene::gamescore;
    Acc = PlayScene::accuracy * 100;
    std::sprintf(resultScoreBuf, "%08d", Score);
    std::sprintf(resultAccBuf, "%.2f%%", Acc);
    curId = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId;

    // bg
    AddNewObject(new Engine::Image("stage-select/bg" + std::to_string(curId) + ".png", 800, 450, 1600, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));

    AddNewObject(new Engine::Label("RESULT", "pirulen.ttf", 60, halfW, halfH / 4 - 10, 127, 255, 212, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 200, halfH * 7 / 4 - 65, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 255, 255, 255, 255, 0.5, 0.5));

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
    // Output to the file
    std::ofstream out("../Resource/scoreboard" \
                        + std::to_string(curId) + ".txt", std::ios::app);
    std::string name = "GUEST";
    if (StartScene::logged) name = StartScene::username;
    out << name << " " << Score << std::endl;
    out.flush();
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
