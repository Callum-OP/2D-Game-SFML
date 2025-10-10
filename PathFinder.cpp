#include "PathFinder.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

// Grid constructor, made up of nodes
Grid::Grid(int w, int h) : width(w), height(h), nodes(h, std::vector<Node>(w)) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            nodes[y][x].x = x;
            nodes[y][x].y = y;
            nodes[y][x].g = std::numeric_limits<float>::infinity();
            nodes[y][x].h = 0;
        }
    }
}

// Check if a position is inside the grid bounds
bool Grid::inBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Function to estimate the h cost from node a to node b
float estimatedCost(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

// Function to get the neighbours of a node (does not include diagonal neighbours)
std::vector<Node*> getNeighbours(Node* node, Grid& grid) {
    std::vector<Node*> neighbours;
    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];
        if (grid.inBounds(nx, ny)) {
            neighbours.push_back(&grid.nodes[ny][nx]);
        }
    }
    return neighbours;
}

// Function to see if there is enough space for the object to fit through the path
bool isSpaceFree(int x, int y, Grid& grid, int objW, int objH) {
    for (int dy = 0; dy < objH; ++dy) {
        for (int dx = 0; dx < objW; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (!grid.inBounds(nx, ny) || grid.nodes[ny][nx].wall) {
                return false;
            }
        }
    }
    return true;
}

// The A Star pathfinding algorithm
std::vector<Node*> aStar(Node* start, Node* goal, Grid& grid, int objectWidth, int objectHeight) {
    // List of nodes to be checked
    std::vector<Node*> openSet;
    start->g = 0;
    start->h = estimatedCost(start, goal);
    openSet.push_back(start);
    Node* closestToGoal = start;
    float lowestH = start->h;

    while (!openSet.empty()) {
        // Sort list of nodes by lowest F cost
        std::sort(openSet.begin(), openSet.end(), [](Node* a, Node* b) {
            return a->f() < b->f();
        });

        Node* current = openSet.front();
        openSet.erase(openSet.begin());

        // If the goal has been reached, reconstruct the path
        if (current == goal) {
            std::vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->previousNode;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        current->visited = true;

        // Check each neighbour
        for (Node* neighbour : getNeighbours(current, grid)) {
            // Complete checks
            if (neighbour->x + objectWidth > grid.width || neighbour->y + objectHeight > grid.height) continue;
            if (!isSpaceFree(neighbour->x, neighbour->y, grid, objectWidth, objectHeight)) continue;
            float tentativeG = current->g + 1; // Distance/cost between neighbouring nodes
            // If the cost is less (if this path to neighbor is better), then record it
            if (tentativeG < neighbour->g) {
                neighbour->previousNode = current;
                neighbour->g = tentativeG;
                neighbour->h =  estimatedCost(neighbour, goal);
                // Add neighbour to list of nodes if not already on it
                if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) {
                    openSet.push_back(neighbour);
                }
                // Get neighbour closest to goal, for checking if goal is not possible to reach
                if (neighbour->h < lowestH) {
                    lowestH = neighbour->h;
                    closestToGoal = neighbour;
                }
            }
        }
    }
    // If no path to goal then return path to closest possible node
    if (closestToGoal != start && closestToGoal->previousNode != nullptr) {
        std::vector<Node*> path;
        Node* current = closestToGoal;
        while (current != nullptr) {
            path.push_back(current);
            current = current->previousNode;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    return {}; // No path found
}
