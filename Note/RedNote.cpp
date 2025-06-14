#include <allegro5/base.h>
#include <random>
#include <string>

#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "RedNote.hpp"
#include "Scene/PlayScene.hpp"

RedNote::RedNote(int appearTiming, int hitTiming, int line, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection) : Note("play/note1.png", appearTiming, hitTiming, line, 1, speed, speedMult, position, forwardDirection) {
}