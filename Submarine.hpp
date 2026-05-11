#pragma once
#include <raylib.h>
#include <vector>
#include "torpedo.hpp"
#include "entity.hpp"

class Submarine : public Entity { // Represents the submarine,can move left/right and fire torpedoes
public:
    Submarine();
    ~Submarine();
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
    void MoveLeft();
    void MoveRight();
    void FireTorpedo();
    void Reset();
    std::vector<Torpedo> torpedoes;

    // Power-up state - written directly by PowerUp::ApplyTo()
    double doubleTorpedoTimer;  // >0 means double-fire is active
    bool   shieldActive;        // true = next hit is absorbed

private:
    Texture2D image; // Submarine image texture
    double lastFireTime;
    Sound torpedoSound;
};