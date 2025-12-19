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

// Store the symbol, texture and type of tiles
struct TileAsset {
    char symbol;
    sf::Texture texture;
    enum class Type { Floor, Wall, Item, Enemy } type;
};

// Tilemap renderer
class TileMapRenderer {
    std::vector<TileAsset> tiles;

public:
    TileMapRenderer() {
        // Load all tile definitions
        loadTile('#', "assets/images/Wall.png", TileAsset::Type::Wall);
        
        loadTile('.', "assets/images/Floor.png", TileAsset::Type::Floor);
        loadTile(',', "assets/images/Grass.png", TileAsset::Type::Floor);

        loadTile('H', "assets/images/HealthPickup.png", TileAsset::Type::Item);
        loadTile('G', "assets/images/GoldPickup.png", TileAsset::Type::Item);
        loadTile('C', "assets/images/ChestClosed.png", TileAsset::Type::Item);
        loadTile('Q', "assets/images/ChestOpenFull.png", TileAsset::Type::Item);
        loadTile('O', "assets/images/ChestOpenEmpty.png",     TileAsset::Type::Item);
    }

    // Add tiles to tile list
    void loadTile(char symbol, const std::string& filename, TileAsset::Type type) {
        TileAsset def;
        def.symbol = symbol;
        if (!def.texture.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load " + filename);
        }
        def.type = type;
        tiles.push_back(std::move(def));
    }

    const TileAsset* find(char symbol) const {
        for (auto& t : tiles) {
            if (t.symbol == symbol) return &t;
        }
        return nullptr;
    }

    // Draw all floors
    void drawFloors(sf::RenderWindow& window, const MapLoader& map) {
        for (int y = 0; y < map.getSize().y; ++y) {
            for (int x = 0; x < map.getSize().x; ++x) {
                char tile = map.getTile(x, y);
                const TileAsset* def = find(tile);
                if (def && def->type == TileAsset::Type::Floor) {
                    sf::Sprite sprite(def->texture);
                    sprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(sprite);
                // Should still draw a floor even if there is another object there
                // It will decide the texture of the floor by looking at nearby floors
                } else {
                    // Count nearby floor symbols
                    std::map<char,int> counts;
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dx == 0 && dy == 0) continue;
                            int nx = x + dx;
                            int ny = y + dy;
                            if (nx >= 0 && ny >= 0 && nx < map.getSize().x && ny < map.getSize().y) {
                                char neighbor = map.getTile(nx, ny);
                                const TileAsset* ndef = find(neighbor);
                                if (ndef && ndef->type == TileAsset::Type::Floor) {
                                    counts[neighbor]++;
                                }
                            }
                        }
                    }
                    // Pick the floor symbol with the highest count
                    char floorSymbol = '.';
                    int bestCount = -1;
                    for (auto& kv : counts) {
                        if (kv.second > bestCount) {
                            bestCount = kv.second;
                            floorSymbol = kv.first;
                        }
                    }
                    const TileAsset* def = find(floorSymbol);
                    if (def && def->type == TileAsset::Type::Floor) {
                        sf::Sprite sprite(def->texture);
                        sprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                        window.draw(sprite);
                    }
                }
            }
        }
    }

    // Draw all walls
    void drawWalls(sf::RenderWindow& window, const MapLoader& map) {
        for (int y = 0; y < map.getSize().y; ++y) {
            for (int x = 0; x < map.getSize().x; ++x) {
                char tile = map.getTile(x, y);
                const TileAsset* def = find(tile);
                if (def && def->type == TileAsset::Type::Wall) {
                    sf::Sprite sprite(def->texture);
                    sprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(sprite);
                }
            }
        }
    }

    // Draw all items
    void drawItems(sf::RenderWindow& window, const MapLoader& map) {
        for (int y = 0; y < map.getSize().y; ++y) {
            for (int x = 0; x < map.getSize().x; ++x) {
                char tile = map.getTile(x, y);
                const TileAsset* def = find(tile);
                if (def && def->type == TileAsset::Type::Item) {
                    sf::Sprite sprite(def->texture);
                    sprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(sprite);
                }
            }
        }
    }
};