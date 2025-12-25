#pragma once

#include <set>

#include "football_game/helpers.hpp"
#include "football_game/pitch.hpp"

namespace football {

class PitchController {
 public:
  enum class UserInput {
    MoveRight,
    MoveLeft,
    MoveUp,
    MoveDown,
    Kick,
  };

  struct AgentDecisions {
    physics2d::Vector<float> player_move = {0, 0};
    bool kick = false;
  };

  PitchController(Pitch& pitch);

  void ApplyUserInput(const std::set<UserInput>& user_input);
  void ApplyAgentDecisions(const AgentDecisions& input);
  void UpdatePitch();

 private:
  //   void MovePlayer
  bool IsInField(const physics2d::Point<float>& point) const;

  Pitch& pitch_;
};

}  // namespace football
