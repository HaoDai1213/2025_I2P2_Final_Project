//
// Created by Hsuan on 2024/4/10.
//

#ifndef INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
#define INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class Image;
}

class StartScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgm;
public:
    Engine::Label *UILogin;
    Engine::Label *UILogout;
    Engine::Label *UIReg;
    static bool logged;
    static std::string username;
    explicit StartScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int num);
    void RegisterOnClick(int num);
    void LoginOnClick(int num);
    void LogoutOnClick(int num);
    void SettingsOnClick(int num);
};
#endif   // INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
