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

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    Engine::ImageButton *login;
    Engine::ImageButton *reg;

    AddNewObject(new Engine::Label("Final Project", "pirulen.ttf", 120, halfW, halfH / 3 + 50, 136, 136, 204, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    reg = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 330, h - 105, 300, 75);
    reg->SetOnClickCallback(std::bind(&StartScene::RegisterOnClick, this, 2));
    AddNewControlObject(reg);
    AddNewObject(new Engine::Label("Register", "pirulen.ttf", 36, w - 180, h - 67.5, 0, 0, 0, 255, 0.5, 0.5));

    login = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 30, h - 105, 300, 75);
    login->SetOnClickCallback(std::bind(&StartScene::LoginOnClick, this, 2));
    AddNewControlObject(login);
    AddNewObject(new Engine::Label("Login", "pirulen.ttf", 36, 180, h - 67.5, 0, 0, 0, 255, 0.5, 0.5));
}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void StartScene::RegisterOnClick(int num) {

}

void StartScene::LoginOnClick(int num) {

}

void StartScene::LogoutOnClick(int num) {

}

void StartScene::SettingsOnClick(int num) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}