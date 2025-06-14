#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <tuple>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class Player;
class Bullet;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
protected:
    int lives;
    int SpeedMult;
    bool pause;
    unsigned int samplePos;

public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const Engine::Point SpawnGridPoint;
    int MapId;
    float MapBPM;
    float duration;
    float ticks;
    float endTick;
    float lastBullet;
    float lastNote;
    static int gamescore;
    static float accuracy;
    float noteCount;
    float hitCount;
    int combo;
    int playerSpeed;
    bool isRedHit;
    bool isBlueHit;
    static int PFcount;
    static int GRcount;
    static int MScount;
    // Map tiles.
    Group *DebugIndicatorGroup;
    Group *BulletGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Group *NoteGroup;
    Group *PlayerGroup;
    Group *PauseGroup;
    Engine::Label *UIScore;
    Engine::Label *UIAcc;
    Engine::Label *UILives;
    Engine::Label *UICombo;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    Player *player;
    std::list<std::tuple<int, float, int>> bulletData;
    std::list<std::tuple<int, int, int, float>> noteData;
    std::map<int, int> keyState;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void ReadBullet();
    void ReadNote();
    void ConstructUI();
    void HitObject(int timing, int type);
    void Paused();
    void BackOnClick(int num);
    void RetryOnClick(int num);
    void ContinueOnClick(int num);
    // void ModifyReadMapTiles();
};

#endif   // PLAYSCENE_HPP
