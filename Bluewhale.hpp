#pragma once
#include <raylib.h>
#include "entity.hpp"
class BlueWhale : public Entity { // NEW class for the blue whale power-up
public:
    BlueWhale();
    ~BlueWhale();
    void Update() override; // Moves horizontally across the top of the screen when alive
    void Draw() override;
    Rectangle GetBounds() override;
    void Spawn();
    bool alive = false;  // Initialized here
private:
    Texture2D image;
    int speed = 0;  // Initialize here
};