#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include <string>
#include <set>

#include "Engine/IScene.hpp"

struct cmp {
    bool operator() (const std::pair<std::string, int>& A, const std::pair<std::string, int>& B) const {
        return A.second > B.second;
    }
};

class StageSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    int maxStage;

public:
    std::vector<std::pair<std::string, std::string>> songData;
    std::set<std::pair<std::string, int>, cmp> scoreboardData;
    explicit StageSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int num);
    void BackOnClick(int num);
    void PrevOnClick(int num);
    void NextOnClick(int num);
    void ReadSongData();
    void ReadScoreboard(int num);

    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif   // STAGESELECTSCENE_HPP
