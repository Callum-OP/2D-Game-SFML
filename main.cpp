#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "2D Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Create a rectangle in centre of window
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f)); // Set width and height
    rectangle.setFillColor(sf::Color::Green); // Set colour
    sf::Vector2<float> position(275.f, 200.f); // Set coordinates
    rectangle.setPosition(position); // Place rectangle at coordinates

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

        // Create new window with rectangle drawn in
        window.clear();
        window.draw(rectangle);
        window.display();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            // If left key pressed then move character left
            rectangle.move({-1.f, 0.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
        {
            // If right key pressed then move character right
            rectangle.move({1.f, 0.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
        {
            // If down key pressed then move character down
            rectangle.move({0.f, 1.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
        {
            // If up key pressed then move character up
            rectangle.move({0.f, -1.f});
        }
    }
}
