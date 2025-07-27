#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.cpp"
#include "Player.hpp"

#include "Enemy.cpp"
#include "Enemy.hpp"  

int main()
{
    // Create a player
    Player player;

    // Create enemies
    std::vector<Enemy> enemies;
    // Create enemy 1
    sf::Vector2<float> position1(275.f, 200.f); // Set coordinates
    Enemy enemy1(position1, sf::Color::Red);
    enemies.push_back(enemy1);
    // Create enemy 2
    sf::Vector2<float> position2(255.f, 210.f); // Set coordinates
    Enemy enemy2(position2, sf::Color::Red);
    enemies.push_back(enemy2);

    // Create walls
    std::vector<sf::RectangleShape> walls;
    // Create wall 1
    sf::Vector2<float> wallPos1(575.f, 200.f); // Set coordinates
    sf::RectangleShape wall1(wallPos1);
    wall1.setOrigin({wall1.getTextureRect().size.x / 2.0f, wall1.getTextureRect().size.y / 2.0f});
    wall1.setPosition(wallPos1);
    wall1.setFillColor(sf::Color::Red);
    wall1.setScale({0.5f,0.5f});
    walls.push_back(wall1);
    // Create wall 2
    sf::Vector2<float> wallPos2(400.f, 100.f); // Set coordinates
    sf::RectangleShape wall2(wallPos2);
    wall2.setOrigin({wall2.getTextureRect().size.x / 2.0f, wall2.getTextureRect().size.y / 2.0f});
    wall2.setPosition(wallPos2);
    wall2.setFillColor(sf::Color::Red);
    wall2.setScale({0.5f,0.5f});
    walls.push_back(wall2);

    std::vector<sf::FloatRect> wallBounds;

    // Create game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Timers and keys used for detecting double taps
    sf::Clock clock;
    sf::Time lastClickTime; // The last click of each key
    const sf::Time doubleClickTime = sf::milliseconds(500); // Expected time limit for second click to happen
    sf::Keyboard::Scancode lastDirection = sf::Keyboard::Scancode::Unknown; // The direction of last key press

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };
    // Check when key is pressed
    const auto onKeyPressed = [&window, &lastDirection](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
        lastDirection = keyPressed.scancode; // Key that was last clicked
    };
    // Check when key is released
    const auto onKeyReleased = [&window, &player, &clock, &lastClickTime, &doubleClickTime, &lastDirection](const sf::Event::KeyReleased& keyPressed) {
        // If double tap movement keys, tell player to sprint
        if (keyPressed.scancode == sf::Keyboard::Scan::Right || 
            keyPressed.scancode == sf::Keyboard::Scan::D || 
            keyPressed.scancode == sf::Keyboard::Scan::Left || 
            keyPressed.scancode == sf::Keyboard::Scan::A || 
            keyPressed.scancode == sf::Keyboard::Scan::Up || 
            keyPressed.scancode == sf::Keyboard::Scan::W || 
            keyPressed.scancode == sf::Keyboard::Scan::Down || 
            keyPressed.scancode == sf::Keyboard::Scan::S) {
            
            sf::Time now = clock.getElapsedTime(); // Current time
            // Sprint only if within expected click time and facing same direction as last click
            if (now - lastClickTime < doubleClickTime && keyPressed.scancode == lastDirection) {
                player.sprint(true);
            } else {
                player.sprint(false);
            }
            lastDirection = keyPressed.scancode; // Key that was last clicked
            lastClickTime = now; // Time user last clicked
        }
    };

    // Game loop
    while (window.isOpen()) {
        sf::Time delta = clock.restart(); // Time since last frame
        float deltaTime = delta.asSeconds(); // Convert to seconds


        window.handleEvents(onClose, onKeyPressed, onKeyReleased);

        // Handle player controls and enemy updates
        player.handleInput(wallBounds);
        player.update();

        // Create new window with sprites drawn in
        window.clear(sf::Color::White);
        player.draw(window);

        // Display walls
        for (const auto& wall : walls) {
            window.draw(wall);
            sf::FloatRect bounds = wall.getGlobalBounds();
            bounds.size = wall.getGlobalBounds().size *= 0.2f;
            wallBounds.push_back(wall.getGlobalBounds());
        }
        // Display and update enemies
        for (auto& enemy : enemies) {
            enemy.update(deltaTime, player.getPosition(), player.attacking);
            enemy.draw(window);
        }

        // Delete dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& enemy) { return enemy.dead; }),
            enemies.end()
        );


        window.display();
    }
    return 0;
}
