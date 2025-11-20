#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput();
    void Player::sprint(bool sprint);
    void update();
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow& window, const sf::View& view);
    void animate(int xStart, int xEnd, int yStart, int yEnd);

    void Player::takeDamage(int amount);
    void Player::heal(int amount);
    int Player::getHealth() const { return health; };
    int Player::getMaxHealth() const { return maxHealth; };
    bool Player::isDead() const { return health <= 0; };

    int getGold() const { return gold; };;
    void addGold(int amount);
    bool spendGold(int amount);
    void setGold(int amount);

    // Data members
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