#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(sf::Vector2f startPosition, sf::Color startColour);
    void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition, bool playerAttacking);
    sf::Vector2f getPosition();
    void Enemy::draw(sf::RenderWindow& window);
    void Enemy::animate(int xStart, int xEnd, int yStart, int yEnd);
    sf::Texture texture;
    sf::Sprite sprite;

    int textureX, textureY;
    float timer, timerMax;
    float speed;
    int finalColumn, finalRow;
    bool moving, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

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

    bool dead = false;
};

#endif // ENEMY_HPP