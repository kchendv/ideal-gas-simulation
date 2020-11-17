#pragma once

#include <core/particle.h>

#include "cinder/gl/gl.h"
#include "histogram.h"

namespace idealgas {

namespace visualizer {

/**
 * Simulation of a ideal gas experiment
 */
class Simulation {
 public:
  /**
   * Constructs a Simulation based on the given box size and number of particles
   * @param top_left_corner The coordinate of the top left corner of the container
   * @param box_width The width of the gas container
   * @param box_height The height of the gas container
   */
  Simulation(const glm::vec2 &top_left_corner,
             double box_width, double box_height);

  /**
   * Draws the next frame of the Simulation
   */
  void Draw() const;

  /**
   * Updates the Simulation between frames
   */
  void Update();

 private:
  std::vector<Particle> particles_;
  std::vector<Histogram> histograms_;

  // Simulation view settings
  glm::vec2 top_left_corner_;
  double box_width_;
  double box_height_;

  // Default particle settings
  const double kMaxSpeedFactor = 0.2;
  struct ParticleConfig {
    size_t type;
    ci::Color color;
    float radius;
    double mass;
    size_t amount;

    ParticleConfig(size_t type, ci::Color color, float radius, double mass, size_t amount) :
            type(type), color(color), radius(radius), mass(mass), amount(amount) {};
  };

  std::vector<ParticleConfig> particle_configs_ {
          ParticleConfig(0, "red", 20, 100, 20),
          ParticleConfig(1, "blue", 10, 50, 10),
          ParticleConfig(2, "green", 10, 500, 5),
          ParticleConfig(3, "yellow", 20, 500, 5)};

  // Default histogram settings
  const size_t kSpeedTicks = 8;
  const double kSpeedInterval = 0.5;
  const size_t kFrequencyTicks = 6;
  const float kHistogramWidth = 150;
  const float kHistogramHeight = 150;
  const float kHistogramSpacing = 90;

  /**
   * Initialises a random set of Particles within the container
   */
  void InitializeParticles();

  /**
   * Initialises a set of empty Histograms, one for each particle type
   */
  void InitializeHistograms();

  /**
   * Updates the position of every Particle in the Simulation
   */
  void ProcessParticleMovement();

  /**
   * Updates the velocity of every Particle in the Simulation based on collisions
   */
  void ProcessParticleCollision();

  /**
   * Updates the Histogram
   */
  void UpdateHistogram();

  /**
   * Generates a random double value in a given range
   * @param min The minimum possible value
   * @param max The maximum possible value
   * @return A random double value
   */
  inline double GenerateRandomDouble(double min, double max);
};

}  // namespace visualizer

}  // namespace idealgas
