#pragma once

#include <core/particle.h>

#include "cinder/gl/gl.h"

namespace idealgas {

namespace visualizer {

/**
 * A histogram of speed distribution of a Particle
 */
class Histogram {
 public:
  /**
   * Constructs a Histogram based on the given number of ticks on each axis
   * The interval of the frequency bins and the background color
   * @param speed_ticks
   * @param speed_interval
   * @param frequency_ticks
   * @param histogram_color
   */
  Histogram(size_t speed_ticks, double speed_interval,
            size_t frequency_ticks, ci::Color histogram_color);

  /**
   * Draws the Histogram
   * @param offset The offset from the top left corner of the window
   * @param width The width of the histogram
   * @param height The height of the histogram
   */
  void Draw(const glm::vec2& offset, float width, float height) const;

  /**
   * Clears the particle count in the Histogram
   */
  void ResetCount();

  /**
   * Counts a particle towards a certain frequency bin
   * @param particle The incremented particle
   */
  void CountParticle(const Particle& particle);

 private:
  const size_t kSpeedTicks;
  const double kSpeedInterval;
  const size_t kFrequencyTicks;
  const ci::Color kHistogramColor;
  std::vector<size_t> frequencies_;

  // Labels and Font setting
  const std::string kXLabel = "Speed";
  const std::string kYLabel = "Frequency";
  const ci::Font kAxisLabelFont = ci::Font("Arial", 20);
  const ci::Font kTickLabelFont = ci::Font("Arial", 15);

  /**
   * Draws the Histogram background
   * @param offset The offset from the top left corner of the window
   * @param width The width of the histogram
   * @param height The height of the histogram
   */
  void DrawBackground(const glm::vec2& offset, float width, float height) const;

  /**
   * Draws the tick lines and labels of the Histogram
   * @param offset The offset from the top left corner of the window
   * @param width The width of the histogram
   * @param height The height of the histogram
   * @param total_frequency The total frequency of all frequency bins
   */
  void DrawTicks(const glm::vec2& offset, float width,
                 float height, size_t total_frequency) const;

  /**
   * Draws the Histogram bars
   * @param offset The offset from the top left corner of the window
   * @param width The width of the histogram
   * @param height The height of the histogram
   * @param total_frequency The total frequency of all frequency bins
   */
  void DrawBars(const glm::vec2& offset, float width,
                float height, size_t total_frequency) const;
};

}  // namespace visualizer

}  // namespace idealgas
