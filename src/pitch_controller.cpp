#include "football_game/pitch_controller.hpp"

#include <chrono>
#include <cmath>
#include <iostream>

#include "football_game/helpers.hpp"
#include "football_game/pitch.hpp"

namespace football {

namespace {
constexpr float kPlayerMoveValueCm = 25.0f;

}  // namespace

PitchController::PitchController(Pitch& pitch) : pitch_(pitch) {}

void PitchController::ApplyUserInput(const std::set<UserInput>& user_input) {
  const auto previous_player_position = pitch_.player.position;
  physics2d::Vector<float> move_decision{0, 0};

  for (auto action : user_input) {
    switch (action) {
      // TODO: limit to pitch dimensions
      case UserInput::MoveRight:
        move_decision.x += kPlayerMoveValueCm;
        break;
      case UserInput::MoveLeft:
        move_decision.x -= kPlayerMoveValueCm;
        //
        break;
      case UserInput::MoveUp:
        move_decision.y += kPlayerMoveValueCm;
        break;
      case UserInput::MoveDown:
        move_decision.y -= kPlayerMoveValueCm;
        break;
      case UserInput::Kick:
        std::cout << "ball: " << pitch_.ball.position
                  << ", player: " << pitch_.player.position << "\n";
        const auto player_ball_relation =
            pitch_.ball.position - pitch_.player.position;
        const auto distance_to_ball = player_ball_relation.GetLength();
        std::cout << player_ball_relation
                  << ", distance_to_ball: " << distance_to_ball
                  << ", normalized: " << player_ball_relation.Normalize()
                  << "\n";
        if (distance_to_ball <= 150.0f) {
          std::cout << "Kick the ball\n";
          const auto kick_direction = pitch_.player.direction;
          const float kick_power = 100.0;
          pitch_.ball.velocity = kick_direction * kick_power;
          std::cout << "pitch_.ball.velocity: " << pitch_.ball.velocity << "\n";
        }
        break;
    }
  }

  pitch_.player.position += move_decision.Normalize() * kPlayerMoveValueCm;

  const auto position_change =
      physics2d::Vector<float>{pitch_.player.position -
                               previous_player_position}
          .Normalize();
  // TODO: don't compare floats directly
  // Prevent setting direction to zero.
  if (position_change.x != 0.0f || position_change.y != 0.0f) {
    pitch_.player.direction = position_change;
  }
}

void PitchController::ApplyAgentDecisions(const AgentDecisions& decisions) {
  const auto previous_bot_position = pitch_.bot.position;

  if (decisions.kick) {
    std::cout << "Bot kicks the ball\n";
    const auto kick_direction = pitch_.bot.direction;
    const float kick_power = 100.0;
    pitch_.ball.velocity = kick_direction * kick_power;
    std::cout << "pitch_.ball.velocity: " << pitch_.ball.velocity << "\n";
    return;
  }

  std::cout << __func__
            << "() decisions.player_move: " << decisions.player_move.Normalize()
            << "\n";
  pitch_.bot.position += decisions.player_move.Normalize() * kPlayerMoveValueCm;

  const auto position_change =
      physics2d::Vector<float>{pitch_.bot.position - previous_bot_position}
          .Normalize();
  // TODO: don't compare floats directly
  // Prevent setting direction to zero.
  if (position_change.x != 0.0f || position_change.y != 0.0f) {
    pitch_.bot.direction = position_change;
  }
}

void PitchController::UpdatePitch() {
  const float ball_damping = 0.9f;

  pitch_.ball.position += pitch_.ball.velocity;
  pitch_.ball.velocity *= ball_damping;

  if (pitch_.ball.velocity.GetLength() < 5.0) {
    pitch_.ball.velocity = {0, 0};
  }

  const auto player_ball_relation =
      pitch_.ball.position - pitch_.player.position;
  //   std::cout << player_ball_relation.GetLength() << "\n";
  if (player_ball_relation.GetLength() <= 80.0f) {
    pitch_.ball.velocity = {0, 0};
    pitch_.ball.position =
        pitch_.player.position + pitch_.player.direction * 41.0f;
  }

  const auto bot_ball_relation = pitch_.ball.position - pitch_.bot.position;
  //   std::cout << bot_ball_relation.GetLength() << "\n";
  if (bot_ball_relation.GetLength() <= 80.0f) {
    pitch_.ball.velocity = {0, 0};
    const auto previuos_ball_distance = bot_ball_relation.GetLength();
    pitch_.ball.position = pitch_.bot.position + pitch_.bot.direction * 41.0f;
    // std::cout << __func__ << "() pitch_.bot.direction: " <<
    // pitch_.bot.direction
    //           << ", previuos_ball_distance: " << previuos_ball_distance
    //           << ", new distance: "
    //           << physics2d::Vector<float>(pitch_.ball.position -
    //                                       pitch_.bot.position)
    //                  .GetLength()
    //           << ", change: " << pitch_.bot.direction * 41.0f << "\n";
  }

  // Make the ball bounces from the edges of the pitch for now.
  // TODO: would be better to not mix coordinates with diameters in every
  //       calculation
  if (pitch_.ball.position.y < 0) {
    std::cout << "out Y\n";
    pitch_.ball.velocity.y *= -1.0f;
    pitch_.ball.position.y = 0.0f;
  }
  if (pitch_.ball.position.y > pitch_.width_cm) {
    std::cout << "out Y\n";
    pitch_.ball.velocity.y *= -1.0f;
    pitch_.ball.position.y = pitch_.width_cm;
  }
  if (pitch_.ball.position.x < 0) {
    std::cout << "out X\n";
    pitch_.ball.velocity.x *= -1.0f;
    pitch_.ball.position.x = 0.0f;
  }
  if (pitch_.ball.position.x > pitch_.length_cm) {
    std::cout << "out X\n";
    pitch_.ball.velocity.x *= -1.0f;
    pitch_.ball.position.x = pitch_.length_cm;
  }
}

}  // namespace football