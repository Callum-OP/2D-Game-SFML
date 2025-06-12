#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
    : texture{},
        sprite( // Create a player sprite
            [this]() {
              if (!texture.loadFromFile("spritesheet.png"))
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

    // Default movement speed
    verticalSpeed = 2.0f;
    horizontalSpeed = 2.0f;

    // Coordinates for current texture in spritesheet
    textureX = 1;
    textureY = 1;

    // Timer for animations and delays
    timer = 0.0f;
    timerMax = 0.5f;

    // When x coordinates have reached the final column
    finalColumn = 1211;

    // Start and end coordinates for animation textures in spritesheet
    walkingDownXStart = 485;
    walkingDownYStart = 727;
    walkingDownXEnd = 727;
    walkingDownYEnd = 969;
}

void Player::handleInput() {

    // If left key pressed then move character left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
        sprite.move({-horizontalSpeed, 0.0f});
    }
    // If right key pressed then move character right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
        sprite.move({horizontalSpeed, 0.0f});
    }
    // If up key pressed then move character up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
        sprite.move({0.0f, -verticalSpeed});
    }
    // If down key pressed then move character down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
        sprite.move({0.0f, verticalSpeed});

        // Walking down animation
        timer += 0.1f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 242;
            // Call animate function to play the walking down animation
            animate(walkingDownXStart, walkingDownXEnd, 
                walkingDownYStart, walkingDownYEnd, finalColumn);
            timer = 0.0f;
        }
    }
}

// Reusable function for animating player
void Player::animate(int xStart, int xEnd, int yStart, int yEnd, int finalCol) {
    // If current texture coordinates outside of expected values then use start coordinates
    if(textureY == yStart) {if(textureX < xStart) {textureX = xStart;}}
    if(textureY == yEnd) {if(textureX > xEnd) {textureY = yStart; textureX = xStart;}}
    if(textureY < yStart || textureY > yEnd) {textureX = xStart; textureY = yStart;}

    // If x is at final column, switch to next row
    if (textureX > finalCol) { 
        textureX = 1; textureY += 242;
    }

    // If x value is valid, then change current sprite texture coordinates
    if (textureX <= finalCol) {
        sprite.setTextureRect({{textureX, textureY}, {240, 240}});
    }
}


void Player::update() {
    
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
