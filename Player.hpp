#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Player : public Entity {
public:
    Player(sf::Vector2f startPosition);

    EntityType type() const override { return EntityType::Player; }
    Object& collider() override { return playerCollider; }
    void takeDamage(int amount) override;
    void update(float deltaTime) override;
    sf::Vector2f getPosition() const override { return sprite.getPosition(); }
    void drawShadow(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    bool isDead() const override { return health <= 0; };

    void handleInput();
    void sprint(bool sprint);
    void drawUI(sf::RenderWindow& window, const sf::View& view);
    void animate(int xStart, int xEnd, int yStart, int yEnd);

    void heal(int amount);
    int getHealth() const { return health; };
    int getMaxHealth() const { return maxHealth; };

    int getGold() const { return gold; };;
    void addGold(int amount);
    bool spendGold(int amount);
    void setGold(int amount);

    // Data members
    Object playerCollider;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture heartFull, heartEmpty;
    sf::Sprite heartSprite;
    sf::Clock damageClock;
    sf::Time damageCooldown = sf::seconds(1.f); // 1 second cooldown
    sf::Color baseColour;

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    sf::Vector2f movement;
    int finalColumn, finalRow;
    bool moving, sprinting, attacking;
    Direction currentDirection = Direction::South;

    int maxHealth;
    int health;
    int spriteSize;
    int gold;
};

#endif // PLAYER_HPP