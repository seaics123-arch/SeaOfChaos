#include "coralblock.hpp"

CoralBlock::CoralBlock(Vector2 position) // position is the top-left corner of the block
{
    this->position = position;
}

void CoralBlock::Update()
{
    // Coral blocks are static; no movement logic needed
}

void CoralBlock::Draw()
{
    // Coral-pink colour: warm pinkish-orange to look like coral reef
    DrawRectangle(position.x, position.y, 3, 3, { 194, 154, 108, 255 });
}

Rectangle CoralBlock::GetBounds()
{
    return { position.x, position.y, 3, 3 };
}