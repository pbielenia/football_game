#pragma once

#include <set>
#include <string>

#include "football_game/pitch.hpp"
#include "football_game/pitch_controller.hpp"

namespace football {

class Renderer {
 public:
  struct Window {
    unsigned width;
    unsigned height;
    std::string name;
    unsigned target_fps;
  };

  Renderer(const Pitch& pitch, const Window& window);

  std::set<PitchController::UserInput> GetUserInput() const;
  void RenderFrame();

 private:
  const Pitch& pitch_;
  Window window_;
  float scale_;
};

}  // namespace football