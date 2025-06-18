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

    sf::Clock clock;
    sf::Time lastClickTime;
    const sf::Time doubleClickTime = sf::milliseconds(500);
    sf::Keyboard::Scancode lastDirection = sf::Keyboard::Scancode::Unknown;

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };
    // Check when key is pressed
    const auto onKeyPressed = [&window, &player, &clock, &lastClickTime, &doubleClickTime](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
    };
    // Check when key is released
    const auto onKeyReleased = [&window, &player, &clock, &lastClickTime, &doubleClickTime, &lastDirection](const sf::Event::KeyReleased& keyPressed) {
        // If double tap movement keys, tell player to sprint
        if (keyPressed.scancode == sf::Keyboard::Scan::Right || 
            keyPressed.scancode == sf::Keyboard::Scan::D || 
            keyPressed.scancode == sf::Keyboard::Scan::Left || 
            keyPressed.scancode == sf::Keyboard::Scan::A || 
            keyPressed.scancode == sf::Keyboard::Scan::Up || 
            keyPressed.scancode == sf::Keyboard::Scan::W || 
            keyPressed.scancode == sf::Keyboard::Scan::Down || 
            keyPressed.scancode == sf::Keyboard::Scan::S) {
                
            sf::Time now = clock.getElapsedTime();
            if (now - lastClickTime < doubleClickTime && keyPressed.scancode == lastDirection) {
                player.sprint(true);
            } else {
                player.sprint(false);
            }
            lastClickTime = now;
            lastDirection = keyPressed.scancode;
        }
    };

    // Game loop
    while (window.isOpen()) {
        window.handleEvents(onClose, onKeyPressed, onKeyReleased);

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
