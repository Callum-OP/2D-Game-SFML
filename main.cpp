#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({640, 480}), "2D Game", sf::Style::Titlebar | sf::Style::Close);

    // Create a rectangle
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f)); // Width: 100, Height: 50
    rectangle.setFillColor(sf::Color::Green); // Set its color

    while (window.isOpen())
    {
        while (auto event = window.pollEvent()) 
        {
            if (auto closeEvent = event->getIf<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }
    return 0;
}
