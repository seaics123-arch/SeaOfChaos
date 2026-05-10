#include <vector>
#include "coralblock.hpp"
#include "entity.hpp"

class CoralBunker : public Entity { // Represents a bunker made of many small coral blocks

public:
    CoralBunker(Vector2 position);
    void Update() override;
    void Draw() override;
    Rectangle GetBounds() override;
    Vector2 GetPosition() const;

    std::vector<CoralBlock> blocks; // The individual blocks that make up the bunker shape
    static std::vector<std::vector<int>> grid; // 2D grid defining the bunker shape (1 = block present, 0 = empty)
};