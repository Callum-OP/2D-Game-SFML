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
    verticalSpeed = 2.0f;
    horizontalSpeed = 2.0f;
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

}

void Player::handleInput() {
    // If both right and up key pressed then move character right and up at same time
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        moving = true; northEast = true;
        north = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        sprite.move({horizontalSpeed, 0.0f}); sprite.move({0.0f, -verticalSpeed});
        // Waits for set amount of time then plays jogging north east animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingNorthEastXStart, joggingNorthEastXEnd, 
            joggingNorthEastYStart, joggingNorthEastYEnd); timer = 0.0f;}
    }
    // If both right and down key pressed then move character right and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        moving = true; southEast = true;
        north = false; northEast = false; east = false; south = false; southWest = false; west = false; northWest = false;
        sprite.move({horizontalSpeed, 0.0f}); sprite.move({0.0f, verticalSpeed});
        // Waits for set amount of time then plays jogging south east animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingSouthEastXStart, joggingSouthEastXEnd, 
            joggingSouthEastYStart, joggingSouthEastYEnd); timer = 0.0f;}
    }
    // If both left and up key pressed then move character left and up at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({-horizontalSpeed, 0.0f}); sprite.move({0.0f, -verticalSpeed});
        moving = true; northWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; west = false;
        // Waits for set amount of time then plays jogging north west animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingNorthWestXStart, joggingNorthWestXEnd, 
            joggingNorthWestYStart, joggingNorthWestYEnd); timer = 0.0f;}
    }
    // If both left and down key pressed then move character left and down at same time
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({-horizontalSpeed, 0.0f}); sprite.move({0.0f, verticalSpeed});
        moving = true; southWest = true;
        north = false; northEast = false; east = false; southEast = false; south = false; west = false; northWest = false;
        // Waits for set amount of time then plays jogging south west animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingSouthWestXStart, joggingSouthWestXEnd, 
            joggingSouthWestYStart, joggingSouthWestYEnd); timer = 0.0f;}
    }
    // If right key pressed then move character right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        sprite.move({horizontalSpeed, 0.0f});
        moving = true; east = true;
        north = false; northEast = false; southEast = false; south = false ;southWest = false; west = false; northWest = false;
        // Waits for set amount of time then plays jogging east animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingEastXStart, joggingEastXEnd, 
            joggingEastYStart, joggingEastYEnd); timer = 0.0f;}
    }
    // If up key pressed then move character up
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({0.0f, -verticalSpeed});
        moving = true; north = true;
        northEast = false; east = false; southEast = false; south = false; southWest = false; west = false; northWest = false;
        // Waits for set amount of time then plays jogging north animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingNorthXStart, joggingNorthXEnd, 
            joggingNorthYStart, joggingNorthYEnd); timer = 0.0f;}
    }
    // If down key pressed then move character down
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({0.0f, verticalSpeed});
        moving = true; south = true;
        north = false; northEast = false; east = false; southEast = false; southWest = false; west = false; northWest = false;
        // Waits for set amount of time then plays jogging south animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingSouthXStart, joggingSouthXEnd, 
            joggingSouthYStart, joggingSouthYEnd); timer = 0.0f;}
    }
    // If left key pressed then move character left
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        sprite.move({-horizontalSpeed, 0.0f});
        moving = true; west = true;
        north = false; northEast = false; east = false; southEast = false; south = false; southWest = false; northWest = false;
        // Waits for set amount of time then plays jogging west animation
        timer += 0.08f;
        if (timer >= timerMax) {textureX += 240; animate(joggingWestXStart, joggingWestXEnd, 
            joggingWestYStart, joggingWestYEnd); timer = 0.0f;}
    } else {
        moving = false;
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

// Checking for changes to player
void Player::update() {
    // If player is not moving then change player to standing pose
    if (moving == false) {
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

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
