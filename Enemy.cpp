#pragma once
#include "Enemy.hpp"
#include "Player.hpp"
#include "Physics.hpp"
#include "PathFinder.hpp"
#include "Entity.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Constants.hpp"

Enemy::Enemy(sf::Vector2f startPosition, sf::Texture& texture, Grid& grid, Player& player)
: texture(texture), // Store reference to texture
    sprite(texture), // Initialize sprite with texture
    grid(grid), // Store reference to grid
    player(player) // Store reference to player
{

    // Size of textures in the spritesheet
    spriteSize = ENEMY_SPRITE_SIZE;
    // Colour tint of character
    baseColour = ENEMY_BASE_COLOUR;

    // Create a enemy sprite
    enemyCollider = { Vec2(startPosition.x, startPosition.y), { Vec2(-10, -10), Vec2(10, 10) } };
    sprite.setOrigin({spriteSize / 2.0f, (spriteSize + spriteSize / 2) / 2.0f});
    sprite.setPosition(toSF(enemyCollider.pos));
    sprite.setTexture(texture);
    sprite.setTextureRect({{485,1}, {spriteSize,spriteSize}});
    sprite.setScale({0.8f,0.8f});
    sprite.setColor(ENEMY_BASE_COLOUR);

    spawnPoint = toSF(enemyCollider.pos);

    // Set health
    maxHealth = ENEMY_MAX_HEALTH;
    health = ENEMY_MAX_HEALTH;

    // Default movement speed
    speed = ENEMY_SPEED;
    moving = false;

    // Timer for animations and delays
    timer = ENEMY_TIMER;
    timerMax = ENEMY_TIMER_MAX;

    // When x coordinates have reached the final column
    finalColumn = ENEMY_FINAL_COLUMN;
    finalRow = ENEMY_FINAL_ROW;

    // Coordinates for current texture in spritesheet
    auto coords = enemyAnimationTable.at({Action::Standing, Direction::South});
    textureX = coords.xStart;
    textureY = coords.yStart;

    randomPoint = getRandomPatrolPoint(40.f);
}

// Choose a random point from the radius around the enemy
sf::Vector2f Enemy::getRandomPatrolPoint(float radius) {
    // Try 10 times to find a random valid target
    for (int attempts = 0; attempts < 10; ++attempts) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.14159265358979323846;
        float dist  = static_cast<float>(rand()) / RAND_MAX * radius;
        sf::Vector2f target = spawnPoint + sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        int gx = static_cast<int>(target.x) / TILE_SIZE;
        int gy = static_cast<int>(target.y) / TILE_SIZE;

        if (grid.inBounds(gx, gy)) {
            return target;
        }
    }
    // Return spawn point if no valid patrol point can be found
    return spawnPoint;
}

// Take damage until no health left, with half a second delay between damage
void Enemy::takeDamage(int amount) {
        if (damageClock.getElapsedTime() >= damageCooldown) {
            health -= amount;
            damageClock.restart();
        }
    }

// Heal enemy until max health
void Enemy::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

// Reusable function for animating enemy
void Enemy::animate(int xStart, int xEnd, int yStart, int yEnd) {
    // If current texture coordinates outside of expected values then use start coordinates
    if(textureY == yStart) {if(textureX < xStart) {textureX = xStart; textureY = yStart;}} // If column is before start of anim
    if(textureY == yEnd) {if(textureX > xEnd) {textureX = xStart; textureY = yStart;}} // If column is past end of anim
    if(textureY < yStart || textureY > yEnd) {textureX = xStart; textureY = yStart;} // If row is before or after anim
    if(textureY != yStart && textureY != yEnd) {textureX = xStart; textureY = yStart;} // Row value is invalid

    // If x is at final column, switch to next row
    if (textureX > finalColumn) { textureX = 0; textureY += spriteSize;}
    // If y is at final row, then use start coordinates
    if (textureY > finalRow) {textureX = xStart; textureY = yStart;}

    // If x value is valid, then change current sprite texture coordinates
    if (textureX <= finalColumn) {
        sprite.setTextureRect({{textureX, textureY}, {spriteSize, spriteSize}});
    }
}

void Enemy::update(float deltaTime)
{
    auto playerPosition = player.getPosition();
    bool playerAttacking = player.attacking;

    sf::Vector2f position = sprite.getPosition();
    float dx = playerPosition.x - position.x;
    float dy = playerPosition.y - position.y;
    float playerDistance = std::sqrt(dx*dx + dy*dy);

    float stopRadius   = 500.0f;
    attackRadius = 80.0f;

    moving = false;
    attacking = false;

    int enemyGX;
    int enemyGY;
    int targetGX;
    int targetGY;
    Node* start;
    Node* goal;

    // ---- Movement and pathfinding ----

    // Chase player, if they are not too close or far away
    if (playerDistance > attackRadius && playerDistance < stopRadius) {
        targetPosition = playerPosition;
    } 
    // Attack if too close to player
    else if (playerDistance <= attackRadius) {
        attacking = true;
        targetPosition = std::nullopt;
    } 
    // Wander randomly around spawn if player is far away
    else if (playerDistance > stopRadius) {
        // If we don't have a target, pick random one
        if (!targetPosition) {
            randomPoint = getRandomPatrolPoint(300.f);
            targetPosition = randomPoint;
        }

        // Check if we've reached the target
        sf::Vector2f dir = *targetPosition - sprite.getPosition();
        float dist = std::sqrt(dir.x*dir.x + dir.y*dir.y);

        // Pause before picking a new point
        idleTimer += 0.1f;
        if (idleTimer > 20.0f) {
            randomPoint = getRandomPatrolPoint(500.f);
            targetPosition = randomPoint;
            idleTimer = 0.f;
        }
    }

    // Take damage if being attacked when close to player
    if (playerDistance <= attackRadius * 1.5f && playerAttacking) {
        takeDamage(1);
    }

    if (targetPosition) {
        // Convert world into grid
        enemyGX  = static_cast<int>(enemyCollider.pos.x) / TILE_SIZE;
        enemyGY  = static_cast<int>(enemyCollider.pos.y) / TILE_SIZE;
        targetGX = static_cast<int>(targetPosition->x) / TILE_SIZE;
        targetGY = static_cast<int>(targetPosition->y) / TILE_SIZE;
        if (grid.inBounds(enemyGX, enemyGY) && grid.inBounds(targetGX, targetGY)) {
            start = &grid.nodes[enemyGY][enemyGX];
            goal  = &grid.nodes[targetGY][targetGX];

            // Reset node costs
            for (auto& row : grid.nodes) {
                for (auto& n : row) {
                    n.visited = false;
                    n.previousNode = nullptr;
                    n.g = std::numeric_limits<float>::infinity();
                    n.h = 0;
                }
            }

            // Begin pathfinding
            std::vector<Node*> path = aStar(start, goal, grid, 2, 2);
            if (path.size() > 1) {
                // Move toward next step
                sf::Vector2f nextPos(
                    path[1]->x * TILE_SIZE + TILE_SIZE / 2,
                    path[1]->y * TILE_SIZE + TILE_SIZE / 2
                );
                sf::Vector2f dir = nextPos - position;
                float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
                if (len > 0.1f) {
                    dir /= len;
                    sf::Vector2f movement = dir * speed * deltaTime;
                    sprite.move(movement);
                    enemyCollider.pos.x += movement.x;
                    enemyCollider.pos.y += movement.y;
                    moving = true;
                    direction = dir;
                }
            }
        }
    }

    // Sync sprite with collider
    sprite.setPosition(toSF(enemyCollider.pos));

    // ---- Animation ----

    // Calculate current direction
    float absX = std::abs(direction.x);
    float absY = std::abs(direction.y);
    const float diagonalThreshold = 0.25f;
    
    if (absX < diagonalThreshold && absY < diagonalThreshold) {
    } else if (std::abs(absX - absY) < diagonalThreshold) {
        // Diagonal
        if (direction.x > 0 && direction.y < 0) currentDirection = Direction::NorthEast;
        else if (direction.x < 0 && direction.y < 0) currentDirection = Direction::NorthWest;
        else if (direction.x > 0 && direction.y > 0) currentDirection = Direction::SouthEast;
        else if (direction.x < 0 && direction.y > 0) currentDirection = Direction::SouthWest;
    } else if (absX > absY) {
        // Four directional
        currentDirection = (direction.x > 0) ? Direction::East : Direction::West;
    } else {
        currentDirection = (direction.y > 0) ? Direction::South : Direction::North;
    }

    // Get the correct animation based on the direction
    timer += 0.08f;
    if (timer >= timerMax) {
        textureX += spriteSize;
        // Check direction and whether enemy is moving or attacking or standing still
        if (moving) {
            auto coords = enemyAnimationTable.at({Action::Jog, currentDirection});
            animate(coords.xStart, coords.xEnd, coords.yStart, coords.yEnd);
        } else if (attacking) {
            auto coords = enemyAnimationTable.at({Action::Attack, currentDirection});
            animate(coords.xStart, coords.xEnd, coords.yStart, coords.yEnd);
        } else {
            auto coords = playerAnimationTable.at({Action::Standing, currentDirection});
            textureX = coords.xStart; textureY = coords.yStart;
            sprite.setTextureRect({{textureX, textureY}, {spriteSize, spriteSize}});
        }
        timer = 0.0f;
    }

    // Indicate damage by turning sprite red
    if (damageClock.getElapsedTime() >= damageCooldown - sf::seconds(0.3f)) {
        sprite.setColor(baseColour);
    } else {
        sprite.setColor(sf::Color::Red);
    }
}

// For drawing enemy in main

// Draw shadow seperately
void Enemy::drawShadow(sf::RenderWindow& window) {
    // Add shadow under character
    sf::CircleShape shadow(spriteSize / 5.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    shadow.setOrigin({spriteSize / 2.0f, (spriteSize + spriteSize / 2) / 2.0f});
    shadow.setScale({1.0f, 0.4f});
    sf::Vector2f position = sprite.getPosition();
    shadow.setPosition({position.x + spriteSize / 3.18f, position.y + spriteSize / 2.6f});
    window.draw(shadow);
}

// Draw enemy sprite
void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}