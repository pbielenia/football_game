#pragma once

#include <set>

#include "football_game/pitch.hpp"
#include "football_game/pitch_controller.hpp"

namespace football {

class Agent {
 public:
  Agent(const Pitch& pitch, std::set<unsigned> controlled_players);
  std::map<unsigned, PitchController::AgentDecision> MakeDecisions();

 private:
  const Pitch& pitch_;
  const std::set<unsigned> controlled_players_;
};

}  // namespace football
