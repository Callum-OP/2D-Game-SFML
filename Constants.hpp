#pragma once

#include <map>
#include <utility>

//----TILEMAP----
inline constexpr int TILE_SIZE = 96;


//----PLAYER----

// Size of player sprite texture
constexpr int PLAYER_SPRITE_SIZE = 300;

// Base colour of player sprite texture
constexpr sf::Color PLAYER_BASE_COLOUR = sf::Color::Green;

// Max health player can have
constexpr int PLAYER_MAX_HEALTH = 5;

// Spritesheet boundaries
constexpr int PLAYER_FINAL_COLUMN = 7800; // These values will need to change if the spritesheet changes
constexpr int PLAYER_FINAL_ROW = 1800;

// Timers for animation
constexpr float PLAYER_TIMER = 0.0f;
constexpr float PLAYER_TIMER_MAX = 0.5f;

// Spped of movement for player
constexpr float PLAYER_V_SPEED = 3.0f;
constexpr float PLAYER_H_SPEED = 3.0f;

// Start and end coordinates for animation textures in spritesheet
struct AnimationCoords {
    int xStart, xEnd, yStart, yEnd;
};
enum class Action { Attack, Jog, Sprint, Standing };
enum class Direction { East, SouthEast, South, SouthWest, West, NorthWest, North, NorthEast };

// Action + Direction → Coordinates
const std::map<std::pair<Action, Direction>, AnimationCoords> playerAnimationTable = {
    // Attack
    { {Action::Attack, Direction::East},      {0,    2100, 0,    0} },
    { {Action::Attack, Direction::North},     {2400, 4500, 0,    0} },
    { {Action::Attack, Direction::NorthEast}, {4800, 6600, 0,    0} },
    { {Action::Attack, Direction::NorthWest}, {6900,  900, 0,  300} },
    { {Action::Attack, Direction::South},     {1200, 3300, 300, 300} },
    { {Action::Attack, Direction::SouthEast}, {3600, 5700, 300, 300} },
    { {Action::Attack, Direction::SouthWest}, {6000,    0, 300, 600} },
    { {Action::Attack, Direction::West},      {300,  2400, 600, 600} },
    // Jog
    { {Action::Jog, Direction::East},      {2700, 4200, 600, 600} },
    { {Action::Jog, Direction::North},     {4500, 6000, 600, 600} },
    { {Action::Jog, Direction::NorthEast}, {6300, 7800, 600, 600} },
    { {Action::Jog, Direction::NorthWest}, {   0, 1500, 900, 900} },
    { {Action::Jog, Direction::South},     {1800, 3300, 900, 900} },
    { {Action::Jog, Direction::SouthEast}, {3600, 5100, 900, 900} },
    { {Action::Jog, Direction::SouthWest}, {5400, 6900, 900, 900} },
    { {Action::Jog, Direction::West},      {7200,  600, 900, 1200} },
    // Sprint
    { {Action::Sprint, Direction::East},      { 900, 2400, 1200, 1200} },
    { {Action::Sprint, Direction::North},     {2700, 4200, 1200, 1200} },
    { {Action::Sprint, Direction::NorthEast}, {4500, 6000, 1200, 1200} },
    { {Action::Sprint, Direction::NorthWest}, {6300, 7800, 1200, 1200} },
    { {Action::Sprint, Direction::South},     {   0, 1500, 1500, 1500} },
    { {Action::Sprint, Direction::SouthEast}, {1800, 3300, 1500, 1500} },
    { {Action::Sprint, Direction::SouthWest}, {3600, 5100, 1500, 1500} },
    { {Action::Sprint, Direction::West},      {5400, 6900, 1500, 1500} },
    // Standing (Currently not an animation but a single frame)
    { {Action::Standing, Direction::East},      {7200, 7200, 1500, 1500} },
    { {Action::Standing, Direction::North},     {7500, 7500, 1500, 1500} },
    { {Action::Standing, Direction::NorthEast}, {7800, 7800, 1500, 1500} },
    { {Action::Standing, Direction::NorthWest}, {   0,    0, 1800, 1800} },
    { {Action::Standing, Direction::South},     { 300,  300, 1800, 1800} },
    { {Action::Standing, Direction::SouthEast}, { 600,  600, 1800, 1800} },
    { {Action::Standing, Direction::SouthWest}, { 900,  900, 1800, 1800} },
    { {Action::Standing, Direction::West},      {1200, 1200, 1800, 1800} },
};

//----ENEMY----
