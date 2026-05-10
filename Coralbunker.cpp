#include "coralbunker.hpp"


// Define the static grid for the bunker shape (1 = block present, 0 = empty)
std::vector<std::vector<int>> CoralBunker::grid = {
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}
};

CoralBunker::CoralBunker(Vector2 position)
// Constructor initializes the bunker shape based on the static grid and the given position
{
    this->position = position; // Set the top-left corner of the bunker

    for (unsigned int row = 0; row < grid.size(); ++row) { // Iterate through each row of the grid
        for (unsigned int column = 0; column < grid[0].size(); ++column) {
            if (grid[row][column] == 1) {
                float posX = position.x + column * 3;
                float posY = position.y + row * 3;
                blocks.push_back(CoralBlock({ posX, posY })); // Each block is 3x3 pixels, so we multiply to get the correct position
            }
        }
    }
}

void CoralBunker::Update()
{
    // Bunkers are static
}

void CoralBunker::Draw() // Draws all the individual blocks that make up the bunker
{
    for (auto& block : blocks) {
        block.Draw();
    }
}

Rectangle CoralBunker::GetBounds()
{
    // Returns the bounding box of the whole bunker shape
    float w = grid[0].size() * 3;
    float h = grid.size() * 3;
    return { position.x, position.y, w, h };
}

Vector2 CoralBunker::GetPosition() const // Returns the top-left corner position of the bunker
{
    return position;
}