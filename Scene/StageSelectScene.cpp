#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <cstdio>
#include <algorithm>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

int curStage;
char sbBuf[30];

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    if (!curStage) curStage = 1;
    songData.clear();
    scoreboardData.clear();
    sbDataCount = 0;

    ReadSongData();
    ReadScoreboard(curStage);
    maxStage = songData.size();

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 675, halfH * 3 / 2 + 50, 300, 75);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PrevOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 36, halfW - 525, halfH * 3 / 2 + 87.5, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 375, halfH * 3 / 2 + 50, 300, 75);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::NextOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 36, halfW + 525, halfH * 3 / 2 + 87.5, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 25, halfH * 3 / 2 + 50, 300, 75);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, curStage));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 36, halfW + 175, halfH * 3 / 2 + 87.5, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 325, halfH * 3 / 2 + 50, 300, 75);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 36, halfW - 175, halfH * 3 / 2 + 87.5, 0, 0, 0, 255, 0.5, 0.5));

    // song info
    AddNewObject(new Engine::Label(songData[curStage - 1].first + " - " + songData[curStage - 1].second, "pirulen.ttf", 48, 800, 100, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/bg" + std::to_string(curStage) + ".png", 800, 200, 700, 393.75, 0.5, 0));

    // scoreboard
    AddNewObject(new Engine::Label("Leaderboard", "pirulen.ttf", 28, 1200, 200, 255, 255, 255, 255, 0, 0));
    DrawScoreboard();

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("song" + std::to_string(curStage) + ".mp3", true, AudioHelper::BGMVolume);
}
void StageSelectScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StageSelectScene::BackOnClick(int num) {
    curStage = 1;
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void StageSelectScene::PlayOnClick(int num) {
    PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = num;
    curStage = num;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void StageSelectScene::PrevOnClick(int num) {
    if (curStage != 1) {
        curStage--;
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}
void StageSelectScene::NextOnClick(int num) {
    if (curStage != maxStage) {
        curStage++;
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}

void StageSelectScene::ReadSongData() {
    std::string filename = std::string("Resource/songlist.txt");
    std::string name, author;
    int pos;
    songData.clear();
    std::ifstream fin(filename);
    while (fin >> name >> author) {
        std::replace(name.begin(), name.end(), '_', ' ');
        std::replace(author.begin(), author.end(), '_', ' ');
        songData.push_back({name, author});
    }
    fin.close();
}

void StageSelectScene::ReadScoreboard(int num) {
    std::string filename = std::string("../Resource/scoreboard" + std::to_string(num) + ".txt");
    std::string name;
    int score;
    scoreboardData.clear();
    std::ifstream fin(filename);
    while (fin >> name >> score) {
        scoreboardData.insert({name, score});
    }
}

void StageSelectScene::DrawScoreboard() {
    for (auto data : scoreboardData) {
        if (sbDataCount < 3) {
            AddNewObject(new Engine::Label(data.first, "pirulen.ttf", 36, 1200, 250 + sbDataCount * 120, 255, 255, 255, 255, 0, 0));
            std::sprintf(sbBuf, "%08d", data.second);
            AddNewObject(new Engine::Label(sbBuf, "pirulen.ttf", 36, 1200, 300 + sbDataCount * 120, 255, 255, 255, 255, 0, 0));
            sbDataCount++;
        }
    }
    // if records < 3
    for (; sbDataCount < 3; sbDataCount++) {
        AddNewObject(new Engine::Label("-", "pirulen.ttf", 36, 1200, 250 + sbDataCount * 120, 255, 255, 255, 255, 0, 0));
        AddNewObject(new Engine::Label("00000000", "pirulen.ttf", 36, 1200, 300 + sbDataCount * 120, 255, 255, 255, 255, 0, 0));
    }
}

void StageSelectScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
