#include "powerup.hpp"
#include "submarine.hpp"

PowerUp::PowerUp(Vector2 position, Type type)
{
    this->position = position;
    this->type = type;
    active = true;
}

void PowerUp::Update()
{
    if (!active) return;

    // Drift downward each frame
    position.y += FALL_SPEED;

    // Deactivate if it falls off screen without being collected
    if (position.y > GetScreenHeight()) {
        active = false;
    }
}

void PowerUp::Draw()
{
    if (!active) return;

    Color color = GetColor();

    // Draw a small diamond shape to distinguish from torpedoes
    // Diamond = rotated square drawn as four lines
    float cx = position.x + 8;
    float cy = position.y + 8;
    float r = 8.0f;

    DrawRectangle(position.x, position.y, 16, 16, color);

    // White label letter inside the box
    const char* label = "?";
    if (type == DOUBLE_TORPEDO) label = "2";
    if (type == SHIELD)         label = "S";
    if (type == EXTRA_LIFE)     label = "+";
    DrawText(label, position.x + 4, position.y + 2, 12, WHITE);

    (void)cx; (void)cy; (void)r; // suppress unused-variable warnings
}

Rectangle PowerUp::GetBounds()
{
    return { position.x, position.y, 16, 16 };
}

void PowerUp::ApplyTo(Submarine& sub, int& lives)
{
    switch (type) {
    case DOUBLE_TORPEDO:
        sub.doubleTorpedoTimer = 8.0;   // 8-second double-fire window
        break;
    case SHIELD:
        sub.shieldActive = true;
        break;
    case EXTRA_LIFE:
        lives++;
        break;
    }
    active = false;
}

Color PowerUp::GetColor() const
{
    switch (type) {
    case DOUBLE_TORPEDO: return { 80,  200, 255, 255 };   // cyan
    case SHIELD:         return { 120, 255, 120, 255 };   // green
    case EXTRA_LIFE:     return { 255, 100, 150, 255 };   // pink
    default:             return WHITE;
    }
}