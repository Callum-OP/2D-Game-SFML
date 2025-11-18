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
    bool moving, sprinting, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

    int maxHealth;
    int health;
    int spriteSize;
    int gold;

    // Coordinates for textures
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

    int AttackNorthXStart, AttackNorthYStart, AttackNorthXEnd, AttackNorthYEnd;
    int AttackNorthEastXStart, AttackNorthEastYStart, AttackNorthEastXEnd, AttackNorthEastYEnd;
    int AttackEastXStart, AttackEastYStart, AttackEastXEnd, AttackEastYEnd;
    int AttackSouthEastXStart, AttackSouthEastYStart, AttackSouthEastXEnd, AttackSouthEastYEnd;
    int AttackSouthXStart, AttackSouthYStart, AttackSouthXEnd, AttackSouthYEnd;
    int AttackSouthWestXStart, AttackSouthWestYStart, AttackSouthWestXEnd, AttackSouthWestYEnd;
    int AttackWestXStart, AttackWestYStart, AttackWestXEnd, AttackWestYEnd;
    int AttackNorthWestXStart, AttackNorthWestYStart, AttackNorthWestXEnd, AttackNorthWestYEnd;

    int StandingNorthX, StandingNorthY, StandingNorthWestX, StandingNorthWestY, 
    StandingNorthEastX, StandingNorthEastY, StandingEastX, StandingEastY, 
    StandingSouthEastX, StandingSouthEastY, StandingSouthX, StandingSouthY,
    StandingSouthWestX, StandingSouthWestY, StandingWestX, StandingWestY;
};

#endif // PLAYER_HPP