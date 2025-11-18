#pragma once
#include "Enemy.hpp"
#include "Physics.hpp"
#include "PathFinder.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Constants.hpp"

Enemy::Enemy(sf::Vector2f startPosition, sf::Texture& texture)
    : texture(texture), // Store reference to texture
      sprite(texture)   // Initialize sprite with texture
    {

    // Size of textures in the spritesheet
    spriteSize = 300;
    // Colour tint of character
    baseColour = sf::Color::White;

    // Create a enemy sprite
    collider = { Vec2(startPosition.x, startPosition.y), { Vec2(-10, -10), Vec2(10, 10) } };
    sprite.setOrigin({spriteSize / 2.0f, spriteSize / 2.0f});
    sprite.setPosition(toSF(collider.pos));
    sprite.setTexture(texture);
    sprite.setTextureRect({{485,1}, {spriteSize,spriteSize}});
    sprite.setScale({0.8f,0.8f});
    sprite.setColor(sf::Color::Red);

    // Set health values
    maxHealth = 3;
    health = 3;

    // Default movement speed
    speed = 80.0f;
    moving = false;

    // Timer for animations and delays
    timer = 0.0f;
    timerMax = 0.5f;

    // When x coordinates have reached the final column
    finalColumn = 7800;
    finalRow = 1800;

    // Start and end coordinates for animation textures in spritesheet
    // These values will need to be changed if the spritesheet changes
    
    AttackEastXStart = 0; AttackEastYStart = 0; AttackEastXEnd = 2100; AttackEastYEnd = 0;
    AttackNorthXStart = 2400; AttackNorthYStart = 0; AttackNorthXEnd = 4500; AttackNorthYEnd = 0;
    AttackNorthEastXStart = 4800; AttackNorthEastYStart = 0; AttackNorthEastXEnd = 6600; AttackNorthEastYEnd = 0;
    AttackNorthWestXStart = 6900; AttackNorthWestYStart = 0; AttackNorthWestXEnd = 900; AttackNorthWestYEnd = 300;
    AttackSouthXStart = 1200; AttackSouthYStart = 300; AttackSouthXEnd = 3300; AttackSouthYEnd = 300;
    AttackSouthEastXStart = 3600; AttackSouthEastYStart = 300; AttackSouthEastXEnd = 5700; AttackSouthEastYEnd = 300;
    AttackSouthWestXStart = 6000; AttackSouthWestYStart = 300; AttackSouthWestXEnd = 0; AttackSouthWestYEnd = 600;
    AttackWestXStart = 300; AttackWestYStart = 600; AttackWestXEnd = 2400; AttackWestYEnd = 600;

    JogEastXStart = 2700; JogEastYStart = 600; JogEastXEnd = 4200; JogEastYEnd = 600;
    JogNorthXStart = 4500; JogNorthYStart = 600; JogNorthXEnd = 6000; JogNorthYEnd = 600;
    JogNorthEastXStart = 6300; JogNorthEastYStart = 600; JogNorthEastXEnd = 7800; JogNorthEastYEnd = 600;
    JogNorthWestXStart = 0; JogNorthWestYStart = 900; JogNorthWestXEnd = 1500; JogNorthWestYEnd = 900;
    JogSouthXStart = 1800; JogSouthYStart = 900; JogSouthXEnd = 3300; JogSouthYEnd = 900;
    JogSouthEastXStart = 3600; JogSouthEastYStart = 900; JogSouthEastXEnd = 5100; JogSouthEastYEnd = 900;
    JogSouthWestXStart = 5400; JogSouthWestYStart = 900; JogSouthWestXEnd = 6900; JogSouthWestYEnd = 900;
    JogWestXStart = 7200; JogWestYStart = 900; JogWestXEnd = 600; JogWestYEnd = 1200;

    StandingEastX = 7200; StandingEastY = 1500;
    StandingNorthX = 7500; StandingNorthY = 1500;
    StandingNorthEastX = 7800; StandingNorthEastY = 1500;
    StandingNorthWestX = 0; StandingNorthWestY = 1800;
    StandingSouthX = 300; StandingSouthY = 1800;
    StandingSouthEastX = 600; StandingSouthEastY = 1800;
    StandingSouthWestX = 900; StandingSouthWestY = 1800;
    StandingWestX = 1200; StandingWestY = 1800;

    // Coordinates for current texture in spritesheet
    textureX = StandingSouthX;
    textureY = StandingSouthY;
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

void Enemy::update(float deltaTime, Grid& grid, const sf::Vector2f& playerPosition, bool playerAttacking) {
    sf::Vector2f position = sprite.getPosition();
    float dx = playerPosition.x - position.x;
    float dy = playerPosition.y - position.y;
    float distance = std::sqrt(dx*dx + dy*dy);

    float stopRadius   = 500.0f;
    attackRadius = 100.0f;

    moving = false;
    attacking = false;

    // A* pathfinding, chase player, if not too close or far away
    if (distance > attackRadius && distance < stopRadius) {
        // Convert world into grid
        int enemyGX  = static_cast<int>(collider.pos.x) / TILE_SIZE;
        int enemyGY  = static_cast<int>(collider.pos.y) / TILE_SIZE;
        int playerGX = static_cast<int>(playerPosition.x) / TILE_SIZE;
        int playerGY = static_cast<int>(playerPosition.y) / TILE_SIZE;
        if (grid.inBounds(enemyGX, enemyGY) && grid.inBounds(playerGX, playerGY)) {
            Node* start = &grid.nodes[enemyGY][enemyGX];
            Node* goal  = &grid.nodes[playerGY][playerGX];

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
                    collider.pos.x += movement.x;
                    collider.pos.y += movement.y;
                    moving = true;
                    direction = dir;
                }
            }
        }
    }
    // Attack if too close to player
    else if (distance <= attackRadius * 1.5f && playerAttacking) {
        takeDamage(1);
    }
    else if (distance <= attackRadius) {
        attacking = true;
    }

    // Calculate current direction
    std::string currentDirection;
    float absX = std::abs(direction.x);
    float absY = std::abs(direction.y);
    const float diagonalThreshold = 0.25f;
    
    if (absX < diagonalThreshold && absY < diagonalThreshold) {
        // Not moving
        currentDirection = "idle";
    } else if (std::abs(absX - absY) < diagonalThreshold) {
        // Diagonal
        if (direction.x > 0 && direction.y < 0) currentDirection = "northEast";
        else if (direction.x < 0 && direction.y < 0) currentDirection = "northWest";
        else if (direction.x > 0 && direction.y > 0) currentDirection = "southEast";
        else if (direction.x < 0 && direction.y > 0) currentDirection = "southWest";
    } else if (absX > absY) {
        // Four directional
        currentDirection = (direction.x > 0) ? "east" : "west";
    } else {
        currentDirection = (direction.y > 0) ? "south" : "north";
    }

    // Get the correct animation based on the direction
    timer += 0.08f;
    if (timer >= timerMax) {
        textureX += spriteSize;
        // Check direction and whether enemy is moving or attacking or standing still
        if (currentDirection == "north") {
            if (moving) {animate(JogNorthXStart, JogNorthXEnd, JogNorthYStart, JogNorthYEnd);}
            else if (attacking) {animate(AttackNorthXStart, AttackNorthXEnd, AttackNorthYStart, AttackNorthYEnd);}
            else {sprite.setTextureRect({{StandingNorthX, StandingNorthY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "northEast") {
            if (moving) {animate(JogNorthEastXStart, JogNorthEastXEnd, JogNorthEastYStart, JogNorthEastYEnd);}
            else if (attacking) {animate(AttackNorthEastXStart, AttackNorthEastXEnd, AttackNorthEastYStart, AttackNorthEastYEnd);}
            else {sprite.setTextureRect({{StandingNorthEastX, StandingNorthEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "east") {
            if (moving) {animate(JogEastXStart, JogEastXEnd, JogEastYStart, JogEastYEnd);}
            else if (attacking) {animate(AttackEastXStart, AttackEastXEnd, AttackEastYStart, AttackEastYEnd);}
            else {sprite.setTextureRect({{StandingEastX, StandingEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "southEast") {
            if (moving) {animate(JogSouthEastXStart, JogSouthEastXEnd, JogSouthEastYStart, JogSouthEastYEnd);}
            else if (attacking) {animate(AttackSouthEastXStart, AttackSouthEastXEnd, AttackSouthEastYStart, AttackSouthEastYEnd);}
            else {sprite.setTextureRect({{StandingSouthEastX, StandingSouthEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "south") {
            if (moving) {animate(JogSouthXStart, JogSouthXEnd, JogSouthYStart, JogSouthYEnd);}
            else if (attacking) {animate(AttackSouthXStart, AttackSouthXEnd, AttackSouthYStart, AttackSouthYEnd);}
            else {sprite.setTextureRect({{StandingSouthX, StandingSouthY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "southWest") {
            if (moving) {animate(JogSouthWestXStart, JogSouthWestXEnd, JogSouthWestYStart, JogSouthWestYEnd);}
            else if (attacking) {animate(AttackSouthWestXStart, AttackSouthWestXEnd, AttackSouthWestYStart, AttackSouthWestYEnd);}
            else {sprite.setTextureRect({{StandingSouthWestX, StandingSouthWestY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "west") {
            if (moving) {animate(JogWestXStart, JogWestXEnd, JogWestYStart, JogWestYEnd);}
            else if (attacking) {animate(AttackWestXStart, AttackWestXEnd, AttackWestYStart, AttackWestYEnd);}
            else {sprite.setTextureRect({{StandingWestX, StandingWestY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "northWest") {
            if (moving) {animate(JogNorthWestXStart, JogNorthWestXEnd, JogNorthWestYStart, JogNorthWestYEnd);}
            else if (attacking) {animate(AttackNorthWestXStart, AttackNorthWestXEnd, AttackNorthWestYStart, AttackNorthWestYEnd);}
            else {sprite.setTextureRect({{StandingNorthWestX, StandingNorthWestY}, {spriteSize, spriteSize}});}
        } else {
            sprite.setTextureRect({{StandingSouthX, StandingSouthY}, {spriteSize, spriteSize}});
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

sf::Vector2f Enemy::getPosition() {
    return sprite.getPosition();
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}