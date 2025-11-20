#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput();
    void sprint(bool sprint);
    void update();
    sf::Vector2f getPosition();
    void drawUI(sf::RenderWindow& window, const sf::View& view);
    void drawShadow(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window);
    void animate(int xStart, int xEnd, int yStart, int yEnd);

    void takeDamage(int amount);
    void heal(int amount);
    int getHealth() const { return health; };
    int getMaxHealth() const { return maxHealth; };
    bool isDead() const { return health <= 0; };

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