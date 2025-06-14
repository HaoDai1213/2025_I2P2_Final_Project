#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "StartScene.h"
#include "PlayScene.hpp"
#include "RegisterScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void RegisterScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    nowEntering = 0;
    usernameLen = 0;
    passwordLen = 0;
    usernameStrokes.clear();
    passwordStrokes.clear();
    accountData.clear();
    ReadAccounts();
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Image("stage-select/startbg.png", halfW, halfH, w, h, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", halfW, halfH, w, h, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/gradient.png", halfW, halfH, w, h, 0.5, 0.5));

    AddNewObject(new Engine::Label("Register", "pirulen.ttf", 80, halfW, halfH / 3, 255, 255, 255, 255, 0.5, 0.5));
    
    AddNewObject(stateText = new Engine::Label("", "pirulen.ttf", 48, 800, 250, 240, 128, 128, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sand.png", "stage-select/sand.png", halfW - 200, halfH / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&RegisterScene::TextOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Username", "pirulen.ttf", 48, halfW - 250, halfH / 2 + 150, 255, 255, 255, 255, 1, 0.5));
    AddNewObject(usernameLabel = new Engine::Label(usernameStrokes, "pirulen.ttf", 36, halfW - 190, halfH / 2 + 150, 0, 0, 0, 255, 0, 0.5));

    btn = new Engine::ImageButton("stage-select/sand.png", "stage-select/sand.png", halfW - 200, halfH * 3 / 2 - 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&RegisterScene::TextOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Password", "pirulen.ttf", 48, halfW - 250, halfH * 3 / 2 - 100, 255, 255, 255, 255, 1, 0.5));
    AddNewObject(passwordLabel = new Engine::Label(passwordStrokes, "pirulen.ttf", 36, halfW - 190, halfH * 3 / 2 - 100, 0, 0, 0, 255, 0, 0.5));

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 450, h - 165, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&RegisterScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW - 250, h - 100, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW + 50, h - 165, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&RegisterScene::RegisterOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Register", "pirulen.ttf", 48, halfW + 250, h - 100, 255, 255, 255, 255, 0.5, 0.5));

    bgm = AudioHelper::PlayBGM("default.mp3");
}

void RegisterScene::Terminate() {
    AudioHelper::StopBGM(bgm);
    IScene::Terminate();
}

void RegisterScene::ReadAccounts() {
    std::string filename = std::string("../Resource/accounts.txt");
    std::string name, passwd;
    accountData.clear();
    std::ifstream fin(filename);
    while (fin >> name >> passwd) {
        accountData.insert(std::pair<std::string, std::string> (name, passwd));
    }
    fin.close();
}

void RegisterScene::CreateAccount(std::string username, std::string password) {
    std::ofstream fout("../Resource/accounts.txt", std::ios::app);
    fout << username << " " << password << std::endl;
    fout.flush();
}

void RegisterScene::TextOnClick(int num) {
    if (num == 1) nowEntering = 1;
    else if (num == 2) nowEntering = 2;
}

void RegisterScene::BackOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void RegisterScene::RegisterOnClick(int num) {
    if (accountData.count(usernameStrokes) == 0 && usernameLen > 0) {
        if (passwordLen > 0) {
            CreateAccount(usernameStrokes, passwordStrokes);
            Engine::GameEngine::GetInstance().ChangeScene("start");
        }
        else {
            usernameStrokes.clear();
            passwordStrokes.clear();
            usernameLabel->Text = usernameStrokes;
            passwordLabel->Text = passwordStrokes;
            stateText->Text = "Invalid Password";
        }
    }
    else if (usernameLen == 0) {
        usernameStrokes.clear();
        passwordStrokes.clear();
        usernameLabel->Text = usernameStrokes;
        passwordLabel->Text = passwordStrokes;
        stateText->Text = "Invalid Username";
    }
    else {
        usernameStrokes.clear();
        passwordStrokes.clear();
        usernameLabel->Text = usernameStrokes;
        passwordLabel->Text = passwordStrokes;
        stateText->Text = "username has been used";
    }
}

void RegisterScene::OnKeyDown(int keyCode) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    IScene::OnKeyDown(keyCode);

    if (nowEntering == 1) {
        if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z && usernameStrokes.size() < 8) {
            usernameStrokes.push_back(keyCode - 1 + 'a');
            usernameLen++;
            usernameLabel->Text = usernameStrokes;
            Engine::LOG(Engine::INFO) << keyCode << " is clicked";
        }
        else if (keyCode == ALLEGRO_KEY_BACKSPACE && !usernameStrokes.empty()) {
            usernameStrokes.pop_back();
            usernameLen--;
            usernameLabel->Text = usernameStrokes;
        }
    }

    else if (nowEntering == 2) {
        if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z && passwordStrokes.size() < 8) {
            passwordStrokes.push_back(keyCode - 1 + 'a');
            passwordLen++;
            passwordLabel->Text = passwordStrokes;
            Engine::LOG(Engine::INFO) << keyCode << " is clicked";
        }
        else if (keyCode == ALLEGRO_KEY_BACKSPACE && !passwordStrokes.empty()) {
            passwordStrokes.pop_back();
            passwordLen--;
            passwordLabel->Text = passwordStrokes;
        }
    }
}