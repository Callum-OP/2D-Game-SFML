#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
    void update();
    void draw(sf::RenderWindow& window);
    void animate(int xStart, int xEnd, int yStart, int yEnd, int finalCol);
    private:
    sf::Texture texture;
    sf::Sprite sprite;

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    int finalColumn;
    int walkingDownXStart, walkingDownYStart, walkingDownXEnd, walkingDownYEnd;
    int walkingUpXStart, walkingUpYStart, walkingUpXEnd, walkingUpYEnd;
    int walkingLeftXStart, walkingLeftYStart, walkingLeftXEnd, walkingLeftYEnd;
    int walkingRightXStart, walkingRightYStart, walkingRightXEnd, walkingRightYEnd;
};

#endif // PLAYER_HPP