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

#include "player.cpp"
#include "player.hpp"

#include "Enemy.cpp"
#include "Enemy.hpp"  

#include "PathFinder.cpp"

#include "Entity.hpp"
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
                  Object& (playerCollider),
                  MapLoader& map,
                  EffectFunc effect) {
    for (auto it = pickups.begin(); it != pickups.end();) {
        Manifold m = { &(playerCollider), it->collider() };
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

    // Set up audio
    sf::SoundBuffer goldPickupBuffer;
    sf::SoundBuffer healthPickupBuffer;

    // Load audio from files
    if (!goldPickupBuffer.loadFromFile("assets/audio/goldPickup.ogg")) {
        std::cerr << "Error: Could not load gold pickup sound file!" << std::endl;
        return -1;
    }
    if (!healthPickupBuffer.loadFromFile("assets/audio/healthPickup.ogg")) {
        std::cerr << "Error: Could not load health pickup sound file!" << std::endl;
        return -1;
    }

    // Creat sounds
    sf::Sound goldPickupSound(goldPickupBuffer);
    sf::Sound healthPickupSound(healthPickupBuffer);

    // Create list of entities
    std::vector<std::shared_ptr<Entity>> entities;

    // ---- Create a player ----
    sf::Vector2f pos = {275, 200};
    auto player = std::make_shared<Player>(pos);
    entities.push_back(player);
    Object playerCollider = (player->playerCollider);

    // ---- Load tilemap ----
    MapLoader map;
    TileMapRenderer renderer;
    map.loadFromFile("level.txt");
    // Set up pathfinding grid
    sf::Vector2i size = map.getSize();
    Grid grid(size.x, size.y);
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            if (map.getTile(x, y) == '#') {
                grid.nodes[y][x].wall = true;
            }
        }
    }

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

    // ---- Set up objects ----
    std::vector<Object> wallObjects;
    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("assets/images/enemySpritesheet.png"))
        throw std::runtime_error("Failed to load enemy spritesheet");
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
                entities.push_back(std::make_shared<Enemy>(
                    toSF(pos), enemyTex, grid, *player
                ));
            }
        }
    }

    // ---- Set up game window ----
    // Create game window
    sf::RenderWindow window(sf::VideoMode({1500, 800}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    // Create game camera
    sf::View camera;
    sf::Vector2u winSize = window.getSize();
    camera.setSize(sf::Vector2f(winSize.x, winSize.y));
    camera.setCenter(player->getPosition());
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
    bool isPaused = false;
    // Check when key is pressed
    const auto onKeyPressed = [&window, &player, &sprintClock, &lastClickTime, &doubleClickTime, &lastDirection, &keyHeld, &sprint, &isDoubleTap, &isPaused](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
        // Toggle pause when P is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::P)) {
            isPaused = !isPaused;
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
            player->sprint(isDoubleTap);
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

    // Add pause button 
    auto windowSize = window.getSize();
    sf::Texture pauseTex("assets/images/PauseButton.png");
    sf::Texture playTex("assets/images/PlayButton.png");
    sf::Sprite pauseButton(pauseTex);
    pauseButton.setScale({0.07, 0.07});
    pauseButton.setColor(sf::Color::White);
    // To store time paused button was clicked to add a click delay
    sf::Clock clickClock;
    sf::Time clickDelay = sf::milliseconds(300);

    // Game loop
    std::cout << "Starting game";
    while (window.isOpen()) {

        sf::Time delta = clock.restart(); // Time since last frame
        float deltaTime = delta.asSeconds(); // Convert to seconds

        // Handle events and updates
        window.handleEvents(onClose, onKeyPressed, onKeyReleased);
        if (!isPaused) {
            player->handleInput();
            for (auto& e : entities) e->update(deltaTime);
            pauseButton.setTexture(pauseTex);
        } else {
            pauseButton.setTexture(playTex);
        }

        // Create new window with black background
        window.clear(sf::Color::Black);

        if (!isPaused) {
            //---- Movement ----
            Vec2 originalPos = (player->playerCollider).pos;
            // X axis
            float originalX = (player->playerCollider).pos.x;
            (player->playerCollider).pos.x += player->movement.x;
            // Stop if colliding with an object that isn't the player and isn't a pickup
            for (auto& e : entities) { if (e->type() == EntityType::Player) continue; 
                if (e->type() == EntityType::Pickup) continue;
                Manifold m = { &(player->playerCollider), &e->collider() };
                if (AABBvsAABB(&m)) { (player->playerCollider).pos.x = originalX; break; }
            }
            for (auto& obj : colliders) { 
                Manifold m = { &(player->playerCollider), obj };
                if (AABBvsAABB(&m)) { (player->playerCollider).pos.x = originalX; break; }
            }
            // Y axis
            float originalY = (player->playerCollider).pos.y;
            (player->playerCollider).pos.y += player->movement.y;
            // Stop if colliding with object
            for (auto& e : entities) { if (e->type() == EntityType::Player) continue; 
                if (e->type() == EntityType::Pickup) continue;
                Manifold m = { &(player->playerCollider), &e->collider() };
                if (AABBvsAABB(&m)) { (player->playerCollider).pos.y = originalY; break; }
            }
            for (auto& obj : colliders) { Manifold m = { &(player->playerCollider), obj };
                if (AABBvsAABB(&m)) { (player->playerCollider).pos.y = originalY; break; }
            }

            //---- Damage ----
            // Check if enemy is attacking player, if close enough damage player
            for (auto& e : entities) {
                if (e->type() == EntityType::Enemy) {
                    float dx = player->getPosition().x - e->getPosition().x;
                    float dy = player->getPosition().y - e->getPosition().y;
                    float dist = std::sqrt(dx*dx + dy*dy);
                    if (e->isAttacking() && dist <= e->getAttackRadius()) {
                        player->takeDamage(1);
                    }
                }
            }

            //---- Pickups ----
            // Heal player when colliding with health pickups if not at max health
            if (player->getHealth() < player->getMaxHealth()) {
                handlePickup(healthPickups, (player->playerCollider), map, [&](auto& pickup) {
                    player->heal(1);
                    healthPickupSound.play();
                });
            }
            // Give gold when colliding with gold pickups
            handlePickup(goldPickups, (player->playerCollider), map, [&](auto& pickup) {
                player->addGold(25);
                goldPickupSound.play();
            });
        }

        //---- Draw items ----
        // Draw tilemap floors
        renderer.drawFloors(window, map);
        // Draw all shadows for entities (enemies and player)
        for (auto& e : entities) { e->drawShadow(window); }
        // Draw tilemap walls and items
        renderer.drawWalls(window, map);
        renderer.drawItems(window, map);
        // Sort by Y position before drawing, so that the highest objects are drawn first
        std::sort(entities.begin(), entities.end(),
        [](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
            return a->getPosition().y < b->getPosition().y;
        });
        // Draw and update all entity sprites
        for (auto& e : entities) { e->draw(window); }
        // Draw camera and UI
        player->drawUI(window, camera);
        camera.setCenter(player->getPosition());
        window.setView(camera);
        sf::Vector2f viewCenter = camera.getCenter();
        sf::FloatRect bounds = pauseButton.getGlobalBounds();
        pauseButton.setPosition({ // Center button at top of camera
            viewCenter.x - bounds.size.x / 2.f,
            viewCenter.y - bounds.size.y / 2.f - 368.f
        });
        window.draw(pauseButton);
        if (player->isDead()) {
        }
        // Delete dead entities
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::shared_ptr<Entity>& e) {
                    return e->isDead();
                }),
            entities.end()
        );

        // Display window
        window.display();

        // Check if pause button pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
            // Toggle pause if clicked on and click delay has passed
            if (pauseButton.getGlobalBounds().contains(worldPos)) {
                if (clickClock.getElapsedTime() > clickDelay) {
                    isPaused = !isPaused;
                    clickClock.restart(); // Restart delay
                }
            }
        }
    }
    return 0;
}
