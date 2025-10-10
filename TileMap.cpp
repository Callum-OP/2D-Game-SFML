#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <set> 

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Constants.hpp"

// Load map from file
class MapLoader {
public:
    std::vector<std::vector<char>> tileData;

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            tileData.emplace_back(line.begin(), line.end());
        }
    }

    char getTile(int x, int y) const {
        return tileData[y][x];
    }

    sf::Vector2i getSize() const {
        return { static_cast<int>(tileData[0].size()), static_cast<int>(tileData.size()) };
    }

    void setTile(int x, int y, char value) {
        tileData[y][x] = value;
    }   
};

// Tilemap renderer
class TileMapRenderer {
    sf::Texture wallTex;
    sf::Texture floorTex;
    sf::Texture healthPickupTex;
    sf::Texture goldPickupTex;
    sf::Sprite wallSprite;
    sf::Sprite floorSprite;
    sf::Sprite healthPickupSprite;
    sf::Sprite goldPickupSprite;

public:
    // Get tilemap textures
    TileMapRenderer()
        : wallTex{},
          floorTex{},
          healthPickupTex{},
          wallSprite([this]() {
                if (!wallTex.loadFromFile("assets/images/Wall.png"))
                    throw std::runtime_error("Failed to load Wall.png");
                return sf::Sprite(wallTex);
          }()),
            floorSprite([this]() {
                if (!floorTex.loadFromFile("assets/images/Floor.png"))
                    throw std::runtime_error("Failed to load Floor.png");
                return sf::Sprite(floorTex);
          }()),
          healthPickupSprite([this]() {
                if (!healthPickupTex.loadFromFile("assets/images/HealthPickup.png"))
                    throw std::runtime_error("Failed to load HealthPickup.png");
                return sf::Sprite(healthPickupTex);
          }()),
          goldPickupSprite([this]() {
                if (!goldPickupTex.loadFromFile("assets/images/GoldPickup.png"))
                    throw std::runtime_error("Failed to load GoldPickup.png");
                return sf::Sprite(goldPickupTex);
          }())
    {}

    // Draw tilemap
    void draw(sf::RenderWindow& window, const MapLoader& map) {
        for (int y = 0; y < map.getSize().y; ++y) {
            for (int x = 0; x < map.getSize().x; ++x) {
                sf::Sprite* sprite = nullptr;
                char tile = map.getTile(x, y);

                // Always draw floor if it's not a wall
                if (tile != '#') {
                    floorSprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(floorSprite);
                }

                // Draw other tilemap objects
                if (tile == '#') sprite = &wallSprite;
                else if (tile == 'H') sprite = &healthPickupSprite;
                else if (tile == 'G') sprite = &goldPickupSprite;

                if (sprite) {
                    sprite->setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(*sprite);
                }
            }
        }
    }
};