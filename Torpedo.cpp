#include "torpedo.hpp"

// Constructor initializes the torpedo's position, speed, and active state
Torpedo::Torpedo(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true;
}

// Draws the torpedo as a small rectangle if it's active
void Torpedo::Draw()
{
    if (active)
        DrawRectangle(position.x, position.y, 4, 15, { 100, 220, 255, 255 });
}

Rectangle Torpedo::GetBounds()
{
    return { position.x, position.y, 4, 15 };
}

// Checks if the torpedo has moved off the top or bottom of the screen
bool Torpedo::IsOffScreen()
{
    return (position.y > GetScreenHeight() - 100 || position.y < 25);
}


// Updates the torpedo's position based on its speed and deactivates it if it goes off-screen
void Torpedo::Update()
{
    position.y += speed;
    if (active) {
        if (IsOffScreen()) {
            active = false;
        }
    }
}