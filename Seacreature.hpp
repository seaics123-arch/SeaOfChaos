#pragma once
#include <raylib.h>
#include "entity.hpp"

class SeaCreature : public Entity {
public:
    // Type 1 = Pufferfish (100 pts), Type 2 = Clownfish (200 pts), Type 3 = Shark (300 pts), Type 4 = New fish (150 pts)
    SeaCreature(int type, Vector2 position);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
    void Move(int direction);
    void MoveDown(int distance);
    int GetType();
    Vector2 GetPosition() const;
    static void UnloadImages();
    static Texture2D creatureImages[4]; // Static array to hold textures for all creature types
    int type;
};