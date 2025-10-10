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

    int textureX, textureY;
    float timer, timerMax;
    float speed;
    int finalColumn, finalRow;
    bool moving, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

    int maxHealth;
    int health;
    int spriteSize;
    float attackRadius;

    int standingNorthX, standingNorthY, standingNorthWestX, standingNorthWestY, 
    standingNorthEastX, standingNorthEastY, standingEastX, standingEastY, 
    standingSouthEastX, standingSouthEastY, standingSouthX, standingSouthY,
    standingSouthWestX, standingSouthWestY, standingWestX, standingWestY;

    int JogNorthXStart, JogNorthYStart, JogNorthXEnd, JogNorthYEnd;
    int JogNorthEastXStart, JogNorthEastYStart, JogNorthEastXEnd, JogNorthEastYEnd;
    int JogEastXStart, JogEastYStart, JogEastXEnd, JogEastYEnd;
    int JogSouthEastXStart, JogSouthEastYStart, JogSouthEastXEnd, JogSouthEastYEnd;
    int JogSouthXStart, JogSouthYStart, JogSouthXEnd, JogSouthYEnd;
    int JogSouthWestXStart, JogSouthWestYStart, JogSouthWestXEnd, JogSouthWestYEnd;
    int JogWestXStart, JogWestYStart, JogWestXEnd, JogWestYEnd;
    int JogNorthWestXStart, JogNorthWestYStart, JogNorthWestXEnd, JogNorthWestYEnd;

    int attackNorthXStart, attackNorthYStart, attackNorthXEnd, attackNorthYEnd;
    int attackNorthEastXStart, attackNorthEastYStart, attackNorthEastXEnd, attackNorthEastYEnd;
    int attackEastXStart, attackEastYStart, attackEastXEnd, attackEastYEnd;
    int attackSouthEastXStart, attackSouthEastYStart, attackSouthEastXEnd, attackSouthEastYEnd;
    int attackSouthXStart, attackSouthYStart, attackSouthXEnd, attackSouthYEnd;
    int attackSouthWestXStart, attackSouthWestYStart, attackSouthWestXEnd, attackSouthWestYEnd;
    int attackWestXStart, attackWestYStart, attackWestXEnd, attackWestYEnd;
    int attackNorthWestXStart, attackNorthWestYStart, attackNorthWestXEnd, attackNorthWestYEnd;
};

#endif // ENEMY_HPP