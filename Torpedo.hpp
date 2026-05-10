#pragma once
#include <raylib.h>
#include "entity.hpp"

class Torpedo : public Entity {
public:
    Torpedo(Vector2 position, int speed);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
    bool IsOffScreen();
    bool active;
private:
    int speed;
};