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
    curId = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId;

    // bg
    AddNewObject(new Engine::Image("stage-select/bg" + std::to_string(curId) + ".png", 800, 450, 1600, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/half-black.png", 800, 450, 1800, 900, 0.5, 0.5));

    AddNewObject(new Engine::Label("Failed... :(", "pirulen.ttf", 48, halfW, halfH / 4 + 100, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 200, halfH - 15, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&LoseScene::RetryOnClick, this, curId));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Retry", "pirulen.ttf", 48, halfW, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/button.png", "stage-select/button-select.png", halfW - 200, halfH * 3 / 2 - 65, 400, 400 / 3);
    btn->SetOnClickCallback(std::bind(&LoseScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 6 / 4, 255, 255, 255, 255, 0.5, 0.5));

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