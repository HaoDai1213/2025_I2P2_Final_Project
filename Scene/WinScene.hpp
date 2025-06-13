#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <string>

class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    Engine::Label *nameLabel;
    int Score;
    float Acc;
    std::string playDate;
    std::string playTime;

public:
    std::string keyStrokes;
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    void BackOnClick(int stage);
};

#endif   // WINSCENE_HPP
