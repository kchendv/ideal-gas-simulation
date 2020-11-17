#include <visualizer/histogram.h>

namespace idealgas {

namespace visualizer {

using glm::vec2;

Histogram::Histogram(size_t speed_ticks, double speed_interval,
                     size_t frequency_ticks, ci::Color histogram_color) :
  kSpeedTicks(speed_ticks), kSpeedInterval(speed_interval),
  kFrequencyTicks(frequency_ticks), kHistogramColor(histogram_color){

};

void Histogram::Draw(const glm::vec2& offset, float width, float height) const {
  size_t total_frequency = 0;
  for (size_t frequency: frequencies_) {
    total_frequency += frequency;
  }

  DrawBackground(offset, width, height);
  DrawTicks(offset, width, height, total_frequency);
  DrawBars(offset, width, height, total_frequency);
}

void Histogram::ResetCount() {
  frequencies_ = std::vector<size_t> (kSpeedTicks, 0);
}

void Histogram::CountParticle(const Particle &particle) {
  double speed = glm::length(particle.GetVelocity());
  size_t allocated_bin = 0;

  // While the speed is above the minimum value of the next bin, assign to the next bin
  while(speed > (allocated_bin + 1) * kSpeedInterval && allocated_bin < kSpeedTicks - 1) {
    allocated_bin++;
  }
  frequencies_[allocated_bin]++;
}

void Histogram::DrawBackground(const glm::vec2& offset, float width, float height) const {
  ci::Rectf bounding_box(vec2(0, 0) + offset, vec2(width, height) + offset);
  ci::gl::color(ci::Color(kHistogramColor));
  ci::gl::drawStrokedRect(bounding_box, 5);
  ci::gl::color(ci::Color("black"));
  ci::gl::drawSolidRect(bounding_box);

  // Axis labels
  ci::gl::drawStringCentered(kXLabel,
                        vec2(width / 2, height + 2 * kAxisLabelFont.getSize()) + offset,
                       ci::ColorA(1, 1, 1, 1),
                        kAxisLabelFont);

  // Code below derived from:
  // https://discourse.libcinder.org/t/what-is-the-best-way-to-rotate-rectangles-images/410
  ci::gl::translate(offset);
  ci::gl::rotate(-1.5708f); // Rotate pi / 2 radians
  ci::gl::drawStringCentered(kYLabel,
                             vec2(-height / 2, - 3 * kAxisLabelFont.getSize()),
                             ci::ColorA(1, 1, 1, 1),
                             kAxisLabelFont);
  ci::gl::rotate(1.5708f);
  ci::gl::translate(-offset);
}

void Histogram::DrawTicks(const glm::vec2& offset, float width,
                          float height, size_t total_frequency) const {
  ci::gl::color(ci::Color("grey"));
  float y_tick_spacing = height / kFrequencyTicks;
  for (float tick_count = 0; tick_count <= kFrequencyTicks; tick_count++) {
    ci::gl::drawLine(vec2(0, height - (y_tick_spacing * tick_count)) + offset,
                     vec2(width, height - (y_tick_spacing * tick_count)) + offset);

    // Code below derived from:
    // https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
    float tick_label = (tick_count * total_frequency / kFrequencyTicks);
    std::stringstream tick_label_stream;
    tick_label_stream << std::fixed << std::setprecision(1) << tick_label;
    ci::gl::drawStringRight(tick_label_stream.str(),
                       vec2(-5, height - (y_tick_spacing * tick_count) - 10) + offset,
                       ci::ColorA(1, 1, 1, 1),
                       kTickLabelFont);
  }

  float x_tick_spacing = width / kSpeedTicks;
  for (float tick_count = 0; tick_count < kSpeedTicks; tick_count++) {
    ci::gl::drawLine(vec2(x_tick_spacing * tick_count, 0) + offset,
                     vec2(x_tick_spacing * tick_count, height) + offset);

    double tick_label = tick_count * kSpeedInterval;
    std::stringstream tick_label_stream;
    tick_label_stream << std::fixed << std::setprecision(1) << tick_label;
    ci::gl::drawString(tick_label_stream.str(),
                  vec2(x_tick_spacing * tick_count, height + 5) + offset,
                  ci::ColorA(1, 1, 1, 1),
                  kTickLabelFont);
  }
}

void Histogram::DrawBars(const glm::vec2& offset, float width,
                         float height, size_t total_frequency) const {
  ci::gl::color(ci::Color("white"));
  float x_tick_spacing = width / kSpeedTicks;

  for (size_t frequency_bin = 0; frequency_bin < kSpeedTicks; frequency_bin++) {
    float bar_ratio = float(frequencies_[frequency_bin]) / float(total_frequency);
    vec2 bottom_left = vec2(x_tick_spacing * frequency_bin, height) + offset;
    vec2 top_right = vec2(x_tick_spacing * (frequency_bin + 1),
                          height  * (1- bar_ratio)) + offset;

    ci::gl::drawSolidRect(ci::Rectf(bottom_left, top_right));
  }
}

}  // namespace visualizer

}  // namespace idealgas
