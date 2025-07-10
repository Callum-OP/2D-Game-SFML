#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
    void Player::sprint(bool sprint);
    void update();
    void draw(sf::RenderWindow& window);
    void animate(int xStart, int xEnd, int yStart, int yEnd);
    private:
    sf::Texture texture;
    sf::Sprite sprite;

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    int finalColumn, finalRow;
    bool moving, sprinting, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

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

    int sprintingNorthXStart, sprintingNorthYStart, sprintingNorthXEnd, sprintingNorthYEnd;
    int sprintingNorthEastXStart, sprintingNorthEastYStart, sprintingNorthEastXEnd, sprintingNorthEastYEnd;
    int sprintingEastXStart, sprintingEastYStart, sprintingEastXEnd, sprintingEastYEnd;
    int sprintingSouthEastXStart, sprintingSouthEastYStart, sprintingSouthEastXEnd, sprintingSouthEastYEnd;
    int sprintingSouthXStart, sprintingSouthYStart, sprintingSouthXEnd, sprintingSouthYEnd;
    int sprintingSouthWestXStart, sprintingSouthWestYStart, sprintingSouthWestXEnd, sprintingSouthWestYEnd;
    int sprintingWestXStart, sprintingWestYStart, sprintingWestXEnd, sprintingWestYEnd;
    int sprintingNorthWestXStart, sprintingNorthWestYStart, sprintingNorthWestXEnd, sprintingNorthWestYEnd;

    int attackNorthXStart, attackNorthYStart, attackNorthXEnd, attackNorthYEnd;
    int attackNorthEastXStart, attackNorthEastYStart, attackNorthEastXEnd, attackNorthEastYEnd;
    int attackEastXStart, attackEastYStart, attackEastXEnd, attackEastYEnd;
    int attackSouthEastXStart, attackSouthEastYStart, attackSouthEastXEnd, attackSouthEastYEnd;
    int attackSouthXStart, attackSouthYStart, attackSouthXEnd, attackSouthYEnd;
    int attackSouthWestXStart, attackSouthWestYStart, attackSouthWestXEnd, attackSouthWestYEnd;
    int attackWestXStart, attackWestYStart, attackWestXEnd, attackWestYEnd;
    int attackNorthWestXStart, attackNorthWestYStart, attackNorthWestXEnd, attackNorthWestYEnd;
};

#endif // PLAYER_HPP