#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({640, 480}), "2D Game", sf::Style::Titlebar | sf::Style::Close);

    // Create a rectangle in centre of window
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f)); // Set width and height
    rectangle.setFillColor(sf::Color::Green); // Set colour
    sf::Vector2<float> position(275.f, 200.f); // Set coordinates
    rectangle.setPosition(position); // Place rectangle at coordinates

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            // If window is closed
            if (auto closeEvent = event->getIf<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Create new window with rectangle drawn in
        window.clear();
        window.draw(rectangle);
        window.display();
    }
    return 0;
}
