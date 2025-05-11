#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main()
{
    const unsigned int WINDOW_WIDTH = 800;
    const unsigned int WINDOW_HEIGHT = 600;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "SFML window");

    const sf::Font font("arial.ttf");
    sf::Text text(font, "Hello SFML", 50);

    sf::RectangleShape rect({50, 50});
    rect.setPosition({50, 50});

    sf::RectangleShape enemy({50, 50});
    enemy.setPosition({200, 200});

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            movement.x -= 1.f; // Intend to move left.
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            movement.x += 1.f; // Intend to move right.
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            movement.y -= 1.f; // Intend to move up (Y is typically down in screen coordinates).
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            movement.y += 1.f; // Intend to move down.
        }

        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        // If the length is not zero (i.e., a key was pressed), normalize it.
        if (length != 0)
        {
            // Divide the movement vector by its length to make it a unit vector (length 1).
            movement /= length;
        }

        sf::FloatRect rectBounds = rect.getGlobalBounds();
        // Get the global bounding rectangle of the enemy shape.
        sf::FloatRect enemyBounds = enemy.getGlobalBounds();

        if (rectBounds.findIntersection(enemyBounds).has_value())
        {
            enemy.setFillColor(sf::Color(255, 0, 0, 128));
        }else{
            enemy.setFillColor(sf::Color::Green);
        }

        rect.move(movement);

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(rect);

        // Draw the enemy
        window.draw(enemy);

        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }
}