#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
    void Player::sprint(bool sprint);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void animate(int xStart, int xEnd, int yStart, int yEnd);
    private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::string direction;

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    int finalColumn, finalRow;
    bool moving, sprinting, attacking, north, northEast, east, southEast, south, southWest, west, northWest;

    int standingNorthX, standingNorthY, standingNorthWestX, standingNorthWestY, 
    standingNorthEastX, standingNorthEastY, standingEastX, standingEastY, 
    standingSouthEastX, standingSouthEastY, standingSouthX, standingSouthY,
    standingSouthWestX, standingSouthWestY, standingWestX, standingWestY;

    int joggingNorthXStart, joggingNorthYStart, joggingNorthXEnd, joggingNorthYEnd;
    int joggingNorthEastXStart, joggingNorthEastYStart, joggingNorthEastXEnd, joggingNorthEastYEnd;
    int joggingEastXStart, joggingEastYStart, joggingEastXEnd, joggingEastYEnd;
    int joggingSouthEastXStart, joggingSouthEastYStart, joggingSouthEastXEnd, joggingSouthEastYEnd;
    int joggingSouthXStart, joggingSouthYStart, joggingSouthXEnd, joggingSouthYEnd;
    int joggingSouthWestXStart, joggingSouthWestYStart, joggingSouthWestXEnd, joggingSouthWestYEnd;
    int joggingWestXStart, joggingWestYStart, joggingWestXEnd, joggingWestYEnd;
    int joggingNorthWestXStart, joggingNorthWestYStart, joggingNorthWestXEnd, joggingNorthWestYEnd;

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