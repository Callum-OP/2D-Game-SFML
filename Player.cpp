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
    //sprite.setColor(sf::Color::White);

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
    // These values will need to be changed if the spritesheet changes
    walkingDownXStart = 485;
    walkingDownYStart = 727;
    walkingDownXEnd = 727;
    walkingDownYEnd = 969;

    walkingUpXStart = 1;
    walkingUpYStart = 485;
    walkingUpXEnd = 243;
    walkingUpYEnd = 727;

    walkingLeftXStart = 969;
    walkingLeftYStart = 969;
    walkingLeftXEnd = 1211;
    walkingLeftYEnd = 1211;

    walkingRightXStart = 969;
    walkingRightYStart = 1;
    walkingRightXEnd = 1211;
    walkingRightYEnd = 243;
}

void Player::handleInput() {

    // If left key pressed then move character left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        sprite.move({-horizontalSpeed, 0.0f});

        // Walking left animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 242;
            // Call animate function to play the walking left animation
            animate(walkingLeftXStart, walkingLeftXEnd, 
                walkingLeftYStart, walkingLeftYEnd, finalColumn);
            timer = 0.0f;
        }
    }
    // If right key pressed then move character right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        sprite.move({horizontalSpeed, 0.0f});

        // Walking right animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 242;
            // Call animate function to play the walking right animation
            animate(walkingRightXStart, walkingRightXEnd, 
                walkingRightYStart, walkingRightYEnd, finalColumn);
            timer = 0.0f;
        }
    }
    // If up key pressed then move character up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        sprite.move({0.0f, -verticalSpeed});

        // Walking up animation
        timer += 0.08f;
        // Will wait for set amount of time
        if (timer >= timerMax) {
            textureX += 242;
            // Call animate function to play the walking up animation
            animate(walkingUpXStart, walkingUpXEnd, 
                walkingUpYStart, walkingUpYEnd, finalColumn);
            timer = 0.0f;
        }
    }
    // If down key pressed then move character down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        sprite.move({0.0f, verticalSpeed});

        // Walking down animation
        timer += 0.08f;
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
    if(textureY == yStart) {if(textureX < xStart) {textureX = xStart;}} // If column is incorrect
    else if(textureY == yEnd) {if(textureX > xEnd) {textureX = xStart; textureY = yStart;}}
    if(textureY < yStart || textureY > yEnd) {textureX = xStart; textureY = yStart;} // If row is incorrect

    // If x is at final column, switch to next row
    if (textureX > finalCol) { textureX = 1; textureY += 242;}
    // If y is at final column, then use start coordinates
    if (textureY > finalCol) {textureX = xStart; textureY = yStart;}

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
