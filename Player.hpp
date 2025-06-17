#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
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
    int joggingNorthXStart, joggingNorthYStart, joggingNorthXEnd, joggingNorthYEnd;
    int joggingNorthEastXStart, joggingNorthEastYStart, joggingNorthEastXEnd, joggingNorthEastYEnd;
    int joggingEastXStart, joggingEastYStart, joggingEastXEnd, joggingEastYEnd;
    int joggingSouthEastXStart, joggingSouthEastYStart, joggingSouthEastXEnd, joggingSouthEastYEnd;
    int joggingSouthXStart, joggingSouthYStart, joggingSouthXEnd, joggingSouthYEnd;
    int joggingSouthWestXStart, joggingSouthWestYStart, joggingSouthWestXEnd, joggingSouthWestYEnd;
    int joggingWestXStart, joggingWestYStart, joggingWestXEnd, joggingWestYEnd;
    int joggingNorthWestXStart, joggingNorthWestYStart, joggingNorthWestXEnd, joggingNorthWestYEnd;
};

#endif // PLAYER_HPP