#include "seacreature.hpp"


// Static array to hold textures for all creature types, initialized to empty textures
Texture2D SeaCreature::creatureImages[4] = {};

SeaCreature::SeaCreature(int type, Vector2 position)
{
    this->type = type;
    this->position = position;

    if (creatureImages[type - 1].id == 0) {
        switch (type) {
        case 1:
            // Pufferfish - lowest row, 100 pts
            creatureImages[0] = LoadTexture("Graphics/pufferfish.png");
            break;
        case 2:
            // Clownfish - middle rows, 200 pts
            creatureImages[1] = LoadTexture("Graphics/clownfish.png");
            break;
        case 3:
            // Shark - top row, 300 pts
            creatureImages[2] = LoadTexture("Graphics/shark.png");
            break;
        case 4:
            // New fish - row 2, 150 pts
            creatureImages[3] = LoadTexture("Graphics/newfish.png");
            break;
        default:
            creatureImages[0] = LoadTexture("Graphics/pufferfish.png");
            break;
        }
    }
}


// Draws the creature at its current position using the appropriate texture based on its type
void SeaCreature::Draw()
{
    DrawTextureV(creatureImages[type - 1], position, WHITE);
}

void SeaCreature::Update()
{
    // Direction-based movement is driven by Game::MoveCreatures()
    // This override satisfies the Entity interface
}

void SeaCreature::Move(int direction)
{
    position.x += direction;
}

void SeaCreature::MoveDown(int distance)
{
    position.y += distance;
}

Vector2 SeaCreature::GetPosition() const
{
    return position;
}

int SeaCreature::GetType()
{
    return type;
}


Rectangle SeaCreature::GetBounds()
{
    return {
        position.x,
        position.y,
        float(creatureImages[type - 1].width),
        float(creatureImages[type - 1].height)
    };
}


// Static method to unload all creature images from GPU memory when the game exits
void SeaCreature::UnloadImages()
{
    for (int i = 0; i < 4; i++) {
        UnloadTexture(creatureImages[i]);
    }
}