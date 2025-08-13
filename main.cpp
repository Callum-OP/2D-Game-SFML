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
    struct Pickup {
        Object pickup;
        Object* collider() { return &pickup; }
    };
    std::vector<Pickup> pickups;
    // Set enemy list
    struct Enem {
        Enemy enemy;
        Object* collider() { return &enemy.collider; }
    };
    std::vector<Enem> enemies;

    std::vector<Object> wallObjects;
    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("playerSpritesheet.png"))
        throw std::runtime_error("Failed to load Wall.png");
    sf::Vector2i size = map.getSize();
    wallObjects.reserve(size.x * size.y);
    for (int y = 0; y < size.y; ++y) { // height
        for (int x = 0; x < size.x; ++x) { // width
            // Walls
            if (map.getTile(x, y) == '#') {
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
                Object pickup;
                pickup.pos = Vec2(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                pickup.aabb.min = Vec2(x * TILE_SIZE, y * TILE_SIZE);
                pickup.aabb.max = Vec2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
                // Add to list
                pickups.push_back(Pickup{pickup});
            }
            // Enemies
            if (map.getTile(x, y) == 'E') {
                Vec2 pos(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                Enemy enemy(toSF(pos), sf::Color::Red, enemyTex);
                enemies.push_back(Enem{enemy});
            }
        }
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
    std::map<sf::Keyboard::Scancode, bool> keyHeld; // Check if key is held
    bool sprint; // Boolean for if sprint is true or not

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };

    // Check when key is pressed
    const auto onKeyPressed = [&window, &player, &clock, &lastClickTime, &doubleClickTime, &lastDirection, &keyHeld, &sprint](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }

        if (!keyHeld[keyPressed.scancode]) {
            // Update key held
            keyHeld[keyPressed.scancode] = true;
            sf::Time now = clock.getElapsedTime();
            // Sprint only if within expected click time and facing same direction as last click
            bool isDoubleTap = (keyPressed.scancode == lastDirection) &&
                (now - lastClickTime < doubleClickTime) &&
                !sprint;
            player.sprint(isDoubleTap);
            if (isDoubleTap) {
                sprint = true; // Consume the double-tap
            } else {
                sprint = false; // Reset if not a valid double-tap
            }
            lastDirection = keyPressed.scancode; // Key that was last clicked
            lastClickTime = now; // Time user last clicked
        }
    };

    
    // Check when key is released
    const auto onKeyReleased = [&window, &keyHeld](const sf::Event::KeyReleased& keyPressed) {
        // If double tap movement keys, tell player to sprint
        if (keyPressed.scancode == sf::Keyboard::Scan::Right || 
            keyPressed.scancode == sf::Keyboard::Scan::D || 
            keyPressed.scancode == sf::Keyboard::Scan::Left || 
            keyPressed.scancode == sf::Keyboard::Scan::A || 
            keyPressed.scancode == sf::Keyboard::Scan::Up || 
            keyPressed.scancode == sf::Keyboard::Scan::W || 
            keyPressed.scancode == sf::Keyboard::Scan::Down || 
            keyPressed.scancode == sf::Keyboard::Scan::S) {
        }
        // Update key held
        keyHeld[keyPressed.scancode] = false;
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
        for (auto& enem : enemies) {
            Manifold m = { &playerCollider, enem.collider()};
            if (AABBvsAABB({&m})) {
                playerCollider.pos.x = originalX;
                break;
            }
        }
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
        for (auto& enem : enemies) {
            Manifold m = { &playerCollider, enem.collider()};
            if (AABBvsAABB({&m})) {
                playerCollider.pos.y = originalY;
                break;
            }
        }
        for (auto& obj : colliders) {
            Manifold m = { &playerCollider, obj };
            if (AABBvsAABB(&m)) {
                playerCollider.pos.y = originalY;
                break;
            }
        }
        // Delete pickup if collided with
        for (auto it = pickups.begin(); it != pickups.end(); ) {
            Manifold m = {&playerCollider, it->collider()};
            if (AABBvsAABB(&m)) {
                // Remove pickup from tilemap
                Vec2 pos = (it)->pickup.pos;
                int tileX = static_cast<int>(pos.x) / TILE_SIZE;
                int tileY = static_cast<int>(pos.y) / TILE_SIZE;
                map.setTile(tileX, tileY, '.');
                // Remove pickup from objects and colliders
                it = pickups.erase(it);
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
        for (auto& enem : enemies) {
            enem.enemy.sprite.setPosition(toSF(enem.enemy.collider.pos));
            enem.enemy.update(deltaTime, player.getPosition(), player.attacking);
            enem.enemy.draw(window);
        }
        // Delete dead enemies
        // First, delete enemy colliders
        for (const Enem& enem : enemies) {
            if (enem.enemy.dead) {
                enemies.erase(
                    std::remove_if(enemies.begin(), enemies.end(), [](const Enem& e) {
                        return e.enemy.dead;
                    }),
                    enemies.end()
                );
            }
        }
        window.display();
    }
    return 0;
}
