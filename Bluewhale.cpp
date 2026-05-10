#include "bluewhale.hpp" // Implementation of the BlueWhale class

BlueWhale::BlueWhale()
{

    // Constructor loads the blue whale image and initializes state
    image = LoadTexture("Graphics/bluewhale.png");
    alive = false;
}

BlueWhale::~BlueWhale()
{
    UnloadTexture(image); // Clean up texture resource
}

void BlueWhale::Spawn() //to spawn a new whale at the top, moving either left-to-right or right-to-left
{
    position.y = 90;
    int side = GetRandomValue(0, 1);

    if (side == 0) {
        position.x = 25;
        speed = 3;
    }
    else {
        position.x = GetScreenWidth() - image.width - 25;
        speed = -3;
    }
    alive = true;
}

void BlueWhale::Update() // Moves horizontally across the top of the screen when alive
{
    if (alive) {
        position.x += speed;
        if (position.x > GetScreenWidth() - image.width - 25 || position.x < 25) {
            alive = false;
        }
    }
}

void BlueWhale::Draw() // Draws the blue whale if it's alive
{
    if (alive) {
        DrawTextureV(image, position, WHITE);
    }
}

Rectangle BlueWhale::GetBounds() // Returns the bounding box for collision detection, or an empty box if not alive
{
    if (alive) {
        return { position.x, position.y, float(image.width), float(image.height) };
    }
    return { position.x, position.y, 0, 0 };
}