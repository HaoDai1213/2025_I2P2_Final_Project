//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

Engine::ImageButton *reg;
Engine::ImageButton *login;
Engine::ImageButton *logout;
bool StartScene::logged = false;
std::string StartScene::username;

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Image("stage-select/startbg.png", halfW, halfH, w, h, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", halfW, halfH, w, h, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/gradient.png", halfW, halfH, w, h, 0.5, 0.5));
    AddNewObject(new Engine::Label("Final Project", "pirulen.ttf", 120, halfW, halfH / 3 + 50, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 200, halfH / 2 + 185, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 200, halfH * 3 / 2 - 65, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 40, halfW, halfH * 3 / 2, 255, 255, 255, 255, 0.5, 0.5));

    if (!logged) {
        username.clear();
        reg = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", 30, h - 120, 300, 100);
        reg->SetOnClickCallback(std::bind(&StartScene::RegisterOnClick, this, 2));
        AddNewControlObject(reg);
        AddNewObject(UIReg = new Engine::Label("Register", "pirulen.ttf", 32, 180, h - 67.5, 255, 255, 255, 255, 0.5, 0.5));

        login = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", w - 330, h - 120, 300, 100);
        login->SetOnClickCallback(std::bind(&StartScene::LoginOnClick, this, 2));
        AddNewControlObject(login);
        AddNewObject(UILogin = new Engine::Label("Login", "pirulen.ttf", 36, w - 180, h - 67.5, 255, 255, 255, 255, 0.5, 0.5));
    }

    else if (logged) {
        // logout, current username
        logout = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", w - 330, h - 120, 300, 100);
        logout->SetOnClickCallback(std::bind(&StartScene::LogoutOnClick, this, 2));
        AddNewControlObject(logout);
        AddNewObject(UILogout = new Engine::Label("Logout", "pirulen.ttf", 36, w - 180, h - 67.5, 255, 255, 255, 255, 0.5, 0.5));

        AddNewObject(new Engine::Label("current username:", "pirulen.ttf", 24, 30,  h - (30 + 40), 255, 255, 255, 255, 0, 1));
        AddNewObject(new Engine::Label(username, "pirulen.ttf", 32, 30, h - 30, 255, 255, 255, 255, 0, 1));        
    }

    bgm = AudioHelper::PlayBGM("default.mp3");
}
void StartScene::Terminate() {
    AudioHelper::StopBGM(bgm);
    IScene::Terminate();
}
void StartScene::PlayOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void StartScene::RegisterOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("register");
}

void StartScene::LoginOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("login");
}

void StartScene::LogoutOnClick(int num) {
    logged = false;
    // clear the user data
    username.clear();
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void StartScene::SettingsOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}