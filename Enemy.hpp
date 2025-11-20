#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Physics.hpp"
#include "PathFinder.hpp"
#include <SFML/Graphics.hpp>

#include "Constants.hpp"

class Enemy {
public:
    Enemy(sf::Vector2f startPosition, sf::Texture& texture);

    void update(float deltaTime, Grid& grid, const sf::Vector2f& playerPosition, bool playerAttacking);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow& window);
    void animate(int xStart, int xEnd, int yStart, int yEnd);

    void takeDamage(int amount);
    void heal(int amount);
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    bool isDead() const { return health <= 0; }

    // Data members
    sf::Texture texture;
    sf::Sprite sprite;
    Object collider;
    sf::Clock damageClock;
    sf::Time damageCooldown = sf::seconds(0.5f);
    sf::Vector2f direction;
    sf::Color baseColour;

    int textureX, textureY;
    float timer, timerMax;
    float speed;
    int finalColumn, finalRow;
    bool moving, attacking;
    Direction currentDirection = Direction::South;

    int maxHealth;
    int health;
    int spriteSize;
    float attackRadius;
};

#endif // ENEMY_HPP