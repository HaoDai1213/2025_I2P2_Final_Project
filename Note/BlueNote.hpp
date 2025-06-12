#ifndef BLUENOTE_HPP
#define BLUENOTE_HPP
#include "Note.hpp"

namespace Engine {
    struct Point;
}   // namespace Engine

class BlueNote : public Note {
public:
    explicit BlueNote(int appearTiming, int hitTiming, int line, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection);
};

#endif   // BLUENOTE_HPP