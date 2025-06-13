#include <allegro5/base.h>
#include <random>
#include <string>

#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "BlueNote.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

BlueNote::BlueNote(int appearTiming, int hitTiming, int line, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection) : Note("play/note2.png", appearTiming, hitTiming, line, 2, speed, speedMult, position, forwardDirection) {
}