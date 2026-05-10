#include "torpedo.hpp"

Torpedo::Torpedo(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true;
}

void Torpedo::Draw()
{
    if (active)
        DrawRectangle(position.x, position.y, 4, 15, { 100, 220, 255, 255 });
}

Rectangle Torpedo::GetBounds()
{
    return { position.x, position.y, 4, 15 };
}

bool Torpedo::IsOffScreen()
{
    return (position.y > GetScreenHeight() - 100 || position.y < 25);
}

void Torpedo::Update()
{
    position.y += speed;
    if (active) {
        if (IsOffScreen()) {
            active = false;
        }
    }
}