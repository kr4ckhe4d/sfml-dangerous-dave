#include "Level.hpp"       // Include the header definition for Level
#include "Constants.hpp"   // Include global constants like TILE_SIZE

// --- Member Function Implementations ---

// Safely retrieves the tile type at given grid coordinates (x, y).
TileType Level::getTile(int x, int y) const {
    // Boundary check
    if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
        return tiles[y][x]; // Access is [row][column] or [y][x]
    }
    return TileType::Air; // Out of bounds is treated as Air
}

// Safely sets the tile type at given grid coordinates (x, y).
bool Level::setTile(int x, int y, TileType newType) {
    // Boundary check
     if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
        tiles[y][x] = newType;
        return true; // Indicate success
    }
    return false; // Indicate failure (out of bounds)
}


// --- Non-Member Helper Function Implementation ---

// Creates a simple, hardcoded level map for demonstration.
Level createSimpleLevel() {
    Level level;
    // Set level dimensions in tiles.
    level.size = {40, 15};
    // Calculate and store the total level size in pixels.
    level.sizePixels = {(float)level.size.x * TILE_SIZE, (float)level.size.y * TILE_SIZE};
    // Allocate the 2D vector for the tile data, initializing all to Air.
    level.tiles.resize(level.size.y, std::vector<TileType>(level.size.x, TileType::Air));

    // --- Define Solid Tiles ---
    // Floor
    for (int x = 0; x < level.size.x; ++x) {
        level.tiles[level.size.y - 1][x] = TileType::Solid;
    }
    // Platforms
    for (int x = 5; x < 10; ++x) level.tiles[10][x] = TileType::Solid;
    for (int x = 12; x < 16; ++x) level.tiles[8][x] = TileType::Solid;
    level.tiles[6][15] = TileType::Solid;
    level.tiles[6][16] = TileType::Solid;
    for (int x = 25; x < 30; ++x) level.tiles[10][x] = TileType::Solid;
    for (int x = 32; x < 36; ++x) level.tiles[7][x] = TileType::Solid;
    level.tiles[12][21] = TileType::Solid;
    level.tiles[12][22] = TileType::Solid;
    // Walls
    for (int y = 11; y < level.size.y -1; ++y) level.tiles[y][2] = TileType::Solid;
    for (int y = 6; y < 11; ++y) level.tiles[y][18] = TileType::Solid;
    for (int y = 8; y < level.size.y -1; ++y) level.tiles[y][38] = TileType::Solid;

    // --- Define Coin Tiles ---
    level.tiles[9][7] = TileType::Coin;
    level.tiles[7][14] = TileType::Coin;
    level.tiles[9][27] = TileType::Coin;
    level.tiles[6][34] = TileType::Coin;
    level.tiles[11][21] = TileType::Coin;

    return level; // Return the fully defined level structure.
}
