#include "Player.hpp"      // Include the header definition for Player
#include "Level.hpp"       // Include the full definition of Level (needed for getTile)
#include "Constants.hpp"   // Include global constants
#include <SFML/Graphics/Rect.hpp> // Needed for sf::FloatRect
#include <iostream>        // Needed for std::cout (in handleLevelBounds)
#include <cmath>           // Needed for std::sqrt (not used here but often in physics)


// --- Member Function Implementations ---

// Constructor
Player::Player(sf::Vector2f startPos)
    : velocity(0.f, 0.f), // Initialize velocity using member initializer list
      isOnGround(false),  // Initialize isOnGround
      score(0)            // Initialize score
{
    shape.setSize({TILE_SIZE * 0.8f, TILE_SIZE * 0.95f});
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({shape.getSize().x / 2.f, shape.getSize().y / 2.f});
    shape.setPosition(startPos);
}

// Apply gravity
void Player::applyGravity() {
    velocity.y += GRAVITY;
}

// Jump action
void Player::jump() {
    if (isOnGround) {
        velocity.y = PLAYER_JUMP_VELOCITY;
        isOnGround = false;
    }
}

// Update position based on velocity
void Player::updatePosition() {
    shape.move(velocity);
}

// Handle collision with solid tiles
void Player::handleCollision(const Level& level) {
    isOnGround = false;
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // --- Vertical Collision Check ---
    sf::FloatRect verticalCheckBounds = playerBounds;
    verticalCheckBounds.position.y += velocity.y;
    int leftTileV = static_cast<int>((verticalCheckBounds.position.x + COLLISION_EPSILON) / TILE_SIZE);
    int rightTileV = static_cast<int>((verticalCheckBounds.position.x + verticalCheckBounds.size.x - COLLISION_EPSILON) / TILE_SIZE);
    int topTileV = static_cast<int>((verticalCheckBounds.position.y + COLLISION_EPSILON) / TILE_SIZE);
    int bottomTileV = static_cast<int>((verticalCheckBounds.position.y + verticalCheckBounds.size.y - COLLISION_EPSILON) / TILE_SIZE);

    for (int x = leftTileV; x <= rightTileV; ++x) {
        if (velocity.y > 0 && level.getTile(x, bottomTileV) == TileType::Solid) {
            shape.setPosition({shape.getPosition().x, (float)bottomTileV * TILE_SIZE - shape.getSize().y / 2.f});
            velocity.y = 0;
            isOnGround = true;
            playerBounds = shape.getGlobalBounds();
            break;
        }
        if (velocity.y < 0 && level.getTile(x, topTileV) == TileType::Solid) {
            shape.setPosition({shape.getPosition().x, (float)(topTileV + 1) * TILE_SIZE + shape.getSize().y / 2.f});
            velocity.y = 0;
            playerBounds = shape.getGlobalBounds();
            break;
        }
    }

    // --- Horizontal Collision Check ---
    sf::FloatRect horizontalCheckBounds = playerBounds;
    horizontalCheckBounds.position.x += velocity.x;
    int leftTileH = static_cast<int>((horizontalCheckBounds.position.x + COLLISION_EPSILON) / TILE_SIZE);
    int rightTileH = static_cast<int>((horizontalCheckBounds.position.x + horizontalCheckBounds.size.x - COLLISION_EPSILON) / TILE_SIZE);
    int topTileH = static_cast<int>((playerBounds.position.y + COLLISION_EPSILON) / TILE_SIZE);
    int bottomTileH = static_cast<int>((playerBounds.position.y + playerBounds.size.y - COLLISION_EPSILON) / TILE_SIZE);

    for (int y = topTileH; y <= bottomTileH; ++y) {
         if (velocity.x > 0 && level.getTile(rightTileH, y) == TileType::Solid) {
            shape.setPosition({(float)rightTileH * TILE_SIZE - shape.getSize().x / 2.f, shape.getPosition().y});
            velocity.x = 0;
            break;
        }
        if (velocity.x < 0 && level.getTile(leftTileH, y) == TileType::Solid) {
            shape.setPosition({(float)(leftTileH + 1) * TILE_SIZE + shape.getSize().x / 2.f, shape.getPosition().y});
            velocity.x = 0;
            break;
        }
    }
}

// Handle collision with level boundaries
void Player::handleLevelBounds(const Level& level) {
    sf::Vector2f playerPos = shape.getPosition();
    sf::Vector2f playerHalfSize = shape.getSize() / 2.f;

    // Left
    if (playerPos.x - playerHalfSize.x < 0.f) {
        shape.setPosition({playerHalfSize.x, playerPos.y});
        velocity.x = 0;
    }
    // Right
    if (playerPos.x + playerHalfSize.x > level.sizePixels.x) {
        shape.setPosition({level.sizePixels.x - playerHalfSize.x, playerPos.y});
        velocity.x = 0;
    }
    // Top
    if (playerPos.y - playerHalfSize.y < 0.f) {
        shape.setPosition({playerPos.x, playerHalfSize.y});
        velocity.y = 0;
    }
    // Bottom (Fall out)
    if (playerPos.y + playerHalfSize.y > level.sizePixels.y) {
        std::cout << "Player fell out of bounds!" << std::endl;
        shape.setPosition({TILE_SIZE * 1.5f, TILE_SIZE * (level.size.y - 3.f)}); // Reset
        velocity = {0.f, 0.f};
        isOnGround = false;
    }
}
