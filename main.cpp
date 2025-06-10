#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
    // Create a player sprite
    sf::Texture texture("spritesheet.png");
    sf::Sprite sprite(texture);

    int texWidth;
    int texHeight;

    sprite.setTextureRect({{485,1}, {240,240}});
    sprite.setOrigin({sprite.getTextureRect().size.x / 2.0f, sprite.getTextureRect().size.y / 2.0f});
    sf::Vector2<float> position(275.f, 200.f); // Set coordinates
    sprite.setPosition(position); // Place sprite at coordinates
    sprite.setScale({1.0f,1.0f});

    float timer = 0.0f;
    float timerMax = 0.5f;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Game loop
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };

    const auto onKeyPressed = [&window](const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
            window.close();
    };

    while (window.isOpen()) {
        window.handleEvents(onClose, onKeyPressed);

        // Remainder of main loop

        // Create new window with sprite drawn in
        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();

        // If left key pressed then move character left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            sprite.move({-2.f, 0.f});
        }
        // If right key pressed then move character right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
        {
            sprite.move({2.f, 0.f});
        }
        // If down key pressed then move character down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
        {
            sprite.move({0.f, 2.f});

            // Walking down animation
            timer += 0.1f;
            if(timer >= timerMax) {
                texWidth += 242;
                // If behind walking down textures, go to start
                if(texHeight == 727) {
                    if(texWidth < 485) {
                        texWidth = 485;
                    }
                }
                // If gone past walking down textures, go back to start
                if(texHeight == 969) {
                    if(texWidth > 727) {
                        texHeight = 727;
                        texWidth = 485;
                    }
                }
                // If not within walking down textures, go to start
                if(texHeight < 727 || texHeight > 969) {
                    texWidth = 485;
                    texHeight = 727; 
                }
                // If texture width reached end of spritesheet
                if(texWidth > 1211) {
                    texWidth = 1;
                    texHeight += 242; 
                }
                // If texture width valid, then change current sprite texture
                if(texWidth <= 1211) {
                    sprite.setTextureRect({{texWidth,texHeight},{240,240}});
                }
                timer = 0.0f;
            }
        }
        // If up key pressed then move character up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
        {
            sprite.move({0.f, -2.f});
        }
    }
}
