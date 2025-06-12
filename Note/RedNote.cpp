#include <allegro5/base.h>
#include <random>
#include <string>

#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "RedNote.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

RedNote::RedNote(int timing, int line, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection) : Note("play/note1.png", timing, line, 1, speed, speedMult, position, forwardDirection) {
}