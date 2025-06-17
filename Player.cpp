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

    // If left key pressed then move character left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        sprite.move({-horizontalSpeed, 0.0f});

        // jogging left animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 240;
            // Call animate function to play the jogging left animation
            animate(joggingWestXStart, joggingWestXEnd, 
                joggingWestYStart, joggingWestYEnd);
            timer = 0.0f;
        }
    }
    // If right key pressed then move character right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        sprite.move({horizontalSpeed, 0.0f});

        // jogging right animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 240;
            // Call animate function to play the jogging right animation
            animate(joggingEastXStart, joggingEastXEnd, 
                joggingEastYStart, joggingEastYEnd);
            timer = 0.0f;
        }
    }
    // If up key pressed then move character up
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({0.0f, -verticalSpeed});

        // jogging up animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 240;
            // Call animate function to play the jogging up animation
            animate(joggingNorthXStart, joggingNorthXEnd, 
                joggingNorthYStart, joggingNorthYEnd);
            timer = 0.0f;
        }
    }
    // If down key pressed then move character down
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({0.0f, verticalSpeed});

        // jogging down animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 240;
            // Call animate function to play the jogging down animation
            animate(joggingSouthXStart, joggingSouthXEnd, 
                joggingSouthYStart, joggingSouthYEnd);
            timer = 0.0f;
        }
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


void Player::update() {
    
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
