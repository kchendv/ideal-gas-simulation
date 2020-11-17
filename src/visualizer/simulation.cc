#include <visualizer/simulation.h>

namespace idealgas {

namespace visualizer {

using glm::vec2;

Simulation::Simulation(const glm::vec2 &top_left_corner,
                       double box_width, double box_height)
    : top_left_corner_(top_left_corner),
      box_width_(box_width),
      box_height_(box_height) {
    InitializeParticles();
    InitializeHistograms();
}

void Simulation::Draw() const {
  ci::gl::color(ci::Color("white"));
  ci::Rectf gas_box(top_left_corner_, top_left_corner_ + vec2(box_width_,box_height_));
  ci::gl::drawStrokedRect(gas_box, 5);

  for (const Particle& particle : particles_) {
    ci::gl::color(particle.GetColor());
    ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadius());
  }

  for (size_t i = 0; i < histograms_.size(); i++) {
    histograms_[i].Draw(top_left_corner_ +
      vec2(box_width_ + kHistogramSpacing, (kHistogramHeight + kHistogramSpacing) * i),
      kHistogramWidth, kHistogramHeight);
  }
}

void Simulation::Update() {
  ProcessParticleMovement();
  ProcessParticleCollision();
  UpdateHistogram();
}

void Simulation::InitializeParticles() {
  srand((unsigned int)time(0));
  std::vector<Particle> initial_particles;
  for (const ParticleConfig& particle_type : particle_configs_) {
    for (size_t i = 0; i < particle_type.amount; i++) {
      double x_pos = GenerateRandomDouble(top_left_corner_.x, top_left_corner_.x + box_width_);
      double y_pos = GenerateRandomDouble(top_left_corner_.y, top_left_corner_.y + box_height_);

      double max_velocity = particle_type.radius * kMaxSpeedFactor;
      double min_velocity = -particle_type.radius * kMaxSpeedFactor;
      double x_vel = GenerateRandomDouble(min_velocity, max_velocity);
      double y_vel = GenerateRandomDouble(min_velocity, max_velocity);

      initial_particles.emplace_back(particle_type.type,
                                     vec2(x_pos, y_pos),
                              vec2(x_vel, y_vel),
                                     particle_type.color,
                                     particle_type.radius,
                                     particle_type.mass);
    }
  }

  particles_ = std::move(initial_particles);
}

void Simulation::InitializeHistograms() {
  for (ParticleConfig& particle_config : particle_configs_) {
    histograms_.emplace_back(kSpeedTicks, kSpeedInterval, kFrequencyTicks, particle_config.color);
  }
}

void Simulation::ProcessParticleMovement() {
  for (Particle& particle : particles_) {
    particle.ProcessMovement();
  }
}

void Simulation::ProcessParticleCollision() {
  for (size_t i = 0; i < particles_.size(); i++) {
    particles_[i].ProcessXWallCollision(top_left_corner_.x);
    particles_[i].ProcessXWallCollision(top_left_corner_.x + box_width_);
    particles_[i].ProcessYWallCollision(top_left_corner_.y);
    particles_[i].ProcessYWallCollision(top_left_corner_.y + box_height_);

    for (size_t j = i + 1; j < particles_.size(); j++) {
     if (CheckCollision(particles_[i], particles_[j])) {
       CollideParticles(particles_[i], particles_[j]);
     }
    }
  }
}

void Simulation::UpdateHistogram() {
  for (Histogram& histogram : histograms_) {
    histogram.ResetCount();
  }

  for (const Particle& particle : particles_) {
    histograms_[particle.GetType()].CountParticle(particle);
  }
}

inline double Simulation::GenerateRandomDouble(double min, double max) {
  // Code below derived from:
  // https://blog.gtwang.org/programming/c-cpp-rand-random-number-generation-tutorial-examples/
  return (max - min) * rand() / (RAND_MAX + 1.0) + min;
}
}  // namespace visualizer

}  // namespace idealgas
