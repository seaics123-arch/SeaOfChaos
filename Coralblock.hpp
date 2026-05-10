#pragma once
#include <raylib.h>
#include "entity.hpp"

class CoralBlock : public Entity {    // Represents a single 3x3 pixel block of a coral bunker
public:
    CoralBlock(Vector2 position); // position is the top-left corner of the block
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
};
