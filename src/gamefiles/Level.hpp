#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp> // Required for sf::Vector2u and sf::Vector2f

// Defines symbolic names for different types of tiles in the level grid.
enum class TileType { // Use 'enum class' for stronger type safety
    Air = 0,
    Solid = 1,
    Coin = 2
};

// Structure to hold all data related to a game level.
struct Level {
    // --- Member Variables ---
    std::vector<std::vector<TileType>> tiles; // 2D grid holding the tile types
    sf::Vector2u size;                        // Dimensions of the level in tiles (width, height)
    sf::Vector2f sizePixels;                  // Dimensions of the level in pixels

    // --- Member Functions (Declarations) ---
    // Safely retrieves the tile type at given grid coordinates (x, y).
    TileType getTile(int x, int y) const;
    // Safely sets the tile type at given grid coordinates (x, y).
    bool setTile(int x, int y, TileType newType);
};

// --- Non-Member Helper Function (Declaration) ---
// Creates a simple, hardcoded level map for demonstration.
Level createSimpleLevel();
