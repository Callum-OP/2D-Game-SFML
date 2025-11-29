#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Player.hpp"
#include "Physics.hpp"
#include "PathFinder.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

#include "Entity.hpp"
#include "Constants.hpp"

class Enemy : public Entity {
public:
    Enemy(sf::Vector2f startPosition, sf::Texture& texture, Grid& grid, Player& player);

    EntityType type() const override { return EntityType::Enemy; }
    Object& collider() override { return enemyCollider; }
    sf::Vector2f getPosition() const override { return sprite.getPosition(); }
    bool isAttacking() const override { return attacking; }
    float getAttackRadius() const override { return attackRadius; }
    void update(float deltaTime) override;
    void drawShadow(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    bool isDead() const override { return health <= 0; }

    sf::Vector2f Enemy::getRandomPatrolPoint(float radius);
    void animate(int xStart, int xEnd, int yStart, int yEnd);

    void takeDamage(int amount);
    void heal(int amount);
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

    // Data members
    Object enemyCollider;
    sf::Texture texture;
    sf::Sprite sprite;
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
    sf::Vector2f spawnPoint;
    sf::Vector2f randomPoint;
    std::optional<sf::Vector2f> targetPosition;
    float idleTimer = 0.f;

    int maxHealth;
    int health;
    int spriteSize;
    float attackRadius;
    
private:
    Grid& grid;
    Player& player;
};

#endif // ENEMY_HPP