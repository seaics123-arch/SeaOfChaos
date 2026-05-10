#pragma once
#include <raylib.h>
#include "entity.hpp"

// Forward-declare Submarine so PowerUp can call ApplyTo() without a circular include
class Submarine;

class PowerUp : public Entity {
public:
    enum Type {
        DOUBLE_TORPEDO,   // fire two torpedoes side by side for 8 seconds
        SHIELD,           // absorb one enemy torpedo hit
        EXTRA_LIFE        // gain one additional life
    };

    PowerUp(Vector2 position, Type type);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;

    // Applies the power-up effect to the submarine, then deactivates self
    void ApplyTo(Submarine& sub, int& lives);

    bool active;
    Type type;

private:
    // Fall speed (power-ups drift downward like dropped items)
    static constexpr float FALL_SPEED = 2.0f;

    // Returns colour matching the power-up type
    Color GetColor() const;
};
