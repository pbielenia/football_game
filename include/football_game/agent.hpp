#pragma once

#include "football_game/pitch.hpp"
#include "football_game/pitch_controller.hpp"

namespace football {

class Agent {
 public:
  Agent(const Pitch& pitch);
  PitchController::AgentDecisions MakeDecisions();

 private:
  const Pitch& pitch_;
};

}  // namespace football