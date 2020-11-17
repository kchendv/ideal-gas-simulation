#pragma once

#include "cinder/gl/gl.h"

namespace idealgas {

/**
 * Representation of a Gas Particle
 */
class Particle {
 public:
   /**
    * Constructs a Particle with a given type, initial position, velocity, color, mass and radius
    * @param type The type of the particle
    * @param position The initial position
    * @param velocity The initial velocity
    * @param color The display color
    * @param radius The radius
    * @param mass The mass
    */
   Particle(size_t type, const glm::vec2& position, const glm::vec2& velocity,
            const ci::Color& color, float radius, double mass);

   /**
    * Updates the Particle's position based on its velocity
    */
   void ProcessMovement();

   /**
    * Update the Particle's velocity if it collides with a vertical wall
    * @param wall_pos The X position of the vertical wall
    */
   void ProcessXWallCollision(double wall_pos);

   /**
   * Update the Particle's velocity if it collides with a horizontal wall
   * @param wall_pos The Y position of the horizontal wall
   */
   void ProcessYWallCollision(double wall_pos);

   // Getters
   size_t GetType() const;
   const glm::vec2& GetPosition() const;
   const glm::vec2& GetVelocity() const;
   const cinder::Color& GetColor() const;
   float GetRadius() const;
   double GetMass() const;

   // Setters
   void SetVelocity(const glm::vec2 &velocity);

private:
   size_t type_;
   glm::vec2 position_;
   glm::vec2 velocity_;
   ci::Color color_;
   float radius_;
   double mass_;
};

/**
 * Checks if two Particles collide
 * @param particle_a A Particle
 * @param particle_b A Particle
 * @return True if the two particles collide
 */
bool CheckCollision(const Particle& particle_a, const Particle& particle_b);

/**
 * Updates the velocities of two colliding particles
 * @param particle_a A Particle
 * @param particle_b A Particle
 */
void CollideParticles(Particle& particle_a, Particle& particle_b);
}  // namespace idealgas

