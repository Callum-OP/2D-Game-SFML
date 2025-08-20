#include <iostream>

#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

Player::Player()
    : texture{},
        sprite( // Create a player sprite
            [this]() {
              if (!texture.loadFromFile("playerSpritesheet.png"))
                  throw std::runtime_error("Failed to load sprite sheet");
              return sf::Sprite(texture);
            }()
        ),
        heartSprite( // Create a player sprite
            [this]() {
              if (!heartFull.loadFromFile("HeartFull.png"))
                  throw std::runtime_error("Failed to load health textures");
              return sf::Sprite(heartFull);
            }()
        )
    {
    // Set up sprite
    sprite.setTexture(texture);
    sprite.setTextureRect({{485,1}, {240,240}});
    sprite.setScale({1.0f,1.0f});
    //sprite.setColor(sf::Color::White);

    // Health textures
    heartSprite.setScale({0.2f, 0.2f});
    if (!heartFull.loadFromFile("HeartFull.png") ||
        !heartEmpty.loadFromFile("HeartEmpty.png")) {
        throw std::runtime_error("Failed to load health textures");
    }

    // Set health values
    maxHealth = 5;
    health = 5;

    // Set gold
    gold = 0;

    // Default movement speed
    verticalSpeed = 3.0f;
    horizontalSpeed = 3.0f;
    movement = {horizontalSpeed, 0};
    moving = false;

    // Coordinates for current texture in spritesheet
    textureX = 0;
    textureY = 0;

    // Timer for animations and delays
    timer = 0.0f;
    timerMax = 0.5f;

    // When x coordinates have reached the final column
    finalColumn = 3840;
    finalRow = 2400;

    // Start and end coordinates for animation textures in spritesheet
    // These values will need to be changed if the spritesheet changes

    standingEastX = 0; standingEastY = 0;
    standingNorthX = 240; standingNorthY = 0;
    standingNorthEastX = 480; standingNorthEastY = 0;
    standingNorthWestX = 720; standingNorthWestY = 0;
    standingSouthX = 960; standingSouthY = 0;
    standingSouthEastX = 1200; standingSouthEastY = 0;
    standingSouthWestX = 1440; standingSouthWestY = 0;
    standingWestX = 1680; standingWestY = 0;

    JogEastXStart = 1920; JogEastYStart = 0; JogEastXEnd = 3120; JogEastYEnd = 0;
    JogNorthXStart = 3360; JogNorthYStart = 0; JogNorthXEnd = 480; JogNorthYEnd = 240;
    JogNorthEastXStart = 720; JogNorthEastYStart = 240; JogNorthEastXEnd = 1920; JogNorthEastYEnd = 240;
    JogNorthWestXStart = 2160; JogNorthWestYStart = 240; JogNorthWestXEnd = 3360; JogNorthWestYEnd = 240;
    JogSouthXStart = 3600; JogSouthYStart = 240; JogSouthXEnd = 720; JogSouthYEnd = 480;
    JogSouthEastXStart = 960; JogSouthEastYStart = 480; JogSouthEastXEnd = 2160; JogSouthEastYEnd = 480;
    JogSouthWestXStart = 2400; JogSouthWestYStart = 480; JogSouthWestXEnd = 3600; JogSouthWestYEnd = 480;
    JogWestXStart = 3840; JogWestYStart = 480; JogWestXEnd = 960; JogWestYEnd = 720;

    SlashEastXStart = 960; SlashEastYStart = 960; SlashEastXEnd = 2400; SlashEastYEnd = 960;
    SlashNorthXStart = 2640; SlashNorthYStart = 960; SlashNorthXEnd = 0; SlashNorthYEnd = 1200;
    SlashNorthEastXStart = 240; SlashNorthEastYStart = 1200; SlashNorthEastXEnd = 1680; SlashNorthEastYEnd = 1200;
    SlashNorthWestXStart = 1920; SlashNorthWestYStart = 1200; SlashNorthWestXEnd = 3360; SlashNorthWestYEnd = 1200;
    SlashSouthXStart = 3600; SlashSouthYStart = 1200; SlashSouthXEnd = 960; SlashSouthYEnd = 1440;
    SlashSouthEastXStart = 1200; SlashSouthEastYStart = 1440; SlashSouthEastXEnd = 2640; SlashSouthEastYEnd = 1440;
    SlashSouthWestXStart = 2880; SlashSouthWestYStart = 1440; SlashSouthWestXEnd = 240; SlashSouthWestYEnd = 1680;
    SlashWestXStart = 480; SlashWestYStart = 1680; SlashWestXEnd = 1920; SlashWestYEnd = 1680;

    SprintEastXStart = 2160; SprintEastYStart = 1680; SprintEastXEnd = 3360; SprintEastYEnd = 1680;
    SprintNorthXStart = 3600; SprintNorthYStart = 1680; SprintNorthXEnd = 720; SprintNorthYEnd = 1920;
    SprintNorthEastXStart = 960; SprintNorthEastYStart = 1920; SprintNorthEastXEnd = 2160; SprintNorthEastYEnd = 1920;
    SprintNorthWestXStart = 2400; SprintNorthWestYStart = 1920; SprintNorthWestXEnd = 3600; SprintNorthWestYEnd = 1920;
    SprintSouthXStart = 3840; SprintSouthYStart = 1920; SprintSouthXEnd = 960; SprintSouthYEnd = 2160;
    SprintSouthEastXStart = 1200; SprintSouthEastYStart = 2160; SprintSouthEastXEnd = 2400; SprintSouthEastYEnd = 2160;
    SprintSouthWestXStart = 2640; SprintSouthWestYStart = 2160; SprintSouthWestXEnd = 3840; SprintSouthWestYEnd = 2160;
    SprintWestXStart = 0; SprintWestYStart = 2400; SprintWestXEnd = 1200; SprintWestYEnd = 2400;
}

// Take damage until no health left, with a 1 second delay between damage
void Player::takeDamage(int amount) {
        if (damageClock.getElapsedTime() >= damageCooldown) {
            health -= amount;
            damageClock.restart();
        }
    }

// Heal player until max health
void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::addGold(int amount) {
    gold += amount;
}

bool Player::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

void Player::setGold(int amount) {
    gold = amount;
}


void Player::handleInput() {
    // If both right and up key pressed then move character right and up at same time
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; northEast = true;
        north = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        movement = {horizontalSpeed / 1.5f, -verticalSpeed / 1.5f};
        // Waits for set amount of time then plays Jog north east animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed / 1.5f * 4, -verticalSpeed / 1.5f * 4}; sprite.move(movement); textureX += 240; animate(SprintNorthEastXStart, SprintNorthEastXEnd, 
            SprintNorthEastYStart, SprintNorthEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogNorthEastXStart, JogNorthEastXEnd, 
            JogNorthEastYStart, JogNorthEastYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If both right and down key pressed then move character right and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; southEast = true;
        north = false; northEast = false; east = false; south = false; southWest = false; west = false; northWest = false;
        movement = {horizontalSpeed / 1.5f, verticalSpeed / 1.5f};
        // Waits for set amount of time then plays Jog south east animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed / 1.5f * 4, verticalSpeed / 1.5f * 4}; textureX += 240; animate(SprintSouthEastXStart, SprintSouthEastXEnd, 
            SprintSouthEastYStart, SprintSouthEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogSouthEastXStart, JogSouthEastXEnd, 
            JogSouthEastYStart, JogSouthEastYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If both left and up key pressed then move character left and up at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; northWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; west = false;
        movement = {-horizontalSpeed / 1.5f, -verticalSpeed / 1.5f};
        // Waits for set amount of time then plays Jog north west animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed / 1.5f * 4, -verticalSpeed / 1.5f * 4}; textureX += 240; animate(SprintNorthWestXStart, SprintNorthWestXEnd, 
            SprintNorthWestYStart, SprintNorthWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogNorthWestXStart, JogNorthWestXEnd, 
            JogNorthWestYStart, JogNorthWestYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If both left and down key pressed then move character left and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; southWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; west = false; northWest = false;
        movement = {-horizontalSpeed / 1.5f, verticalSpeed / 1.5f};
        // Waits for set amount of time then plays Jog south west animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed / 1.5f * 4, verticalSpeed / 1.5f * 4}; textureX += 240; animate(SprintSouthWestXStart, SprintSouthWestXEnd, 
            SprintSouthWestYStart, SprintSouthWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogSouthWestXStart, JogSouthWestXEnd, 
            JogSouthWestYStart, JogSouthWestYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If right key pressed then move character right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        moving = true; east = true;
        north = false; northEast = false; southEast = false; south = false ;southWest = false; west = false; northWest = false;
        movement = {horizontalSpeed, 0.0f};
        // Waits for set amount of time then plays Jog east animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed * 4, 0.0f}; textureX += 240; animate(SprintEastXStart, SprintEastXEnd, 
            SprintEastYStart, SprintEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogEastXStart, JogEastXEnd, 
            JogEastYStart, JogEastYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If up key pressed then move character up
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; north = true;
        northEast = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        movement = {0.0f, -verticalSpeed};
        // Waits for set amount of time then plays Jog north animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {0.0f, -verticalSpeed * 4}; textureX += 240; animate(SprintNorthXStart, SprintNorthXEnd, 
            SprintNorthYStart, SprintNorthYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogNorthXStart, JogNorthXEnd, 
            JogNorthYStart, JogNorthYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If down key pressed then move character down
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; south = true;
        north = false; northEast = false; east = false; southEast = false; southWest = false; west = false; northWest = false;
        movement = {0.0f, verticalSpeed};
        // Waits for set amount of time then plays Jog south animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {0.0f, verticalSpeed * 4}; textureX += 240; animate(SprintSouthXStart, SprintSouthXEnd, 
            SprintSouthYStart, SprintSouthYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogSouthXStart, JogSouthXEnd, 
            JogSouthYStart, JogSouthYEnd); timer = 0.0f;}
        sprite.move({movement});
    }
    // If left key pressed then move character left
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        moving = true; west = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; northWest = false;
        movement = {-horizontalSpeed, 0.0f};
        // Waits for set amount of time then plays Jog west animation
        timer += 0.08f;
        // Sprinting animation
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed * 4, 0.0f}; textureX += 240; animate(SprintWestXStart, SprintWestXEnd, 
            SprintWestYStart, SprintWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += 240; animate(JogWestXStart, JogWestXEnd, 
            JogWestYStart, JogWestYEnd); timer = 0.0f;}
        sprite.move({movement});
    } else {
        movement = {0.0f, 0.0f};
        moving = false;
    }
    // If space pressed then attack and change to attack animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
        attacking = true;
        timer += 0.08f;
        // Check direction user is facing
        if (north) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashNorthXStart, SlashNorthXEnd, 
                SlashNorthYStart, SlashNorthYEnd); timer = 0.0f;}
        } else if (northEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashNorthEastXStart, SlashNorthEastXEnd, 
                SlashNorthEastYStart, SlashNorthEastYEnd); timer = 0.0f;}
        } else if (east) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashEastXStart, SlashEastXEnd, 
                SlashEastYStart, SlashEastYEnd); timer = 0.0f;}
        } else if (southEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashSouthEastXStart, SlashSouthEastXEnd, 
                SlashSouthEastYStart, SlashSouthEastYEnd); timer = 0.0f;}
        } else if (south) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashSouthXStart, SlashSouthXEnd, 
                SlashSouthYStart, SlashSouthYEnd); timer = 0.0f;}
        } else if (southWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashSouthWestXStart, SlashSouthWestXEnd, 
                SlashSouthWestYStart, SlashSouthWestYEnd); timer = 0.0f;}
        } else if (west) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashWestXStart, SlashWestXEnd, 
                SlashWestYStart, SlashWestYEnd); timer = 0.0f;}
        } else if (northWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(SlashNorthWestXStart, SlashNorthWestXEnd, 
                SlashNorthWestYStart, SlashNorthWestYEnd); timer = 0.0f;}
        }
    } else {
        attacking = false;
    }
}


// Reusable function for animating player
void Player::animate(int xStart, int xEnd, int yStart, int yEnd) {
    // If current texture coordinates outside of expected values then use start coordinates
    if(textureY == yStart) {if(textureX < xStart) {textureX = xStart; textureY = yStart;}} // If column is before start of anim
    if(textureY == yEnd) {if(textureX > xEnd) {textureX = xStart; textureY = yStart;}} // If column is past end of anim
    if(textureY < yStart || textureY > yEnd) {textureX = xStart; textureY = yStart;} // If row is before or after anim
    if(textureY != yStart && textureY != yEnd) {textureX = xStart; textureY = yStart;} // Row value is invalid

    // If x is at final column, switch to next row
    if (textureX > finalColumn) { textureX = 0; textureY += 240;}
    // If y is at final row, then use start coordinates
    if (textureY > finalRow) {textureX = xStart; textureY = yStart;}

    // If x value is valid, then change current sprite texture coordinates
    if (textureX <= finalColumn) {
        sprite.setTextureRect({{textureX, textureY}, {240, 240}});
    }
}

// Sprint movement boolean, to be called true from main when user double tabs movement keys
void Player::sprint(bool sprint) {
    if (sprint) {
        sprinting = true;
    } else {
        sprinting = false;
    }
}

// Checking for changes to player
void Player::update() {
    // If player is not moving then change player to standing pose
    if (moving == false && attacking == false) {
        if (east) {textureX = standingEastX; textureY = standingEastY;}
        if (north) {textureX = standingNorthX; textureY = standingNorthY;}
        if (northEast) {textureX = standingNorthEastX; textureY = standingNorthEastY;}
        if (northWest) {textureX = standingNorthWestX; textureY = standingNorthWestY;}
        if (south) {textureX = standingSouthX; textureY = standingSouthY;}
        if (southEast) {textureX = standingSouthEastX; textureY = standingSouthEastY;}
        if (southWest) {textureX = standingSouthWestX; textureY = standingSouthWestY;}
        if (west) {textureX = standingWestX; textureY = standingWestY;}
        sprite.setTextureRect({{textureX, textureY}, {240, 240}});
    }

    // Indicate damage by turning sprite red
    if (damageClock.getElapsedTime() >= damageCooldown - sf::seconds(0.7f)) {
        sprite.setColor(sf::Color::White);
    } else {
        sprite.setColor(sf::Color::Red);
    }
}

// Get position of player in main
sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}

// For drawing player in main
void Player::draw(sf::RenderWindow& window, const sf::View& camera) {
    window.draw(sprite);
    // Draw health bar at top left of camera
    sf::Vector2f cameraTopLeft = camera.getCenter() - (camera.getSize() / 2.f);
    for (int i = 0; i < maxHealth; ++i) {
        heartSprite.setTexture(i < health ? heartFull : heartEmpty);
        heartSprite.setPosition({
            cameraTopLeft.x + 10.f + i * 60.f,
            cameraTopLeft.y + 10.f
        });
        window.draw(heartSprite);
    }
}
