#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void LoseScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Label("Failed... :(", "pirulen.ttf", 48, halfW, halfH / 4 + 100, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH, 400, 100);
    btn->SetOnClickCallback(std::bind(&LoseScene::RetryOnClick, this, dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Retry", "pirulen.ttf", 48, halfW, halfH + 50, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 6 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&LoseScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 6 / 4, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("lose.mp3", true, AudioHelper::BGMVolume);
}
void LoseScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void LoseScene::BackOnClick(int num) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void LoseScene::RetryOnClick(int num) {
    PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = num;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}