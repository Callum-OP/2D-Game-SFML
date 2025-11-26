#pragma once
#include <SFML/Graphics.hpp>
#include "Physics.hpp"

enum class EntityType {
    Player, Enemy, Pickup
};

class Entity {
public:
    virtual EntityType type() const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual bool isAttacking() const { return false; }
    virtual float getAttackRadius() const { return 0.f; }
    virtual void takeDamage(int amount) {};
    virtual Object& collider() = 0;

    virtual void update(float dt) = 0;
    virtual void drawShadow(sf::RenderWindow& window) {};
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isDead() const = 0;

    virtual ~Entity() = default;
};