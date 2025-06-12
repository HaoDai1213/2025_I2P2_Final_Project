#ifndef REDNOTE_HPP
#define REDNOTE_HPP
#include "Note.hpp"

namespace Engine {
    struct Point;
}   // namespace Engine

class RedNote : public Note {
public:
    explicit RedNote(int appearTiming, int hitTiming, int line, float speed, float speedMult, Engine::Point position, Engine::Point forwardDirection);
};

#endif   // REDNOTE_HPP