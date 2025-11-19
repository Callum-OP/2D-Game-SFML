#include <iostream>

#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

Player::Player()
    : texture{},
        sprite( // Create a player sprite
            [this]() {
                if (!texture.loadFromFile("assets/images/playerSpritesheet.png"))
                    throw std::runtime_error("Failed to load sprite sheet");
                return sf::Sprite(texture);
            }()
        ),
        heartSprite( // Create a player sprite
            [this]() {
                if (!heartFull.loadFromFile("assets/images/HeartFull.png"))
                    throw std::runtime_error("Failed to load health textures");
                return sf::Sprite(heartFull);
            }()
        )
    {
    // Size of textures in the spritesheet
    spriteSize = 300;
    // Colour tint of character
    baseColour = sf::Color::Green;

    // Set up sprite
    sprite.setOrigin({spriteSize / 2.0f, (spriteSize + spriteSize / 2) / 2.0f});
    sprite.setTexture(texture);
    sprite.setTextureRect({{485, 1}, {spriteSize, spriteSize}});
    sprite.setScale({0.8f,0.8f});

    // Set health textures
    heartSprite.setScale({0.2f, 0.2f});
    if (!heartFull.loadFromFile("assets/images/HeartFull.png") || !heartEmpty.loadFromFile("assets/images/HeartEmpty.png")) {
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

    SprintEastXStart = 900; SprintEastYStart = 1200; SprintEastXEnd = 2400; SprintEastYEnd = 1200;
    SprintNorthXStart = 2700; SprintNorthYStart = 1200; SprintNorthXEnd = 4200; SprintNorthYEnd = 1200;
    SprintNorthEastXStart = 4500; SprintNorthEastYStart = 1200; SprintNorthEastXEnd = 6000; SprintNorthEastYEnd = 1200;
    SprintNorthWestXStart = 6300; SprintNorthWestYStart = 1200; SprintNorthWestXEnd = 7800; SprintNorthWestYEnd = 1200;
    SprintSouthXStart = 0; SprintSouthYStart = 1500; SprintSouthXEnd = 1500; SprintSouthYEnd = 1500;
    SprintSouthEastXStart = 1800; SprintSouthEastYStart = 1500; SprintSouthEastXEnd = 3300; SprintSouthEastYEnd = 1500;
    SprintSouthWestXStart = 3600; SprintSouthWestYStart = 1500; SprintSouthWestXEnd = 5100; SprintSouthWestYEnd = 1500;
    SprintWestXStart = 5400; SprintWestYStart = 1500; SprintWestXEnd = 6900; SprintWestYEnd = 1500;

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
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed / 1.5f * 4, -verticalSpeed / 1.5f * 4}; sprite.move(movement); textureX += spriteSize; animate(SprintNorthEastXStart, SprintNorthEastXEnd, 
            SprintNorthEastYStart, SprintNorthEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogNorthEastXStart, JogNorthEastXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed / 1.5f * 4, verticalSpeed / 1.5f * 4}; textureX += spriteSize; animate(SprintSouthEastXStart, SprintSouthEastXEnd, 
            SprintSouthEastYStart, SprintSouthEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogSouthEastXStart, JogSouthEastXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed / 1.5f * 4, -verticalSpeed / 1.5f * 4}; textureX += spriteSize; animate(SprintNorthWestXStart, SprintNorthWestXEnd, 
            SprintNorthWestYStart, SprintNorthWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogNorthWestXStart, JogNorthWestXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed / 1.5f * 4, verticalSpeed / 1.5f * 4}; textureX += spriteSize; animate(SprintSouthWestXStart, SprintSouthWestXEnd, 
            SprintSouthWestYStart, SprintSouthWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogSouthWestXStart, JogSouthWestXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {horizontalSpeed * 4, 0.0f}; textureX += spriteSize; animate(SprintEastXStart, SprintEastXEnd, 
            SprintEastYStart, SprintEastYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogEastXStart, JogEastXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {0.0f, -verticalSpeed * 4}; textureX += spriteSize; animate(SprintNorthXStart, SprintNorthXEnd, 
            SprintNorthYStart, SprintNorthYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogNorthXStart, JogNorthXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {0.0f, verticalSpeed * 4}; textureX += spriteSize; animate(SprintSouthXStart, SprintSouthXEnd, 
            SprintSouthYStart, SprintSouthYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogSouthXStart, JogSouthXEnd, 
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
        if (timer >= timerMax && sprinting) {movement = {-horizontalSpeed * 4, 0.0f}; textureX += spriteSize; animate(SprintWestXStart, SprintWestXEnd, 
            SprintWestYStart, SprintWestYEnd); timer = 0.0f;}
        // Jog animation
        else if (timer >= timerMax) {textureX += spriteSize; animate(JogWestXStart, JogWestXEnd, 
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
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackNorthXStart, AttackNorthXEnd, 
                AttackNorthYStart, AttackNorthYEnd); timer = 0.0f;}
        } else if (northEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackNorthEastXStart, AttackNorthEastXEnd, 
                AttackNorthEastYStart, AttackNorthEastYEnd); timer = 0.0f;}
        } else if (east) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackEastXStart, AttackEastXEnd, 
                AttackEastYStart, AttackEastYEnd); timer = 0.0f;}
        } else if (southEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackSouthEastXStart, AttackSouthEastXEnd, 
                AttackSouthEastYStart, AttackSouthEastYEnd); timer = 0.0f;}
        } else if (south) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackSouthXStart, AttackSouthXEnd, 
                AttackSouthYStart, AttackSouthYEnd); timer = 0.0f;}
        } else if (southWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackSouthWestXStart, AttackSouthWestXEnd, 
                AttackSouthWestYStart, AttackSouthWestYEnd); timer = 0.0f;}
        } else if (west) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackWestXStart, AttackWestXEnd, 
                AttackWestYStart, AttackWestYEnd); timer = 0.0f;}
        } else if (northWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += spriteSize; animate(AttackNorthWestXStart, AttackNorthWestXEnd, 
                AttackNorthWestYStart, AttackNorthWestYEnd); timer = 0.0f;}
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
    if (textureX > finalColumn) { textureX = 0; textureY += spriteSize;}
    // If y is at final row, then use start coordinates
    if (textureY > finalRow) {textureX = xStart; textureY = yStart;}

    // If x value is valid, then change current sprite texture coordinates
    if (textureX <= finalColumn) {
        sprite.setTextureRect({{textureX, textureY}, {spriteSize, spriteSize}});
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
        if (east) {textureX = StandingEastX; textureY = StandingEastY;}
        if (north) {textureX = StandingNorthX; textureY = StandingNorthY;}
        if (northEast) {textureX = StandingNorthEastX; textureY = StandingNorthEastY;}
        if (northWest) {textureX = StandingNorthWestX; textureY = StandingNorthWestY;}
        if (south) {textureX = StandingSouthX; textureY = StandingSouthY;}
        if (southEast) {textureX = StandingSouthEastX; textureY = StandingSouthEastY;}
        if (southWest) {textureX = StandingSouthWestX; textureY = StandingSouthWestY;}
        if (west) {textureX = StandingWestX; textureY = StandingWestY;}
        sprite.setTextureRect({{textureX, textureY}, {spriteSize, spriteSize}});
    }

    // Indicate damage by turning sprite red
    if (damageClock.getElapsedTime() >= damageCooldown - sf::seconds(0.7f)) {
        sprite.setColor(baseColour);
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
    // Add shadow under character
    sf::CircleShape shadow(spriteSize / 5.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    shadow.setOrigin({spriteSize / 2.0f, (spriteSize + spriteSize / 2) / 2.0f});
    shadow.setScale({1.0f, 0.4f});
    sf::Vector2f position = sprite.getPosition();
    shadow.setPosition({position.x + spriteSize / 3.18f, position.y + spriteSize / 2.6f});
    window.draw(shadow);

    const sf::Font font("assets/fonts/MagicSchoolOne.ttf");
    window.draw(sprite);

    // Set up UI
    // Draw health bar at top left of camera
    sf::Vector2f cameraLocation = camera.getCenter() - (camera.getSize() / 2.f);
    for (int i = 0; i < maxHealth; ++i) {
        heartSprite.setTexture(i < health ? heartFull : heartEmpty);
        heartSprite.setPosition({
            cameraLocation.x + 10.f + i * 60.f,
            cameraLocation.y + 10.f
        });
        window.draw(heartSprite);
    }
    // Draw gold text at top-right
    sf::Text goldText(font, "Gold: " + std::to_string(Player::getGold()), 44); // Set text string, font, character size
    goldText.setFillColor(sf::Color::Yellow);
    sf::FloatRect textBounds = goldText.getLocalBounds();
    goldText.setOrigin(textBounds.size);
    goldText.setPosition({
        cameraLocation.x + camera.getSize().x - 10.f,
        cameraLocation.y + 37.f
    });
    window.draw(goldText);
}
