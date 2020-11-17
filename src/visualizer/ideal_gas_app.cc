#include <visualizer/ideal_gas_app.h>

namespace idealgas {

namespace visualizer {

IdealGasApp::IdealGasApp()
    : simulation_(glm::vec2(kMargin, kMargin),
                  kBoxWidth, kBoxHeight)  {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
}

void IdealGasApp::draw() {
  ci::Color8u background_color(20, 20, 20);
  ci::gl::clear(background_color);

  simulation_.Draw();
}

void IdealGasApp::update() {
  simulation_.Update();
}
}  // namespace visualizer

}  // namespace idealgas
