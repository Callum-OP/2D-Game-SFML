#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.cpp"
#include "Player.hpp"

int main()
{
    // Create a player sprite
    Player player;

    // Create game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };
    // Ensure window is closed when Escape key is pressed
    const auto onKeyPressed = [&window](const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
            window.close();
    };

    // Game loop
    while (window.isOpen()) {
        window.handleEvents(onClose, onKeyPressed);

        // Handle player controls
        player.handleInput();
        player.update();

        // Create new window with sprite drawn in
        window.clear(sf::Color::White);
        player.draw(window);
        window.display();
    }

    return 0;
}
