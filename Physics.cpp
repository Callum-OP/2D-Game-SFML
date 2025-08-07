#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Physics.hpp"

// Compare objects
bool AABBvsAABB( Manifold *m )
{
  // Setup a couple pointers to each object 
  Object *A = m->A;
  Object *B = m->B;
 
  // Vector from A to B 
  Vec2 n = B->pos - A->pos;
 
  AABB abox = A->aabb;
  AABB bbox = B->aabb;
 
  // Calculate half extents along x axis for each object 
  float a_extent = (abox.max.x - abox.min.x) / 2;
  float b_extent = (bbox.max.x - bbox.min.x) / 2;
 
  // Calculate overlap on x axis 
  float x_overlap = a_extent + b_extent - abs( n.x );
 
  // SAT test on x axis 
  if(x_overlap > 0)
  {
    // Calculate half extents along x axis for each object 
    float a_extent = (abox.max.y - abox.min.y) / 2;
    float b_extent = (bbox.max.y - bbox.min.y) / 2;
 
    // Calculate overlap on y axis 
    float y_overlap = a_extent + b_extent - abs( n.y );
 
    // SAT test on y axis 
    if(y_overlap > 0)
    {
      // Find out which axis is axis of least penetration 
      if(x_overlap > y_overlap)
      {
        // Point towards B knowing that n points from A to B 
        if(n.x < 0)
          m->normal = Vec2( -1, 0 );
        else
          m->normal = Vec2( 0, 0 );
        m->penetration = x_overlap;
        return true;
      }
      else
      {
        // Point toward B knowing that n points from A to B 
        if(n.y < 0)
          m->normal = Vec2( 0, -1 );
        else
          m->normal = Vec2( 0, 1 );
        m->penetration = y_overlap;
        return true;
      }
      return false;
    }
    return false;
  }
  return false;
};

// Function to convert Vec2 to sf::Vector2f
sf::Vector2f toSF(const Vec2& v) {
    return sf::Vector2f(v.x, v.y);
}

// Function to get dot product 
float Dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}