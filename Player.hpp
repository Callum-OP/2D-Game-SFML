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
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture heartFull, heartEmpty;
    sf::Sprite heartSprite;
    sf::Clock damageClock;
    sf::Time damageCooldown = sf::seconds(1.f); // 1 second cooldown

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    sf::Vector2f movement;
    int finalColumn, finalRow;
    bool moving, sprinting, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

    int maxHealth;
    int health;

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

    int SprintNorthXStart, SprintNorthYStart, SprintNorthXEnd, SprintNorthYEnd;
    int SprintNorthEastXStart, SprintNorthEastYStart, SprintNorthEastXEnd, SprintNorthEastYEnd;
    int SprintEastXStart, SprintEastYStart, SprintEastXEnd, SprintEastYEnd;
    int SprintSouthEastXStart, SprintSouthEastYStart, SprintSouthEastXEnd, SprintSouthEastYEnd;
    int SprintSouthXStart, SprintSouthYStart, SprintSouthXEnd, SprintSouthYEnd;
    int SprintSouthWestXStart, SprintSouthWestYStart, SprintSouthWestXEnd, SprintSouthWestYEnd;
    int SprintWestXStart, SprintWestYStart, SprintWestXEnd, SprintWestYEnd;
    int SprintNorthWestXStart, SprintNorthWestYStart, SprintNorthWestXEnd, SprintNorthWestYEnd;

    int SlashNorthXStart, SlashNorthYStart, SlashNorthXEnd, SlashNorthYEnd;
    int SlashNorthEastXStart, SlashNorthEastYStart, SlashNorthEastXEnd, SlashNorthEastYEnd;
    int SlashEastXStart, SlashEastYStart, SlashEastXEnd, SlashEastYEnd;
    int SlashSouthEastXStart, SlashSouthEastYStart, SlashSouthEastXEnd, SlashSouthEastYEnd;
    int SlashSouthXStart, SlashSouthYStart, SlashSouthXEnd, SlashSouthYEnd;
    int SlashSouthWestXStart, SlashSouthWestYStart, SlashSouthWestXEnd, SlashSouthWestYEnd;
    int SlashWestXStart, SlashWestYStart, SlashWestXEnd, SlashWestYEnd;
    int SlashNorthWestXStart, SlashNorthWestYStart, SlashNorthWestXEnd, SlashNorthWestYEnd;
};

#endif // PLAYER_HPP