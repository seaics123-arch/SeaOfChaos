#pragma once
#include <raylib.h>
#include "entity.hpp"

class CoralBlock : public Entity {
public:
    CoralBlock(Vector2 position);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
};
