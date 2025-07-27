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
        )
    {
    sprite.setTexture(texture);
    sprite.setTextureRect({{485,1}, {240,240}});
    sprite.setOrigin({sprite.getTextureRect().size.x / 2.0f, sprite.getTextureRect().size.y / 2.0f});
    sf::Vector2<float> position(275.f, 200.f); // Set coordinates
    sprite.setPosition(position); // Place sprite at coordinates
    sprite.setScale({1.0f,1.0f});
    //sprite.setColor(sf::Color::White);

    // Default movement speed
    verticalSpeed = 3.0f;
    horizontalSpeed = 3.0f;
    moving = false;

    // Coordinates for current texture in spritesheet
    textureX = 0;
    textureY = 0;

    // Timer for animations and delays
    timer = 0.0f;
    timerMax = 0.5f;

    // When x coordinates have reached the final column
    finalColumn = 2880;
    finalRow = 3120;

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

    JogEastXStart = 1920; JogEastYStart = 0; JogEastXEnd = 0; JogEastYEnd = 240;
    JogNorthXStart = 240; JogNorthYStart = 240; JogNorthXEnd = 1440; JogNorthYEnd = 240;
    JogNorthEastXStart = 1680; JogNorthEastYStart = 240; JogNorthEastXEnd = 2880; JogNorthEastYEnd = 240;
    JogNorthWestXStart = 0; JogNorthWestYStart = 480; JogNorthWestXEnd = 1200; JogNorthWestYEnd = 480;
    JogSouthXStart = 1440; JogSouthYStart = 480; JogSouthXEnd = 2640; JogSouthYEnd = 480;
    JogSouthEastXStart = 2880; JogSouthEastYStart = 480; JogSouthEastXEnd = 960; JogSouthEastYEnd = 720;
    JogSouthWestXStart = 1200; JogSouthWestYStart = 720; JogSouthWestXEnd = 2400; JogSouthWestYEnd = 720;
    JogWestXStart = 2640; JogWestYStart = 720; JogWestXEnd = 720; JogWestYEnd = 960;

    attacking = false;
    attackEastXStart = 1680; attackEastYStart = 1200; attackEastXEnd = 0; attackEastYEnd = 1440;
    attackNorthXStart = 240; attackNorthYStart = 1440; attackNorthXEnd = 1680; attackNorthYEnd = 1440;
    attackNorthEastXStart = 1920; attackNorthEastYStart = 1440; attackNorthEastXEnd = 240; attackNorthEastYEnd = 1680;
    attackNorthWestXStart = 480; attackNorthWestYStart = 1680; attackNorthWestXEnd = 1920; attackNorthWestYEnd = 1680;
    attackSouthXStart = 2160; attackSouthYStart = 1680; attackSouthXEnd = 480; attackSouthYEnd = 1920;
    attackSouthEastXStart = 720; attackSouthEastYStart = 1920; attackSouthEastXEnd = 2160; attackSouthEastYEnd = 1920;
    attackSouthWestXStart = 2400; attackSouthWestYStart = 1920; attackSouthWestXEnd = 720; attackSouthWestYEnd = 2160;
    attackWestXStart = 960; attackWestYStart = 2160; attackWestXEnd = 2400; attackWestYEnd = 2160;

    sprinting = false;
    sprintingEastXStart = 2640; sprintingEastYStart = 2160; sprintingEastXEnd = 720; sprintingEastYEnd = 2400;
    sprintingNorthXStart = 960; sprintingNorthYStart = 2400; sprintingNorthXEnd = 2160; sprintingNorthYEnd = 2400;
    sprintingNorthEastXStart = 2400; sprintingNorthEastYStart = 2400; sprintingNorthEastXEnd = 480; sprintingNorthEastYEnd = 2640;
    sprintingNorthWestXStart = 720; sprintingNorthWestYStart = 2640; sprintingNorthWestXEnd = 1920; sprintingNorthWestYEnd = 2640;
    sprintingSouthXStart = 2160; sprintingSouthYStart = 2640; sprintingSouthXEnd = 240; sprintingSouthYEnd = 2880;
    sprintingSouthEastXStart = 480; sprintingSouthEastYStart = 2880; sprintingSouthEastXEnd = 1680; sprintingSouthEastYEnd = 2880;
    sprintingSouthWestXStart = 1920; sprintingSouthWestYStart = 2880; sprintingSouthWestXEnd = 0; sprintingSouthWestYEnd = 3120;
    sprintingWestXStart = 240; sprintingWestYStart = 3120; sprintingWestXEnd = 1440; sprintingWestYEnd = 3120;
}

void Player::handleInput(std::vector<sf::FloatRect> wallBounds) {
    // Predict future collision
    sf::FloatRect nextBounds = sprite.getGlobalBounds();

    // If both right and up key pressed then move character right and up at same time
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; northEast = true;
        north = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        sf::Vector2f movement{horizontalSpeed / 1.5f, -verticalSpeed / 1.5f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog north east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, movement.y * 3}); textureX += 240; animate(sprintingNorthEastXStart, sprintingNorthEastXEnd, 
                sprintingNorthEastYStart, sprintingNorthEastYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogNorthEastXStart, JogNorthEastXEnd, 
                JogNorthEastYStart, JogNorthEastYEnd); timer = 0.0f;}
        }
    }
    // If both right and down key pressed then move character right and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; southEast = true;
        north = false; northEast = false; east = false; south = false; southWest = false; west = false; northWest = false;
        sf::Vector2f movement{horizontalSpeed / 1.5f, verticalSpeed / 1.5f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog south east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, movement.y * 3}); textureX += 240; animate(sprintingSouthEastXStart, sprintingSouthEastXEnd, 
                sprintingSouthEastYStart, sprintingSouthEastYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogSouthEastXStart, JogSouthEastXEnd, 
                JogSouthEastYStart, JogSouthEastYEnd); timer = 0.0f;}
        }
    }
    // If both left and up key pressed then move character left and up at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; northWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; west = false;
        sf::Vector2f movement{-horizontalSpeed / 1.5f, -verticalSpeed / 1.5f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog north west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, movement.y * 3}); textureX += 240; animate(sprintingNorthWestXStart, sprintingNorthWestXEnd, 
                sprintingNorthWestYStart, sprintingNorthWestYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogNorthWestXStart, JogNorthWestXEnd, 
                JogNorthWestYStart, JogNorthWestYEnd); timer = 0.0f;}
        }
    }
    // If both left and down key pressed then move character left and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; southWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; west = false; northWest = false;
        sf::Vector2f movement{-horizontalSpeed / 1.5f, verticalSpeed / 1.5f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog south west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, movement.y * 3}); textureX += 240; animate(sprintingSouthWestXStart, sprintingSouthWestXEnd, 
                sprintingSouthWestYStart, sprintingSouthWestYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogSouthWestXStart, JogSouthWestXEnd, 
                JogSouthWestYStart, JogSouthWestYEnd); timer = 0.0f;}
        }
    }
    // If right key pressed then move character right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        moving = true; east = true;
        north = false; northEast = false; southEast = false; south = false ;southWest = false; west = false; northWest = false;
        sf::Vector2f movement{horizontalSpeed, 0.0f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, 0.0f}); textureX += 240; animate(sprintingEastXStart, sprintingEastXEnd, 
                sprintingEastYStart, sprintingEastYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogEastXStart, JogEastXEnd, 
                JogEastYStart, JogEastYEnd); timer = 0.0f;}
        }
    }
    // If up key pressed then move character up
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; north = true;
        northEast = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        sf::Vector2f movement{0.0f, -verticalSpeed};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog north animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({0.0f, movement.y * 3}); textureX += 240; animate(sprintingNorthXStart, sprintingNorthXEnd, 
                sprintingNorthYStart, sprintingNorthYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogNorthXStart, JogNorthXEnd, 
                JogNorthYStart, JogNorthYEnd); timer = 0.0f;}
        }
    }
    // If down key pressed then move character down
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; south = true;
        north = false; northEast = false; east = false; southEast = false; southWest = false; west = false; northWest = false;
        sf::Vector2f movement{0.0f, verticalSpeed};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog south animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({0.0f, movement.y * 3}); textureX += 240; animate(sprintingSouthXStart, sprintingSouthXEnd, 
                sprintingSouthYStart, sprintingSouthYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogSouthXStart, JogSouthXEnd, 
                JogSouthYStart, JogSouthYEnd); timer = 0.0f;}
        }
    }
    // If left key pressed then move character left
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        moving = true; west = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; northWest = false;
        sf::Vector2f movement{-horizontalSpeed, 0.0f};
        if (!handleCollision(movement, nextBounds, wallBounds)) {
            // Waits for set amount of time then plays Jog west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {sprite.move({movement.x * 3, 0.0f}); textureX += 240; animate(sprintingWestXStart, sprintingWestXEnd, 
                sprintingWestYStart, sprintingWestYEnd); timer = 0.0f;}
            // Jog animation
            else if (timer >= timerMax) {textureX += 240; animate(JogWestXStart, JogWestXEnd, 
                JogWestYStart, JogWestYEnd); timer = 0.0f;}
        }
    } else {
        moving = false;
    }
    // If space pressed then attack and change to attack animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
        attacking = true;
        timer += 0.08f;
        // Check direction user is facing
        if (north) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackNorthXStart, attackNorthXEnd, 
                attackNorthYStart, attackNorthYEnd); timer = 0.0f;}
        } else if (northEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackNorthEastXStart, attackNorthEastXEnd, 
                attackNorthEastYStart, attackNorthEastYEnd); timer = 0.0f;}
        } else if (east) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackEastXStart, attackEastXEnd, 
                attackEastYStart, attackEastYEnd); timer = 0.0f;}
        } else if (southEast) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackSouthEastXStart, attackSouthEastXEnd, 
                attackSouthEastYStart, attackSouthEastYEnd); timer = 0.0f;}
        } else if (south) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackSouthXStart, attackSouthXEnd, 
                attackSouthYStart, attackSouthYEnd); timer = 0.0f;}
        } else if (southWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackSouthWestXStart, attackSouthWestXEnd, 
                attackSouthWestYStart, attackSouthWestYEnd); timer = 0.0f;}
        } else if (west) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackWestXStart, attackWestXEnd, 
                attackWestYStart, attackWestYEnd); timer = 0.0f;}
        } else if (northWest) {
            // Attack animation
            if (timer >= timerMax) {textureX += 240; animate(attackNorthWestXStart, attackNorthWestXEnd, 
                attackNorthWestYStart, attackNorthWestYEnd); timer = 0.0f;}
        }
    } else {
        attacking = false;
    }
}

bool Player::handleCollision(sf::Vector2f movement, sf::FloatRect nextBounds, std::vector<sf::FloatRect> wallBounds) {
// Full movement prediction
    nextBounds.position += movement;

    for (const auto& wall : wallBounds) {
        if (nextBounds.findIntersection(wall).has_value()) {
            // Try horizontal-only movement
            sf::Rect<float> horizontalBounds = nextBounds;
            horizontalBounds.position.x -= movement.x; // Undo x movement

            if (!horizontalBounds.findIntersection(wall).has_value()) {
                sprite.move({0.f, movement.y}); // Slide vertically
                return true;
            }

            // Try vertical-only movement
            sf::Rect<float> verticalBounds = nextBounds;
            verticalBounds.position.y -= movement.y; // Undo y movement

            if (!verticalBounds.findIntersection(wall).has_value()) {
                sprite.move({movement.x, 0.f}); // Slide horizontally
                return true;
            }

            std::cout << "Blocked from all sides\n";
            return true; // Fully blocked
        }
    }
    // No collision — move freely
    sprite.move(movement);
    return false;
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
}

// Get position of player in main
sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}

// For drawing player in main
void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
