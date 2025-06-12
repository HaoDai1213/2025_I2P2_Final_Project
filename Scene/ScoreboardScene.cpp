#include <functional>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <tuple>

#include "Engine/LOG.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "ScoreboardScene.hpp"

std::ifstream in;
std::string pname, pdate, ptime;
int score, page, maxpage;

struct scorecmp {
    bool operator() (const std::tuple<std::string, std::string, std::string, int>& A, const std::tuple<std::string, std::string, std::string, int>& B) {
        return std::get<3>(A) > std::get<3>(B);
    }
};

std::multiset<std::tuple<std::string, std::string, std::string, int>, scorecmp> records;

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    int count = 0;
    if (!page) page = 1;

    if (records.empty()) {
        in.open("C:/Users/barry/Desktop/2025_I2P2_TowerDefense-main/Resource/scoreboard.txt");
        while (in >> pdate >> ptime >> pname >> score) {
            count++;
            records.insert(std::tuple<std::string, std::string, std::string, int>(pdate, ptime, pname, score));
        }
        in.close();
    }
    
    if (!maxpage) maxpage = (count % 5 == 0 ? count / 5 : count / 5 + 1);

    Draw(page);

    AddNewObject(new Engine::Label("SCOREBOARD", "pirulen.ttf", 48, halfW, 100, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 250, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next >", "pirulen.ttf", 48, halfW + 460, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 650, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("< Prev", "pirulen.ttf", 48, halfW - 460, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    records.clear();
    IScene::Terminate();
}

void ScoreboardScene::Draw(int curPage) {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    int ith = 0;

    for (auto& [playerDate, playerTime, playerName, playerScore] : records) {
        ith++;
        if (ith > 5 * (curPage - 1) && ith <= 5 * curPage) {
            int heightpower = (ith % 5 == 0 ? 5 : ith % 5);
            AddNewObject(new Engine::Label(std::to_string(ith), "pirulen.ttf", 36, halfW - 550, 100 + heightpower * 80, 255, 255, 255, 255, 0.5, 0.5));
            AddNewObject(new Engine::Label(playerDate, "pirulen.ttf", 36, halfW - 350, 100 + heightpower * 80, 255, 255, 255, 255, 0.5, 0.5));
            AddNewObject(new Engine::Label(playerTime, "pirulen.ttf", 36, halfW - 100, 100 + heightpower * 80, 255, 255, 255, 255, 0.5, 0.5));
            AddNewObject(new Engine::Label(playerName, "pirulen.ttf", 36, halfW + 250, 100 + heightpower * 80, 255, 255, 255, 255, 0.5, 0.5));
            AddNewObject(new Engine::Label(std::to_string(playerScore), "pirulen.ttf", 36, halfW + 550, 100 + heightpower * 80, 255, 255, 255, 255, 0.5, 0.5));
        }
    }
}


void ScoreboardScene::PrevOnClick(int dummy) {
    if (page != 1) {
        page--; 
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard-scene");
    }
}

void ScoreboardScene::NextOnClick(int dummy) {
    if (page != maxpage) {
        page++;
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard-scene");
        Engine::LOG(Engine::INFO) << "page: " << page << "maxpage: " << maxpage;
    }
}

void ScoreboardScene::BackOnClick(int stage) {
    // Change to select scene.
    page = 1;
    maxpage = 0;
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}