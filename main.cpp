#include <iostream>
#include <cmath>
#include <limits>

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

#include "PathFinder.cpp"

#include "Constants.hpp"

// Functions for conversions
inline sf::Vector2i worldToGrid(const Vec2& pos, int tileSize) {
    return sf::Vector2i(static_cast<int>(pos.x) / tileSize,
                        static_cast<int>(pos.y) / tileSize);
}
inline sf::Vector2f gridToWorld(const Node* node, int tileSize) {
    return sf::Vector2f(node->x * tileSize + tileSize / 2,
        node->y * tileSize + tileSize / 2);
}

// Function for pickup object collision
template <typename PickupType, typename EffectFunc>
void handlePickup(std::vector<PickupType>& pickups,
                  Object& playerCollider,
                  MapLoader& map,
                  EffectFunc effect) {
    for (auto it = pickups.begin(); it != pickups.end();) {
        Manifold m = { &playerCollider, it->collider() };
        if (AABBvsAABB(&m)) {
            // Apply pickup effect
            effect(*it);

            // Remove from tilemap
            Vec2 pos = it->collider()->pos;
            int tileX = static_cast<int>(pos.x) / TILE_SIZE;
            int tileY = static_cast<int>(pos.y) / TILE_SIZE;
            map.setTile(tileX, tileY, '.');

            // Remove from vector
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
}

int main()
{
    sf::Clock clock;
    sf::Clock sprintClock;

    // ---- Create a player ----
    Object playerCollider = { Vec2(275, 200), { Vec2(-50, -50), Vec2(50, 50) } };
    Player player;
    player.sprite.setPosition(toSF(playerCollider.pos));

    // ---- Load tilemap ----
    MapLoader map;
    TileMapRenderer renderer;
    map.loadFromFile("level.txt");

    // ---- Set up colliders ----
    std::vector<Object*> colliders; // Create colliders list
    struct healthPickup {
        Object healthPickup;
        Object* collider() { return &healthPickup; }
    };
    std::vector<healthPickup> healthPickups;
    struct goldPickup {
        Object goldPickup;
        Object* collider() { return &goldPickup; }
    };
    std::vector<goldPickup> goldPickups;
    struct Enem {
        Enemy enemy;
        Object* collider() { return &enemy.collider; }
    };

    // ---- Set up objects ----
    std::vector<Enem> enemies;
    std::vector<Object> wallObjects;
    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("assets/images/enemySpritesheet.png"))
        throw std::runtime_error("Failed to load enemy spritesheet");
    sf::Vector2i size = map.getSize();
    wallObjects.reserve(size.x * size.y);
    for (int y = 0; y < size.y; ++y) { // Height
        for (int x = 0; x < size.x; ++x) { // Width
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
            // Health pickups
            if (map.getTile(x, y) == 'H') {
                Object pickup;
                pickup.pos = Vec2(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                pickup.aabb.min = Vec2(x * TILE_SIZE, y * TILE_SIZE);
                pickup.aabb.max = Vec2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
                // Add to list
                healthPickups.push_back(healthPickup{pickup});
            }
            // Gold pickups
            if (map.getTile(x, y) == 'G') {
                Object pickup;
                pickup.pos = Vec2(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                pickup.aabb.min = Vec2(x * TILE_SIZE, y * TILE_SIZE);
                pickup.aabb.max = Vec2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
                // Add to list
                goldPickups.push_back(goldPickup{pickup});
            }
            // Enemies
            if (map.getTile(x, y) == 'E') {
                Vec2 pos(x * TILE_SIZE + TILE_SIZE / 2.0f, y * TILE_SIZE + TILE_SIZE / 2.0f);
                Enemy enemy(toSF(pos), enemyTex);
                enemies.push_back(Enem{enemy});
            }
        }
    }
    // Set up pathfinding grid
    Grid grid(size.x, size.y);
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            if (map.getTile(x, y) == '#') {
                grid.nodes[y][x].wall = true;
            }
        }
    }

    // ---- Set up game window ----
    // Create game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    // Create game camera
    sf::View camera;
    sf::Vector2u winSize = window.getSize();
    camera.setSize(sf::Vector2f(winSize.x, winSize.y));
    camera.setCenter(player.getPosition());
    window.setView(camera);

    // ---- Events ----
    // Timers and keys used for detecting double taps
    sf::Time lastClickTime; // The last click of each key
    const sf::Time doubleClickTime = sf::milliseconds(500); // Expected time limit for second click to happen
    sf::Keyboard::Scancode lastDirection = sf::Keyboard::Scancode::Unknown; // The direction of last key press
    std::map<sf::Keyboard::Scancode, bool> keyHeld; // Check if key is held
    bool sprint; // If player should sprint or not
    bool isDoubleTap = false; // If use has double tapped or not

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };
    // Check when key is pressed
    const auto onKeyPressed = [&window, &player, &sprintClock, &lastClickTime, &doubleClickTime, &lastDirection, &keyHeld, &sprint, &isDoubleTap](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
        sf::Time now = sprintClock.getElapsedTime();
        // Check for double tap
        if (!keyHeld[keyPressed.scancode]) {
            // Update key held
            keyHeld[keyPressed.scancode] = true;
            // Sprint only if within expected click time and facing same direction as last click
            if ((keyPressed.scancode == lastDirection) && (now - lastClickTime < doubleClickTime) && (!sprint)) {
                isDoubleTap = true;
            } else {
                isDoubleTap = false;
            }
            player.sprint(isDoubleTap);
            if (isDoubleTap) {
                sprint = true;
            } else {
                sprint = false;
            }
        }
        lastDirection = keyPressed.scancode; // Key that was last clicked
        lastClickTime = now; // Time user last clicked
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

        // Handle events and updates
        window.handleEvents(onClose, onKeyPressed, onKeyReleased);
        player.handleInput();
        player.update();

        // Create new window with black background
        window.clear(sf::Color::Black);

        //---- Movement ----
        Vec2 originalPos = playerCollider.pos;
        // X axis
        float originalX = playerCollider.pos.x;
        playerCollider.pos.x += player.movement.x;
        // Stop if colliding with object
        for (auto& enem : enemies) { Manifold m = { &playerCollider, enem.collider() };
            if (AABBvsAABB({&m})) { playerCollider.pos.x = originalX; break; }
        }
        for (auto& obj : colliders) { Manifold m = { &playerCollider, obj };
            if (AABBvsAABB(&m)) { playerCollider.pos.x = originalX; break; }
        }
        // Y axis
        float originalY = playerCollider.pos.y;
        playerCollider.pos.y += player.movement.y;
        // Stop if colliding with object
        for (auto& enem : enemies) { Manifold m = { &playerCollider, enem.collider() };
            if (AABBvsAABB({&m})) { playerCollider.pos.y = originalY; break; }
        }
        for (auto& obj : colliders) { Manifold m = { &playerCollider, obj };
            if (AABBvsAABB(&m)) { playerCollider.pos.y = originalY; break; }
        }

        //---- Damage ----
        // Check if enemy is attacking player, if close enough damage player
        for (auto& enem : enemies) {
            float dx = player.getPosition().x - enem.enemy.getPosition().x;
            float dy = player.getPosition().y - enem.enemy.getPosition().y;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (enem.enemy.attacking && dist <= enem.enemy.attackRadius) {
                player.takeDamage(1);
            }
        }

        //---- Pickups ----
        // Heal player when colliding with health pickups if not at max health
        if (player.getHealth() < player.getMaxHealth()) {
            handlePickup(healthPickups, playerCollider, map, [&](auto& pickup) {
                    player.heal(1);
            
            });
        }
        // Give gold when colliding with gold pickups
        handlePickup(goldPickups, playerCollider, map, [&](auto& pickup) {
            player.addGold(25);
        });

        //---- Draw items ----
        // Draw tilemap with walls and healthPickups
        renderer.draw(window, map);
        // Draw shadows first and update enemies
        player.drawShadow(window);
        for (auto& enem : enemies) {
            enem.enemy.sprite.setPosition(toSF(enem.enemy.collider.pos));
            enem.enemy.update(deltaTime, grid, player.getPosition(), player.attacking);
            enem.enemy.drawShadow(window);
        }
        // Draw player
        player.sprite.setPosition(toSF(playerCollider.pos));
        player.drawUI(window, camera);
        player.drawPlayer(window);
        camera.setCenter(player.getPosition());
        window.setView(camera);
        if (player.isDead()) {
        }
        // Draw enemies
        for (auto& enem : enemies) {
            enem.enemy.drawEnemy(window);
        }
        // Delete dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [](const Enem& enem) {
                return enem.enemy.isDead();
            }),
            enemies.end()
        );

        // Display window
        window.display();
    }
    return 0;
}
