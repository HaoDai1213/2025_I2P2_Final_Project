#ifndef LOGINSCENE_HPP
#define LOGINSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include <map>

#include "Engine/IScene.hpp"

class LoginScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgm;
    int nowEntering;
    int usernameLen;
    int passwordLen;
    Engine::Label *stateText;
    Engine::Label *usernameLabel;
    Engine::Label *passwordLabel;

public:
    std::string usernameStrokes;
    std::string passwordStrokes;
    std::string passwordDisplay;
    std::map<std::string, std::string> accountData;
    explicit LoginScene() = default;
    void Initialize() override;
    void Terminate() override;
    void OnKeyDown(int keyCode) override;
    void ReadAccounts();
    void TextOnClick(int num);
    void BackOnClick(int num);
    void LoginOnClick(int num);
};

#endif   // LOGINSCENE_HPP