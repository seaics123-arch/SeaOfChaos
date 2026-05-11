#include "submarine.hpp"

Submarine::Submarine() // Constructor loads the submarine image and initializes state
{
    image = LoadTexture("Graphics/submarine.png");
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
    torpedoSound = LoadSound("Sounds/laser.ogg");
    doubleTorpedoTimer = 0.0;
    shieldActive = false;
}

Submarine::~Submarine() // Destructor unloads the texture and sound resources
{
    UnloadTexture(image);
    UnloadSound(torpedoSound);
}

void Submarine::Draw()
{
    // Draw shield indicator ring when shield is active
    if (shieldActive) {
        DrawCircleLines(
            position.x + image.width / 2,
            position.y + image.height / 2,
            float(image.width),
            { 120, 255, 120, 200 }
        );
    }
    DrawTextureV(image, position, WHITE);
}

void Submarine::Update()
{
    // Tick down the double-torpedo timer each frame
    if (doubleTorpedoTimer > 0.0) {
        doubleTorpedoTimer -= GetFrameTime();
        if (doubleTorpedoTimer < 0.0) doubleTorpedoTimer = 0.0;
    }
}

void Submarine::MoveLeft() // Moves the submarine left, ensuring it doesn't go off-screen
{
    position.x -= 7;
    if (position.x < 25) {
        position.x = 25;
    }
}

void Submarine::MoveRight() // Moves the submarine right, ensuring it doesn't go off-screen
{
    position.x += 7;
    if (position.x > GetScreenWidth() - image.width - 25) {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Submarine::FireTorpedo() // Fires a torpedo if enough time has passed
{
    if (GetTime() - lastFireTime >= 0.35) {
        float cx = position.x + image.width / 2 - 2;

        // Single torpedo (centre)
        torpedoes.push_back(Torpedo({ cx, position.y }, -6));

        // Double torpedo: fire a second one offset to the right
        if (doubleTorpedoTimer > 0.0) {
            torpedoes.push_back(Torpedo({ cx + 12, position.y }, -6));
        }

        lastFireTime = GetTime();
        PlaySound(torpedoSound);
    }
}

Rectangle Submarine::GetBounds() // Returns the bounding box for collision detection
{
    return { position.x, position.y, float(image.width), float(image.height) };
}

void Submarine::Reset() // Resets the submarine to its initial state, used when starting a new game
{
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    torpedoes.clear();
    doubleTorpedoTimer = 0.0;
    shieldActive = false;
}