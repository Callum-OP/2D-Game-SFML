#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Physics.cpp"
#include "Physics.hpp"

#include "TileMap.cpp"

#include "Player.cpp"
#include "Player.hpp"

#include "Enemy.cpp"
#include "Enemy.hpp"  

int main()
{
    // Create colliders list
    std::vector<Object*> colliders;
    const int TILE_SIZE = 96;

    // Create a player
    Object playerCollider = { Vec2(275, 200), { Vec2(-50, -50), Vec2(50, 50) } };
    Player player;
    player.sprite.setOrigin({240.0f / 2.0f, 350.0f / 2.0f});
    player.sprite.setPosition(toSF(playerCollider.pos));

    // Create tilemap
    MapLoader map;
    TileMapRenderer renderer;
    map.loadFromFile("level.txt");

    // Set up wall and pickup colliders
    std::vector<Object> wallObjects;
    std::vector<Object*> pickupColliders;
    std::vector<Object> pickupObjects;
    std::vector<Enemy> enemies;
    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("playerSpritesheet.png"))
        throw std::runtime_error("Failed to load Wall.png");
    sf::Vector2i size = map.getSize();
    for (int y = 0; y < size.y; ++y) { // height
        for (int x = 0; x < size.x; ++x) { // width
            // Walls
            if (map.getTile(x, y) == '#') {
                wallObjects.reserve(size.x * size.y);
                Object wall;
                wall.pos = Vec2(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                wall.aabb.min = Vec2(x * TILE_SIZE, y * TILE_SIZE);
                wall.aabb.max = Vec2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
                // Add to colliders
                wallObjects.push_back(wall);
                colliders.push_back(&wallObjects.back());
            }
            // Pickups
            if (map.getTile(x, y) == 'P') {
                pickupObjects.reserve(size.x * size.y);
                Object pickup;
                pickup.pos = Vec2(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                pickup.aabb.min = Vec2(x * TILE_SIZE, y * TILE_SIZE);
                pickup.aabb.max = Vec2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
                // Add to colliders
                pickupObjects.push_back(pickup);
                pickupColliders.push_back(&pickupObjects.back());
            }
            // Enemies
            if (map.getTile(x, y) == 'E') {
                Vec2 pos(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                Enemy enemy(toSF(pos), sf::Color::Red, enemyTex);
                enemies.push_back(enemy);
            }
        }
    }
    // Add enemies to colliders
    for (auto& enemy : enemies) {
        colliders.push_back(&enemy.collider);
    }

    // Create game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Create game camera
    sf::View camera;
    sf::Vector2u winSize = window.getSize();
    camera.setSize(sf::Vector2f(winSize.x, winSize.y));
    camera.setCenter(player.getPosition());
    window.setView(camera);

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
        for (auto& obj : colliders) {
            Manifold m = { &playerCollider, obj };
            if (AABBvsAABB(&m)) {
                playerCollider.pos.x = originalX;
                break;
            }
        }
        // --- Y axis ---
        float originalY = playerCollider.pos.y;
        playerCollider.pos.y += player.movement.y;
        // Stop if colliding with object
        for (auto& obj : colliders) {
            Manifold m = { &playerCollider, obj };
            if (AABBvsAABB(&m)) {
                playerCollider.pos.y = originalY;
                break;
            }
        }
        // Delete pickup if collided with
        for (auto it = pickupColliders.begin(); it != pickupColliders.end(); ) {
            Manifold m = {&playerCollider, *it};
            if (AABBvsAABB(&m)) {
                // Remove pickup from tilemap
                Vec2 pos = (*it)->pos;
                int tileX = static_cast<int>(pos.x) / TILE_SIZE;
                int tileY = static_cast<int>(pos.y) / TILE_SIZE;
                map.setTile(tileX, tileY, '.');
                // Remove pickup from objects and colliders
                Object* collidedPickup = *it;
                pickupObjects.erase(std::remove_if(pickupObjects.begin(), pickupObjects.end(),
                    [&](const Object& shape) {
                        return &shape == collidedPickup;
                    }), pickupObjects.end());
                it = pickupColliders.erase(it);
                break;
            } else {
                ++it;
            }
        }

        // Draw tilemap with walls and pickups
        renderer.draw(window, map);
        // Draw player
        player.sprite.setPosition(toSF(playerCollider.pos));
        player.draw(window);
        camera.setCenter(player.getPosition());
        window.setView(camera);
        // Draw and update enemies
        for (auto& enemy : enemies) {
            enemy.sprite.setPosition(toSF(enemy.collider.pos));
            enemy.update(deltaTime, player.getPosition(), player.attacking);
            enemy.draw(window);
        }
        // Delete dead enemies
        // First, delete enemy colliders
        for (const Enemy& enemy : enemies) {
            if (enemy.dead) {
                auto it = std::find(colliders.begin(), colliders.end(), &enemy.collider);
                if (it != colliders.end()) {
                    colliders.erase(it);
                }
            }
        }
        // Then, remove the enemies themselves
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& enemy) { return enemy.dead; }),
            enemies.end()
        );

        window.display();
    }
    return 0;
}
