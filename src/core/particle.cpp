#include <core/particle.h>

namespace idealgas {
using glm::vec2;

Particle::Particle(size_t type, const glm::vec2& position, const glm::vec2& velocity,
                   const ci::Color& color, float radius, double mass) :
        type_(type), position_(position), velocity_(velocity), color_(color), radius_(radius), mass_(mass) {}

void Particle::ProcessMovement() {
  position_ += velocity_;
}

void Particle::ProcessXWallCollision(double wall_pos) {
  // Check that the Particle is within (radius) distance of the wall
  // and is moving towards the wall
  if (abs(position_.x - wall_pos) <= radius_
  && (position_.x - wall_pos) * (velocity_.x) < 0) {
    velocity_.x *= -1;
  }
}

void Particle::ProcessYWallCollision(double wall_pos) {
  // Check that the Particle is within (radius) distance of the wall
  // and is moving towards the wall
  if (abs(position_.y - wall_pos) <= radius_
  && (position_.y - wall_pos) * (velocity_.y) < 0) {
    velocity_.y *= -1;
  }
}

size_t Particle::GetType() const {
  return type_;
}

const glm::vec2& Particle::GetPosition() const {
  return position_;
}

const glm::vec2& Particle::GetVelocity() const {
  return velocity_;
}

const cinder::Color& Particle::GetColor() const {
  return color_;
}

float Particle::GetRadius() const {
  return radius_;
}

double Particle::GetMass() const {
  return mass_;
}
void Particle::SetVelocity(const vec2& velocity) {
  velocity_ = velocity;
}

bool CheckCollision(const Particle& particle_a, const Particle& particle_b) {
  // Check that the Particles are with (sum of radius) distance with each other
  // and is moving towards each other
  return (distance(particle_a.GetPosition(), particle_b.GetPosition())
        <= particle_a.GetRadius() + particle_b.GetRadius()
      && dot((particle_a.GetVelocity() - particle_b.GetVelocity()),
             (particle_a.GetPosition() - particle_b.GetPosition())) < 0);
}

void CollideParticles(Particle &particle_a, Particle &particle_b) {
  vec2 old_velocity = particle_a.GetVelocity();
  particle_a.SetVelocity(old_velocity -
            (particle_a.GetPosition() - particle_b.GetPosition()) *
            float (dot((old_velocity - particle_b.GetVelocity()),
                       (particle_a.GetPosition() - particle_b.GetPosition())) /
            pow(distance(particle_a.GetPosition(), particle_b.GetPosition()), 2) *
            (2 * particle_b.GetMass() / (particle_a.GetMass() + particle_b.GetMass()))));

  particle_b.SetVelocity(particle_b.GetVelocity() -
            (particle_b.GetPosition() - particle_a.GetPosition()) *
            float (dot((particle_b.GetVelocity() - old_velocity),
                       (particle_b.GetPosition() - particle_a.GetPosition())) /
            pow(distance(particle_b.GetPosition(), particle_a.GetPosition()), 2) *
            (2 * particle_a.GetMass() / (particle_a.GetMass() + particle_b.GetMass()))));
}
}  // namespace idealgas