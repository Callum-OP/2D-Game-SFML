#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
    void update();
    void draw(sf::RenderWindow& window);
    private:
    sf::Texture texture;
    sf::Sprite sprite;

    int textureX, textureY;
    float timer, timerMax;
    float verticalSpeed, horizontalSpeed;
    int finalColumn;
    int walkingDownXStart, walkingDownYStart, walkingDownXEnd, walkingDownYEnd;
};

#endif // PLAYER_HPP