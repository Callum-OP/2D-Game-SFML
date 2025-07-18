#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.cpp"
#include "Player.hpp"

#include "Enemy.cpp"
#include "Enemy.hpp"

int main()
{
    // Create a player sprite
    Player player;

    // Create a enemy sprite
    Enemy enemy;

    // Create game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Timers and keys used for detecting double taps
    sf::Clock clock;
    sf::Time lastClickTime; // The last click of each key
    const sf::Time doubleClickTime = sf::milliseconds(500); // Expected time limit for second click to happen
    sf::Keyboard::Scancode lastDirection = sf::Keyboard::Scancode::Unknown; // The direction of last key press

    // Ensures window closes properly when closed
    const auto onClose = [&window](const sf::Event::Closed&) {
        window.close();
    };
    // Check when key is pressed
    const auto onKeyPressed = [&window, &lastDirection](const sf::Event::KeyPressed& keyPressed) {
        // Ensure window is closed when Escape key is pressed
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
        lastDirection = keyPressed.scancode; // Key that was last clicked
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
            
            sf::Time now = clock.getElapsedTime(); // Current time
            // Sprint only if within expected click time and facing same direction as last click
            if (now - lastClickTime < doubleClickTime && keyPressed.scancode == lastDirection) {
                player.sprint(true);
            } else {
                player.sprint(false);
            }
            lastDirection = keyPressed.scancode; // Key that was last clicked
            lastClickTime = now; // Time user last clicked
        }
    };

    // Game loop
    while (window.isOpen()) {
        sf::Time delta = clock.restart(); // Time since last frame
        float deltaTime = delta.asSeconds(); // Convert to seconds


        window.handleEvents(onClose, onKeyPressed, onKeyReleased);

        // Handle player controls
        player.handleInput();
        player.update();
        enemy.update(deltaTime, player.getPosition());

        // Create new window with sprite drawn in
        window.clear(sf::Color::White);

        player.draw(window);
        enemy.draw(window);

        std::cout << "Player: " << player.getPosition().x << ", " << player.getPosition().y << "\n";
        std::cout << "Enemy: " << enemy.getPosition().x << ", " << enemy.getPosition().y << "\n";
        
        window.display();
    }
    return 0;
}
