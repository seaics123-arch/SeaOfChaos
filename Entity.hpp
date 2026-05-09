#pragma once
#include <raylib.h>

class Entity {
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual Rectangle GetBounds() = 0;
    virtual ~Entity() = default;
    Entity() : position({ 0, 0 }) {}
protected:
    Vector2 position;
};