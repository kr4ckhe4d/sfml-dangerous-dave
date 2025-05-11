#pragma once

#include <SFML/Graphics/RectangleShape.hpp> // For sf::RectangleShape
#include <SFML/System/Vector2.hpp>         // For sf::Vector2f

// Forward declaration of Level struct to avoid circular includes.
// We only need to know that 'Level' exists here, not its full definition.
// The full definition is included in Player.cpp where it's needed.
struct Level;

// Structure to group together data and functions for the player character.
struct Player {
    // --- Member Variables ---
    sf::RectangleShape shape;    // Visual representation
    sf::Vector2f velocity;       // Current movement speed/direction
    bool isOnGround;             // Is the player standing on a solid tile?
    int score;                   // Player's score (e.g., collected coins)

    // --- Member Functions (Declarations) ---
    // Constructor
    Player(sf::Vector2f startPos);

    // Physics & Movement
    void applyGravity();
    void jump();
    void updatePosition();

    // Collision Handling
    void handleCollision(const Level& level); // Collision with solid tiles
    void handleLevelBounds(const Level& level); // Collision with level edges
};
