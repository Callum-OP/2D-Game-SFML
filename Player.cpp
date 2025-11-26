#include <iostream>

#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

#include "Constants.hpp"

Player::Player(sf::Vector2f startPosition)
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
    ),
    playerCollider(playerCollider) // Store reference to collider
{
    // Create player collider
    playerCollider = { Vec2(startPosition.x, startPosition.y), { Vec2(-50, -50), Vec2(50, 50) } };

    // Set start position of player
    sprite.setPosition(startPosition);

    // Size of textures in the spritesheet
    spriteSize = PLAYER_SPRITE_SIZE;
    // Colour tint of character
    baseColour = PLAYER_BASE_COLOUR;

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
    // Set health
    maxHealth = PLAYER_MAX_HEALTH;
    health = PLAYER_MAX_HEALTH;
    // Set gold
    gold = 0;

    // Default movement speed
    verticalSpeed = PLAYER_V_SPEED;
    horizontalSpeed = PLAYER_H_SPEED;
    movement = {horizontalSpeed, 0};
    moving = false;

    // Timer for animations and delays
    timer = PLAYER_TIMER;
    timerMax = PLAYER_TIMER_MAX;

    // When x coordinates have reached the final column
    finalColumn = PLAYER_FINAL_COLUMN;
    finalRow = PLAYER_FINAL_ROW;

    // Coordinates for current texture in spritesheet
    auto coords = playerAnimationTable.at({Action::Standing, Direction::South});
    textureX = coords.xStart;
    textureY = coords.yStart;
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
    auto pressed = [&](sf::Keyboard::Scancode key1, std::optional<sf::Keyboard::Scancode> key2 = std::nullopt) {
        if (sf::Keyboard::isKeyPressed(key1)) return true;
        if (key2 && sf::Keyboard::isKeyPressed(*key2)) return true;
        return false;
    };

    // Set up possilbe key combinations to related directions and movement values
    struct InputMapping {
        bool condition;
        Direction dir;
        sf::Vector2f baseMovement;
    };
    std::vector<InputMapping> mappings = {
        { (pressed(sf::Keyboard::Scan::Right, sf::Keyboard::Scan::D) && pressed(sf::Keyboard::Scan::Up, sf::Keyboard::Scan::W)), Direction::NorthEast, { horizontalSpeed / 1.5f, -verticalSpeed / 1.5f } },
        { (pressed(sf::Keyboard::Scan::Right, sf::Keyboard::Scan::D) && pressed(sf::Keyboard::Scan::Down, sf::Keyboard::Scan::S)), Direction::SouthEast, { horizontalSpeed / 1.5f,  verticalSpeed / 1.5f } },
        { (pressed(sf::Keyboard::Scan::Left,  sf::Keyboard::Scan::A) && pressed(sf::Keyboard::Scan::Up,   sf::Keyboard::Scan::W)), Direction::NorthWest, { -horizontalSpeed / 1.5f, -verticalSpeed / 1.5f } },
        { (pressed(sf::Keyboard::Scan::Left,  sf::Keyboard::Scan::A) && pressed(sf::Keyboard::Scan::Down, sf::Keyboard::Scan::S)), Direction::SouthWest, { -horizontalSpeed / 1.5f,  verticalSpeed / 1.5f } },
        { pressed(sf::Keyboard::Scan::Right, sf::Keyboard::Scan::D), Direction::East,  { horizontalSpeed, 0.0f } },
        { pressed(sf::Keyboard::Scan::Up,    sf::Keyboard::Scan::W), Direction::North, { 0.0f, -verticalSpeed } },
        { pressed(sf::Keyboard::Scan::Down,  sf::Keyboard::Scan::S), Direction::South, { 0.0f,  verticalSpeed } },
        { pressed(sf::Keyboard::Scan::Left,  sf::Keyboard::Scan::A), Direction::West,  { -horizontalSpeed, 0.0f } }
    };

    bool moved = false;
    for (auto& m : mappings) {
        if (m.condition) {
            moved = true;
            moving = true;
            currentDirection = m.dir;
            movement = m.baseMovement;

            timer += 0.08f;
            // Play movement animation
            if (timer >= timerMax) {
                Action action = sprinting ? Action::Sprint : Action::Jog; // Check if sprinting or jogging
                auto coords = playerAnimationTable.at({action, currentDirection});
                animate(coords.xStart, coords.xEnd, coords.yStart, coords.yEnd);

                if (sprinting) movement *= 4.0f; // Increase movement speed if sprinting
                textureX += spriteSize; // Increment through textures
                timer = 0.0f;
            }
            sprite.move(movement); // Move player
            break;
        }
    }
    if (!moved) {
        movement = {0.0f, 0.0f};
        moving = false;
    }

    // If space pressed then attack and change to attack animation
    if (pressed(sf::Keyboard::Scan::Space)) {
        attacking = true;
        timer += 0.08f;
        if (timer >= timerMax) {
            auto coords = playerAnimationTable.at({Action::Attack, currentDirection});
            animate(coords.xStart, coords.xEnd, coords.yStart, coords.yEnd);
            textureX += spriteSize;
            timer = 0.0f;
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
void Player::update(float deltaTime) {
    // If player is not moving then change player to standing pose
    if (moving == false && attacking == false) {
        auto coords = playerAnimationTable.at({Action::Standing, currentDirection});
        textureX = coords.xStart; textureY = coords.yStart;
        sprite.setTextureRect({{textureX, textureY}, {spriteSize, spriteSize}});
    }

    // Indicate damage by turning sprite red
    if (damageClock.getElapsedTime() >= damageCooldown - sf::seconds(0.7f)) {
        sprite.setColor(baseColour);
    } else {
        sprite.setColor(sf::Color::Red);
    }

    // Sync sprite with collider
    sprite.setPosition(toSF(playerCollider.pos));
}

// For drawing player in main

// Draw shadow seperately
void Player::drawShadow(sf::RenderWindow& window) {
    // Add shadow under character
    sf::CircleShape shadow(spriteSize / 5.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    shadow.setOrigin({spriteSize / 2.0f, (spriteSize + spriteSize / 2) / 2.0f});
    shadow.setScale({1.0f, 0.4f});
    sf::Vector2f position = sprite.getPosition();
    shadow.setPosition({position.x + spriteSize / 3.18f, position.y + spriteSize / 2.6f});
    window.draw(shadow);
}

// Draw player sprite
void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Draw user interface features
void Player::drawUI(sf::RenderWindow& window, const sf::View& camera) {
    const sf::Font font("assets/fonts/MagicSchoolOne.ttf");
    
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
