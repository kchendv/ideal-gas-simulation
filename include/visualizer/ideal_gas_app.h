#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "simulation.h"

namespace idealgas {

namespace visualizer {

/**
 * Visualization of Ideal Gas simulation
 */
class IdealGasApp : public ci::app::App {
 public:
  /**
   * Constructor
   */
  IdealGasApp();

  /**
   * Draws the next frame of the visualization
   */
  void draw() override;

  /**
   * Updates the visualization between each frame
   */
  void update() override;

 private:
  const double kWindowSize = 1000;
  const double kMargin = 100;
  const double kBoxWidth = 600;
  const double kBoxHeight = 600;

  Simulation simulation_;
};

}  // namespace visualizer

}  // namespace idealgas
