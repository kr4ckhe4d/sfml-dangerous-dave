// --- Includes ---
#include <SFML/Graphics.hpp> // For window, view, shapes, text, events, etc.
#include <optional>          // For event polling
#include <vector>            // Used indirectly via Level.hpp
#include <string>            // For std::to_string
#include <iostream>          // For std::cout, std::cerr
#include <cmath>             // Used indirectly via Player.cpp
#include <filesystem>        // For font loading path
#include <algorithm>         // For std::clamp

// Include our custom headers
#include "gamefiles/Constants.hpp" // Game constants
#include "gamefiles/Level.hpp"     // Level definition and createSimpleLevel()
#include "gamefiles/Player.hpp"    // Player definition

// --- Helper Functions (Specific to this main file) ---

// Draws the level tiles that are currently visible within the camera's view.
// Kept in main.cpp because it needs the RenderWindow.
void drawLevel(sf::RenderWindow &window, const Level &level)
{
    sf::RectangleShape solidTileShape({(float)TILE_SIZE, (float)TILE_SIZE});
    solidTileShape.setFillColor(sf::Color::Blue);

    sf::CircleShape coinShape(TILE_SIZE * 0.3f);
    coinShape.setFillColor(sf::Color::Yellow);
    coinShape.setOrigin({coinShape.getRadius(), coinShape.getRadius()});

    // View Culling
    sf::View currentView = window.getView();
    sf::FloatRect viewBounds;
    viewBounds.position = currentView.getCenter() - currentView.getSize() / 2.f;
    viewBounds.size = currentView.getSize();
    int startX = std::max(0, static_cast<int>(viewBounds.position.x / TILE_SIZE));
    int endX = std::min((int)level.size.x, static_cast<int>((viewBounds.position.x + viewBounds.size.x) / TILE_SIZE) + 1);
    int startY = std::max(0, static_cast<int>(viewBounds.position.y / TILE_SIZE));
    int endY = std::min((int)level.size.y, static_cast<int>((viewBounds.position.y + viewBounds.size.y) / TILE_SIZE) + 1);

    for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            TileType currentTile = level.tiles[y][x];
            if (currentTile == TileType::Solid)
            {
                solidTileShape.setPosition({(float)x * TILE_SIZE, (float)y * TILE_SIZE});
                window.draw(solidTileShape);
            }
            else if (currentTile == TileType::Coin)
            {
                coinShape.setPosition({(float)x * TILE_SIZE + TILE_SIZE / 2.f,
                                       (float)y * TILE_SIZE + TILE_SIZE / 2.f});
                window.draw(coinShape);
            }
        }
    }
}

// Handles checking for and collecting coins.
// Kept in main.cpp as it modifies both Player and Level state.
// Takes a non-const reference to level to allow modifying tiles.
void handleCoinCollection(Player &player, Level &level)
{
    sf::FloatRect playerBounds = player.shape.getGlobalBounds();
    int leftTile = static_cast<int>((playerBounds.position.x + COLLISION_EPSILON) / TILE_SIZE);
    int rightTile = static_cast<int>((playerBounds.position.x + playerBounds.size.x - COLLISION_EPSILON) / TILE_SIZE);
    int topTile = static_cast<int>((playerBounds.position.y + COLLISION_EPSILON) / TILE_SIZE);
    int bottomTile = static_cast<int>((playerBounds.position.y + playerBounds.size.y - COLLISION_EPSILON) / TILE_SIZE);

    for (int y = topTile; y <= bottomTile; ++y)
    {
        for (int x = leftTile; x <= rightTile; ++x)
        {
            if (level.getTile(x, y) == TileType::Coin)
            {
                player.score++;
                level.setTile(x, y, TileType::Air); // Remove coin
                std::cout << "Coin collected! Score: " << player.score << std::endl;
                // Optional: Add sound effect here
            }
        }
    }
}

// --- Main Game Function ---
int main()
{
    // --- Window Setup ---
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Scrolling Platformer");
    window.setFramerateLimit(60);

    // --- Font and Text Setup (for Score Display) ---
    sf::Font font;
    std::filesystem::path fontPath = "arial.ttf"; // Make sure arial.ttf is accessible
    if (!font.openFromFile(fontPath))
    {
        std::cerr << "Error loading font: " << fontPath.string() << std::endl;
    }
    sf::Text scoreText({font, "Score: 0"});
    scoreText.setFillColor(sf::Color::White);

    // --- Create Level and Player ---
    Level currentLevel = createSimpleLevel();                                   // Uses function from Level.cpp
    Player player({TILE_SIZE * 1.5f, TILE_SIZE * (currentLevel.size.y - 3.f)}); // Uses constructor from Player.cpp

    // --- View (Camera) Setup ---
    sf::View gameView({0.f, 0.f}, {(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT});

    // --- Game Loop ---
    while (window.isOpen())
    {
        // --- 1. Event Handling ---
        std::optional<sf::Event> optEvent;
        while ((optEvent = window.pollEvent()))
        {
            if (optEvent->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (optEvent->is<sf::Event::KeyPressed>())
            {
                if (auto *keyPressed = optEvent->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scan::Space || keyPressed->scancode == sf::Keyboard::Scan::Up)
                    {
                        player.jump(); // Calls Player member function
                    }
                }
            }
        }

        // --- 2. Input Handling (Continuous) ---
        player.velocity.x = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            player.velocity.x = -PLAYER_MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            player.velocity.x = PLAYER_MOVE_SPEED;
        }

        // --- 3. Game Logic / Updates ---
        player.applyGravity();                  // Calls Player member function
        player.handleCollision(currentLevel);   // Calls Player member function
        player.handleLevelBounds(currentLevel); // Calls Player member function
        player.updatePosition();                // Calls Player member function

        handleCoinCollection(player, currentLevel); // Call helper function

        // --- Update View Position ---
        sf::Vector2f viewCenter = player.shape.getPosition();
        float minViewX = gameView.getSize().x / 2.f;
        float maxViewX = currentLevel.sizePixels.x - gameView.getSize().x / 2.f;
        float minViewY = gameView.getSize().y / 2.f;
        float maxViewY = currentLevel.sizePixels.y - gameView.getSize().y / 2.f;
        if (currentLevel.sizePixels.x < gameView.getSize().x)
            minViewX = maxViewX = currentLevel.sizePixels.x / 2.f;
        if (currentLevel.sizePixels.y < gameView.getSize().y)
            minViewY = maxViewY = currentLevel.sizePixels.y / 2.f;
        viewCenter.x = std::clamp(viewCenter.x, minViewX, maxViewX);
        viewCenter.y = std::clamp(viewCenter.y, minViewY, maxViewY);
        gameView.setCenter(viewCenter);

        // --- Update Score Text ---
        scoreText.setString("Score: " + std::to_string(player.score));

        // --- 4. Rendering ---
        window.clear(sf::Color(100, 150, 255));

        // Apply the game view for world elements
        window.setView(gameView);
        drawLevel(window, currentLevel); // Call helper function
        window.draw(player.shape);       // Draw player shape

        // Draw HUD Elements
        window.setView(window.getDefaultView()); // Reset view for HUD
        scoreText.setPosition({10.f, 10.f});     // Use fixed screen coordinates
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
