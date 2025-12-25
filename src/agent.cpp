#include "football_game/agent.hpp"

#include <ios>
#include <iostream>

#include "football_game/helpers.hpp"

namespace football {

Agent::Agent(const Pitch& pitch) : pitch_(pitch) {}

PitchController::AgentDecisions Agent::MakeDecisions() {
  using AgentDecisions = PitchController::AgentDecisions;
  // target: pass the ball to the player if the ball is on the left side

  // If ball is on the left side of the field.
  if (pitch_.ball.position.x >= pitch_.length_cm / 2) {
    return AgentDecisions{};
  }

  const auto bot_ball_relation =
      physics2d::Vector<float>(pitch_.ball.position - pitch_.bot.position);
  const auto bot_player_relation =
      physics2d::Vector<float>(pitch_.player.position - pitch_.bot.position)
          .Normalize();

  std::cout << __func__ << "() bot_ball_relation: " << bot_ball_relation
            << ", normalized: " << bot_ball_relation.Normalize()
            << ", length: " << bot_ball_relation.GetLength()
            << ", pitch_.bot.direction: " << pitch_.bot.direction
            << ", bot_player_relation: " << bot_player_relation << "\n";

  // Move towards the ball.
  if (bot_ball_relation.GetLength() > 42.0f) {
    const auto normalized = bot_ball_relation.Normalize();
    return AgentDecisions{
        .player_move = bot_ball_relation.Normalize(),
    };
  }

  // Prevent blocking on the line.
  if (pitch_.ball.position.x == 0.0f ||
      pitch_.ball.position.x == pitch_.length_cm ||
      pitch_.ball.position.y == 0.0f ||
      pitch_.ball.position.y == pitch_.width_cm) {
    return AgentDecisions{
        .player_move =
            {
                .x = 1.0f,
            },
    };
  }

  // Kick if rotated towards the target

  if (std::abs(pitch_.bot.direction.x - bot_player_relation.x) < 0.3f &&
      std::abs(pitch_.bot.direction.y - bot_player_relation.y) < 0.3f) {
    return AgentDecisions{
        .kick = true,
    };
  }

  // Rotate towards the player.
  return AgentDecisions{
      .player_move = bot_player_relation,
  };
}

}  // namespace football
