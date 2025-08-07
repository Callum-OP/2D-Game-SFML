#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Physics.cpp"

#include "Player.cpp"
#include "Player.hpp"

#include "Enemy.cpp"
#include "Enemy.hpp"  

int main()
{
    // Create a player
    Object playerCollider = { Vec2(275, 200), { Vec2(-50, -50), Vec2(50, 50) } };
    Player player;
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin({player.sprite.getTextureRect().size.x / 2.0f, player.sprite.getTextureRect().size.y / 2.0f});
    player.sprite.setPosition(toSF(playerCollider.pos));

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

    std::vector<Object> pickupColliders;
    std::vector<sf::CircleShape> pickups;

    // I should put the walls code into a seperate file later
    // Create walls
    std::vector<Object> wallColliders;
    std::vector<sf::RectangleShape> walls;
    // Create wall 1
    Object wall1Collider = { Vec2(520, 270), { Vec2(-50, -50), Vec2(50, 50) } };
    sf::RectangleShape wall1(sf::Vector2f(100, 100));
    wall1.setPosition(toSF(wall1Collider.pos));
    wall1.setOutlineThickness(2);
    wall1.setOutlineColor(sf::Color::Black);
    wall1.setFillColor(sf::Color::Transparent);
    // Create wall 2
    Object wall2Collider = { Vec2(420, 120), { Vec2(-50, -50), Vec2(50, 50) } };
    sf::RectangleShape wall2(sf::Vector2f(100, 100));
    wall2.setPosition(toSF(wall2Collider.pos));
    wall2.setOutlineThickness(2);
    wall2.setOutlineColor(sf::Color::Black);
    wall2.setFillColor(sf::Color::Transparent);
    // Set up wall objects list
    wallColliders.push_back(wall1Collider);
    wallColliders.push_back(wall2Collider);
    walls.push_back(wall1);
    walls.push_back(wall2);

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
        player.handleInput();
        player.update();

        // Create new window with sprites drawn in
        window.clear(sf::Color::White);

                // Try movement
        Vec2 originalPos = playerCollider.pos;
        // --- X axis ---
        float originalX = playerCollider.pos.x;
        playerCollider.pos.x += player.movement.x;
        // Stop if colliding with object
        for (auto& obj : wallColliders) {
            Manifold m = { &playerCollider, &obj };
            if (AABBvsAABB(&m)) {
                playerCollider.pos.x = originalX;
                break;
            }
        }
        // --- Y axis ---
        float originalY = playerCollider.pos.y;
        playerCollider.pos.y += player.movement.y;
        // Stop if colliding with object
        for (auto& obj : wallColliders) {
            Manifold m = { &playerCollider, &obj };
            if (AABBvsAABB(&m)) {
                playerCollider.pos.y = originalY;
                break;
            }
        }
        // Delete pickup if collided with
        for (auto it = pickupColliders.begin(); it != pickupColliders.end(); ) {
            Manifold m = {&playerCollider, &(*it)};
            if (AABBvsAABB(&m)) {
                pickups.erase(std::remove_if(pickups.begin(), pickups.end(),
                  [&](const sf::CircleShape& shape) {
                      return shape.getPosition() == toSF(it->pos);
                  }), pickups.end());
                it = pickupColliders.erase(it);
                break;
            } else {
                ++it;
            }
        }

        // Draw walls
        for (const auto& wall : walls) {
          window.draw(wall);
        }
        // Draw pickups
        for (const auto& pickup : pickups) {
          window.draw(pickup);
        }
        // Draw player
        player.sprite.setPosition(toSF(playerCollider.pos));
        player.draw(window);
        // Draw and update enemies
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
