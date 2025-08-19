#pragma once
#include "Enemy.hpp"
#include "Physics.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

Enemy::Enemy(sf::Vector2f startPosition, sf::Texture& texture)
    : texture(texture), // Store reference to texture
      sprite(texture)   // Initialize sprite with texture
    {

    // Create a enemy sprite
    collider = { Vec2(startPosition.x, startPosition.y), { Vec2(-10, -10), Vec2(10, 10) } };
    sprite.setOrigin({600.0f / 2.0f, 750.0f / 2.0f});
    sprite.setPosition(toSF(collider.pos));
    sprite.setTexture(texture);
    sprite.setTextureRect({{485,1}, {spriteSize,spriteSize}});
    sprite.setScale({0.4f,0.4f});
    sprite.setColor(sf::Color::White);

    // Set health values
    maxHealth = 3;
    health = 3;

    // Default movement speed
    speed = 80.0f;
    moving = false;

    // Coordinates for current texture in spritesheet
    textureX = 0;
    textureY = 0;

    // Timer for animations and delays
    timer = 0.0f;
    timerMax = 0.5f;

    // Size of textures in the spritesheet
    spriteSize = 600;

    // When x coordinates have reached the final column
    finalColumn = 7200;
    finalRow = 2400;

    // Start and end coordinates for animation textures in spritesheet
    // These values will need to be changed if the spritesheet changes

    standingEastX = 0; standingEastY = 0;
    standingNorthX = 600; standingNorthY = 0;
    standingNorthEastX = 1200; standingNorthEastY = 0;
    standingNorthWestX = 1800; standingNorthWestY = 0;
    standingSouthX = 2400; standingSouthY = 0;
    standingSouthEastX = 3000; standingSouthEastY = 0;
    standingSouthWestX = 3600; standingSouthWestY = 0;
    standingWestX = 4200; standingWestY = 0;

    JogEastXStart = 4800; JogEastYStart = 0; JogEastXEnd = 0; JogEastYEnd = 600;
    JogNorthXStart = 600; JogNorthYStart = 600; JogNorthXEnd = 3600; JogNorthYEnd = 600;
    JogNorthEastXStart = 4200; JogNorthEastYStart = 600; JogNorthEastXEnd = 7200; JogNorthEastYEnd = 600;
    JogNorthWestXStart = 0; JogNorthWestYStart = 1200; JogNorthWestXEnd = 3000; JogNorthWestYEnd = 1200;
    JogSouthXStart = 3600; JogSouthYStart = 1200; JogSouthXEnd = 6600; JogSouthYEnd = 1200;
    JogSouthEastXStart = 7200; JogSouthEastYStart = 1200; JogSouthEastXEnd = 2400; JogSouthEastYEnd = 1800;
    JogSouthWestXStart = 3000; JogSouthWestYStart = 1800; JogSouthWestXEnd = 6000; JogSouthWestYEnd = 1800;
    JogWestXStart = 6600; JogWestYStart = 1800; JogWestXEnd = 1800; JogWestYEnd = 2400;

    attacking = false;
    attackEastXStart = 1680; attackEastYStart = 1200; attackEastXEnd = 0; attackEastYEnd = 1440;
    attackNorthXStart = 240; attackNorthYStart = 1440; attackNorthXEnd = 1680; attackNorthYEnd = 1440;
    attackNorthEastXStart = 1920; attackNorthEastYStart = 1440; attackNorthEastXEnd = 240; attackNorthEastYEnd = 1680;
    attackNorthWestXStart = 480; attackNorthWestYStart = 1680; attackNorthWestXEnd = 1920; attackNorthWestYEnd = 1680;
    attackSouthXStart = 2160; attackSouthYStart = 1680; attackSouthXEnd = 480; attackSouthYEnd = 1920;
    attackSouthEastXStart = 720; attackSouthEastYStart = 1920; attackSouthEastXEnd = 2160; attackSouthEastYEnd = 1920;
    attackSouthWestXStart = 2400; attackSouthWestYStart = 1920; attackSouthWestXEnd = 720; attackSouthWestYEnd = 2160;
    attackWestXStart = 960; attackWestYStart = 2160; attackWestXEnd = 2400; attackWestYEnd = 2160;
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

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition, bool playerAttacking) {
    // Chase player
    sf::Vector2f position = sprite.getPosition();
    sf::Vector2f direction = playerPosition - position; // Calculate direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float distance = std::sqrt(std::pow(playerPosition.x - position.x, 2) + std::pow(playerPosition.y - position.y, 2)); // Calculate distance from player
    float stopRadius = 500.0f; // When to stop chasing
    float attackRadius = 60.0f; // When to attack
    // Chase player, if not too close or far away
    if (distance > attackRadius && distance < stopRadius) {
        if (length != 0.f) { // Avoid division by zero
            moving = true;
            attacking = false;
            sf::Vector2f normalized = direction / length;
            sf::Vector2f movement = (normalized * speed * deltaTime);
            sprite.move(movement); // Move enemy sprite
            collider.pos.x += movement.x;
            collider.pos.y += movement.y;
            direction = normalized; // Set direction
        }
    }
    else if (distance <= attackRadius * 1.5 && playerAttacking) {
        Enemy::takeDamage(1);
    // Stop if too close to player
    } else if (distance <= attackRadius) {
        attacking = true;
        moving = false;
    // Stop if too far away from player
    } else if (distance >= stopRadius) {
        attacking = false;
        moving = false;
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
            else if (attacking) {animate(attackNorthXStart, attackNorthXEnd, attackNorthYStart, attackNorthYEnd);}
            else {sprite.setTextureRect({{standingNorthX, standingNorthY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "northEast") {
            if (moving) {animate(JogNorthEastXStart, JogNorthEastXEnd, JogNorthEastYStart, JogNorthEastYEnd);}
            else if (attacking) {animate(attackNorthEastXStart, attackNorthEastXEnd, attackNorthEastYStart, attackNorthEastYEnd);}
            else {sprite.setTextureRect({{standingNorthEastX, standingNorthEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "east") {
            if (moving) {animate(JogEastXStart, JogEastXEnd, JogEastYStart, JogEastYEnd);}
            else if (attacking) {animate(attackEastXStart, attackEastXEnd, attackEastYStart, attackEastYEnd);}
            else {sprite.setTextureRect({{standingEastX, standingEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "southEast") {
            if (moving) {animate(JogSouthEastXStart, JogSouthEastXEnd, JogSouthEastYStart, JogSouthEastYEnd);}
            else if (attacking) {animate(attackSouthEastXStart, attackSouthEastXEnd, attackSouthEastYStart, attackSouthEastYEnd);}
            else {sprite.setTextureRect({{standingSouthEastX, standingSouthEastY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "south") {
            if (moving) {animate(JogSouthXStart, JogSouthXEnd, JogSouthYStart, JogSouthYEnd);}
            else if (attacking) {animate(attackSouthXStart, attackSouthXEnd, attackSouthYStart, attackSouthYEnd);}
            else {sprite.setTextureRect({{standingSouthX, standingSouthY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "southWest") {
            if (moving) {animate(JogSouthWestXStart, JogSouthWestXEnd, JogSouthWestYStart, JogSouthWestYEnd);}
            else if (attacking) {animate(attackSouthWestXStart, attackSouthWestXEnd, attackSouthWestYStart, attackSouthWestYEnd);}
            else {sprite.setTextureRect({{standingSouthWestX, standingSouthWestY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "west") {
            if (moving) {animate(JogWestXStart, JogWestXEnd, JogWestYStart, JogWestYEnd);}
            else if (attacking) {animate(attackWestXStart, attackWestXEnd, attackWestYStart, attackWestYEnd);}
            else {sprite.setTextureRect({{standingWestX, standingWestY}, {spriteSize, spriteSize}});}
        } else if (currentDirection == "northWest") {
            if (moving) {animate(JogNorthWestXStart, JogNorthWestXEnd, JogNorthWestYStart, JogNorthWestYEnd);}
            else if (attacking) {animate(attackNorthWestXStart, attackNorthWestXEnd, attackNorthWestYStart, attackNorthWestYEnd);}
            else {sprite.setTextureRect({{standingNorthWestX, standingNorthWestY}, {spriteSize, spriteSize}});}
        } else {
            sprite.setTextureRect({{standingSouthX, standingSouthY}, {spriteSize, spriteSize}});
        }
        timer = 0.0f;
    }

    // Indicate damage by turning sprite red
    if (damageClock.getElapsedTime() >= damageCooldown - sf::seconds(0.3f)) {
        sprite.setColor(sf::Color::White);
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