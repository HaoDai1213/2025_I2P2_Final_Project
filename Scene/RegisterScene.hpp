#ifndef REGISTERSCENE_HPP
#define REGISTERSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include <map>

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class Image;
}

class RegisterScene final : public Engine::IScene {
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
    std::map<std::string, std::string> accountData;
    explicit RegisterScene() = default;
    void Initialize() override;
    void Terminate() override;
    void OnKeyDown(int keyCode) override;
    void ReadAccounts();
    void CreateAccount(std::string username, std::string password);
    void TextOnClick(int num);
    void BackOnClick(int num);
    void RegisterOnClick(int num);
};

#endif   // REGISTERSCENE_HPP