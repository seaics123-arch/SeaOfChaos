#pragma once
#include <raylib.h>
#include "entity.hpp"

class Torpedo : public Entity { // Represents a torpedo fired by either the submarine or sea creatures
public:
    Torpedo(Vector2 position, int speed);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
    bool IsOffScreen();
    bool active;
private:
    int speed; // Positive = moves down (enemy torpedo), Negative = moves up (player torpedo)
};