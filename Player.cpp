#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

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
    finalColumn = 2160;
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

    joggingEastXStart = 1920; joggingEastYStart = 0; joggingEastXEnd = 720; joggingEastYEnd = 240;
    joggingNorthXStart = 960; joggingNorthYStart = 240; joggingNorthXEnd = 2160; joggingNorthYEnd = 240;
    joggingNorthEastXStart = 0; joggingNorthEastYStart = 480; joggingNorthEastXEnd = 1200; joggingNorthEastYEnd = 480;
    joggingNorthWestXStart = 1440; joggingNorthWestYStart = 480; joggingNorthWestXEnd = 240; joggingNorthWestYEnd = 720;
    joggingSouthXStart = 480; joggingSouthYStart = 720; joggingSouthXEnd = 1680; joggingSouthYEnd = 720;
    joggingSouthEastXStart = 1920; joggingSouthEastYStart = 720; joggingSouthEastXEnd = 720; joggingSouthEastYEnd = 960;
    joggingSouthWestXStart = 960; joggingSouthWestYStart = 960; joggingSouthWestXEnd = 2160; joggingSouthWestYEnd = 960;
    joggingWestXStart = 0; joggingWestYStart = 1200; joggingWestXEnd = 1200; joggingWestYEnd = 1200;

    sprinting = false;
    sprintingEastXStart = 1440; sprintingEastYStart = 1200; sprintingEastXEnd = 240; sprintingEastYEnd = 1440;
    sprintingNorthXStart = 480; sprintingNorthYStart = 1440; sprintingNorthXEnd = 1680; sprintingNorthYEnd = 1440;
    sprintingNorthEastXStart = 1920; sprintingNorthEastYStart = 1440; sprintingNorthEastXEnd = 720; sprintingNorthEastYEnd = 1680;
    sprintingNorthWestXStart = 960; sprintingNorthWestYStart = 1680; sprintingNorthWestXEnd = 2160; sprintingNorthWestYEnd = 1680;
    sprintingSouthXStart = 0; sprintingSouthYStart = 1920; sprintingSouthXEnd = 1200; sprintingSouthYEnd = 1920;
    sprintingSouthEastXStart = 1440; sprintingSouthEastYStart = 1920; sprintingSouthEastXEnd = 240; sprintingSouthEastYEnd = 2160;
    sprintingSouthWestXStart = 480; sprintingSouthWestYStart = 2160; sprintingSouthWestXEnd = 1680; sprintingSouthWestYEnd = 2160;
    sprintingWestXStart = 1920; sprintingWestYStart = 2160; sprintingWestXEnd = 720; sprintingWestYEnd = 2400;

    sprinting = false;
    attackEastXStart = 1440; attackEastYStart = 1200; attackEastXEnd = 240; attackEastYEnd = 1440;
    attackNorthXStart = 480; attackNorthYStart = 1440; attackNorthXEnd = 1680; attackNorthYEnd = 1440;
    attackNorthEastXStart = 1920; attackNorthEastYStart = 1440; attackNorthEastXEnd = 720; attackNorthEastYEnd = 1680;
    attackNorthWestXStart = 960; attackNorthWestYStart = 1680; attackNorthWestXEnd = 2160; attackNorthWestYEnd = 1680;
    attackSouthXStart = 0; attackSouthYStart = 1920; attackSouthXEnd = 1200; attackSouthYEnd = 1920;
    attackSouthEastXStart = 1440; attackSouthEastYStart = 1920; attackSouthEastXEnd = 240; attackSouthEastYEnd = 2160;
    attackSouthWestXStart = 480; attackSouthWestYStart = 2160; attackSouthWestXEnd = 1680; attackSouthWestYEnd = 2160;
    attackWestXStart = 1920; attackWestYStart = 2160; attackWestXEnd = 720; attackWestYEnd = 2400;
}

void Player::handleInput() {
    // If both right and up key pressed then move character north east
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({horizontalSpeed / 1.5f, -verticalSpeed / 1.5f});
        if (sprinting) {sprite.move({horizontalSpeed / 1.5f * 1.5f, -verticalSpeed / 1.5f * 1.5f});}
        moving = true;
    }
    // If both right and down key pressed then move character south east
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({horizontalSpeed / 1.5f, verticalSpeed / 1.5f});
        if (sprinting) {sprite.move({(horizontalSpeed / 1.5f) * 1.5f, (verticalSpeed / 1.5f) * 1.5f});}
        moving = true;
    }
    // If both left and up key pressed then move character north west
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({-horizontalSpeed / 1.5f, -verticalSpeed / 1.5f});
        if (sprinting) {sprite.move({-horizontalSpeed / 1.5f * 1.5f, -verticalSpeed / 1.5f * 1.5f});}
        moving = true;
    }
    // If both left and down key pressed then move character south west
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({-horizontalSpeed / 1.5f, verticalSpeed / 1.5f});
        if (sprinting) {sprite.move({(-horizontalSpeed / 1.5f) * 1.5f, (verticalSpeed / 1.5f) * 1.5f});}
        moving = true;
    }
    // If right key pressed then move character east
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        sprite.move({horizontalSpeed, 0.0f});
        if (sprinting) {sprite.move({horizontalSpeed * 1.5f, 0.0f});}
        moving = true;
    }
    // If up key pressed then move character north
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({0.0f, -verticalSpeed});
        if (sprinting) {sprite.move({0.0f, -verticalSpeed * 1.5f});}
        moving = true;
    }
    // If down key pressed then move character south
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({0.0f, verticalSpeed});
        if (sprinting) {sprite.move({0.0f, verticalSpeed * 1.5f});}
        moving = true;
    }
    // If left key pressed then move character west
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        sprite.move({-horizontalSpeed, 0.0f});
        if (sprinting) {sprite.move({-horizontalSpeed * 1.5f, 0.0f});}
        moving = true;
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

// Sprint movement boolean, to be called true in main when user double tabs movement keys
void Player::sprint(bool sprint) {
    if (sprint) {
        sprinting = true;
    } else {
        sprinting = false;
    }
}

// Checking for changes to player (such as animation)
void Player::update(sf::RenderWindow& window) {
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
    if (moving) {
        if (east) {
            // Waits for set amount of time then plays jogging east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingEastXStart, sprintingEastXEnd, 
                sprintingEastYStart, sprintingEastYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingEastXStart, joggingEastXEnd, 
                joggingEastYStart, joggingEastYEnd); timer = 0.0f;}
        }
        if (north) {
            // Waits for set amount of time then plays jogging north animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingNorthXStart, sprintingNorthXEnd, 
                sprintingNorthYStart, sprintingNorthYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingNorthXStart, joggingNorthXEnd, 
                joggingNorthYStart, joggingNorthYEnd); timer = 0.0f;}
        }
        if (northEast) {
            // Waits for set amount of time then plays jogging north east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingNorthEastXStart, sprintingNorthEastXEnd, 
                sprintingNorthEastYStart, sprintingNorthEastYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingNorthEastXStart, joggingNorthEastXEnd, 
                joggingNorthEastYStart, joggingNorthEastYEnd); timer = 0.0f;}
        }
        if (northWest) {
            // Waits for set amount of time then plays jogging north west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingNorthWestXStart, sprintingNorthWestXEnd, 
                sprintingNorthWestYStart, sprintingNorthWestYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingNorthWestXStart, joggingNorthWestXEnd, 
                joggingNorthWestYStart, joggingNorthWestYEnd); timer = 0.0f;}
            }
        if (south) {
            // Waits for set amount of time then plays jogging south animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingSouthXStart, sprintingSouthXEnd, 
                sprintingSouthYStart, sprintingSouthYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingSouthXStart, joggingSouthXEnd, 
                joggingSouthYStart, joggingSouthYEnd); timer = 0.0f;}
        }
        if (southEast) {
            // Waits for set amount of time then plays jogging south east animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingSouthEastXStart, sprintingSouthEastXEnd, 
                sprintingSouthEastYStart, sprintingSouthEastYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingSouthEastXStart, joggingSouthEastXEnd, 
                joggingSouthEastYStart, joggingSouthEastYEnd); timer = 0.0f;}
        }
        if (southWest) {
            // Waits for set amount of time then plays jogging south west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingSouthWestXStart, sprintingSouthWestXEnd, 
                sprintingSouthWestYStart, sprintingSouthWestYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingSouthWestXStart, joggingSouthWestXEnd, 
                joggingSouthWestYStart, joggingSouthWestYEnd); timer = 0.0f;}
            }
        if (west) {
            // Waits for set amount of time then plays jogging west animation
            timer += 0.08f;
            // Sprinting animation
            if (timer >= timerMax && sprinting) {textureX += 240; animate(sprintingWestXStart, sprintingWestXEnd, 
                sprintingWestYStart, sprintingWestYEnd); timer = 0.0f;}
            // Jogging animation
            else if (timer >= timerMax) {textureX += 240; animate(joggingWestXStart, joggingWestXEnd, 
                joggingWestYStart, joggingWestYEnd); timer = 0.0f;}
        }
    }

    // Get mouse position relative to window
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    sf::Vector2f center = sprite.getPosition();
    // Calculate angle
    float angle = atan2(mousePosF.y - center.y, mousePosF.x - center.x) * 180.0f / 3.14159f;
    if (angle < 0) angle += 360.0f;

    // Get direction of mouse
    if (angle >= 337.5f || angle < 22.5f) {
        east = true;
        north = false; northEast = false; east = true; southEast = false; 
        south = false; southWest = false; west = false; northWest = false;
    } else if (angle < 67.5f) {
        southEast = true;
        north = false; northEast = false; east = false;
        south = false; southWest = false; west = false; northWest = false;
    } else if (angle < 112.5f) {
        south = true;
        north = false; northEast = false; east = false; southEast = false; 
        southWest = false; west = false; northWest = false;
    } else if (angle < 157.5f) {
        southWest = true;
        north = false; northEast = false; east = false; southEast = false; 
        south = false; west = false; northWest = false;
    } else if (angle < 202.5f) {
        west = true;
        north = false; northEast = false; east = false; southEast = false; 
        south = false; southWest = false; northWest = false;
    } else if (angle < 247.5f) {
        northWest = true;
        north = false; northEast = false; east = false; southEast = false; 
        south = false; southWest = false; west = false;
    } else if (angle < 292.5f) {
        north = true;
        northEast = false; east = false; southEast = false; 
        south = false; southWest = false; west = false; northWest = false;
    } else {
        northEast = true;
        north = false; east = false; southEast = false; 
        south = false; southWest = false; west = false; northWest = false;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
