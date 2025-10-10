#pragma once
#include <vector>

// Node structure, can be wall or empty
struct Node {
    // Node types
    bool wall = false;
    bool visited = false;
    Node* previousNode = nullptr;
    // G, H and F cost
    float g = std::numeric_limits<float>::infinity();
    float h = 0;
    float f() const { return g + h; }
    // Position of each node
    int x = 0;
    int y = 0;
};

// Grid structure
struct Grid {
    int width, height;
    std::vector<std::vector<Node>> nodes;

    explicit Grid(int w, int h);
    bool inBounds(int x, int y) const;
};

// Function declarations
float estimatedCost(Node* a, Node* b);
std::vector<Node*> getNeighbours(Node* node, Grid& grid);
bool isSpaceFree(int x, int y, Grid& grid, int objW, int objH);
std::vector<Node*> aStar(Node* start, Node* goal, Grid& grid, int objectWidth, int objectHeight);
