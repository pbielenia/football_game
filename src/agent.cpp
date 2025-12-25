#include "football_game/agent.hpp"

#include <ios>
#include <iostream>

#include "football_game/helpers.hpp"

namespace football {

Agent::Agent(const Pitch& pitch, std::set<unsigned> controlled_players)
    : pitch_(pitch),
      controlled_players_(std::move(controlled_players)) {}

std::map<unsigned, PitchController::AgentDecision> Agent::MakeDecisions() {
  using AgentDecisions = std::map<unsigned, PitchController::AgentDecision>;
  const auto& user_controlled_player = pitch_.players.at(0);
  const auto& ball = pitch_.ball;
  // target: pass the ball to the player if the ball is on the left side

  // If ball is on the left side of the field.
  if (ball.position.x >= pitch_.length_cm / 2) {
    return AgentDecisions{};
  }

  //   for (const auto& [player_id, player] : pitch_.players) {
  //     std::cout << __func__ << "() available: " << player_id << "\n";
  //   }

  AgentDecisions decisions;

  for (const auto& player_id : controlled_players_) {
    std::cout << __func__ << "() player: " << player_id << "\n";
    const auto& player = pitch_.players.at(player_id);
    const auto ball_direction =
        physics2d::Vector<float>(ball.position - player.position);
    const auto user_direction =
        physics2d::Vector<float>(user_controlled_player.position -
                                 player.position)
            .Normalize();

    std::cout << __func__ << "() ball_direction: " << ball_direction
              << ", normalized: " << ball_direction.Normalize()
              << ", length: " << ball_direction.GetLength()
              << ", player.direction: " << player.direction
              << ", user_direction: " << user_direction << "\n";

    // Move towards the ball.
    if (ball_direction.GetLength() > 42.0f) {
      decisions[player_id].player_move = ball_direction.Normalize();
      std::cout << __func__ << "():" << __LINE__ << " - continue\n";
      continue;
    };

    // Prevent blocking on the line.
    if (ball.position.x == 0.0f || ball.position.x == pitch_.length_cm ||
        ball.position.y == 0.0f || ball.position.y == pitch_.width_cm) {
      decisions[player_id].player_move = {
          .x = 1.0f,
      };
      std::cout << __func__ << "():" << __LINE__ << " - continue\n";
      continue;
    }

    // Kick if rotated towards the target
    if (std::abs(player.direction.x - user_direction.x) < 0.3f &&
        std::abs(player.direction.y - user_direction.y) < 0.3f) {
      decisions[player_id].kick = true;
      std::cout << __func__ << "():" << __LINE__ << " - continue\n";
      continue;
    }

    // Rotate towards the player.
    std::cout << __func__ << "():" << __LINE__ << " - end\n";
    decisions[player_id].player_move = user_direction;
  }

  //   const auto bot_ball_relation =
  //       physics2d::Vector<float>(pitch_.ball.position - pitch_.bot.position);
  //   const auto bot_player_relation =
  //       physics2d::Vector<float>(pitch_.players.at(0).position -
  //                                pitch_.bot.position)
  //           .Normalize();

  //   std::cout << __func__ << "() bot_ball_relation: " << bot_ball_relation
  //             << ", normalized: " << bot_ball_relation.Normalize()
  //             << ", length: " << bot_ball_relation.GetLength()
  //             << ", pitch_.bot.direction: " << pitch_.bot.direction
  //             << ", bot_player_relation: " << bot_player_relation << "\n";

  //   // Move towards the ball.
  //   if (bot_ball_relation.GetLength() > 42.0f) {
  //     return AgentDecisions{
  //         .player_move = bot_ball_relation.Normalize(),
  //     };
  //   }

  //   // Prevent blocking on the line.
  //   if (pitch_.ball.position.x == 0.0f ||
  //       pitch_.ball.position.x == pitch_.length_cm ||
  //       pitch_.ball.position.y == 0.0f ||
  //       pitch_.ball.position.y == pitch_.width_cm) {
  //     return AgentDecisions{
  //         .player_move =
  //             {
  //                 .x = 1.0f,
  //             },
  //     };
  //   }

  //   // Kick if rotated towards the target

  //   if (std::abs(pitch_.bot.direction.x - bot_player_relation.x) < 0.3f &&
  //       std::abs(pitch_.bot.direction.y - bot_player_relation.y) < 0.3f) {
  //     return AgentDecisions{
  //         .kick = true,
  //     };
  //   }

  //   // Rotate towards the player.
  //   return AgentDecisions{
  //       .player_move = bot_player_relation,
  //   };
  return decisions;
}

}  // namespace football
