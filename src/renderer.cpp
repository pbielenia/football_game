#include "football_game/renderer.hpp"

#include <format>
#include <iostream>

#include "football_game/pitch.hpp"
#include "raylib.h"

namespace football {

namespace {
constexpr float kPlayerSizeCm = 60.0;
constexpr float kBallSizeCm = 22.0;
constexpr auto kBallColor = SKYBLUE;

Camera2D GetCamera(const Pitch& pitch,
                   const Renderer::Window& window,
                   float scale) {
  static const auto camera = Camera2D{
      .offset =
          {
              .x = window.width / 2.0f,
              .y = window.height / 2.0f,
          },
      .target =
          {
              .x = pitch.length_cm / 2.0f,
              .y = pitch.width_cm / 2.0f,
          },
      .zoom = scale,
  };
  return camera;
}

float DetermineWindowPitchScale(const Pitch& pitch,
                                const Renderer::Window& window) {
  const float scale_x = static_cast<float>(window.width) / pitch.length_cm;
  const float scale_y = static_cast<float>(window.height) / pitch.width_cm;
  return std::min(scale_x, scale_y);
}

}  // namespace

Renderer::Renderer(const Pitch& pitch, const Window& window)
    : pitch_(pitch),
      window_(window),
      scale_(DetermineWindowPitchScale(pitch_, window_)) {
  InitWindow(window_.width, window.height, window.name.c_str());
  SetTargetFPS(window.target_fps);
}

std::set<PitchController::UserInput> Renderer::GetUserInput() const {
  using UserInput = PitchController::UserInput;

  std::set<UserInput> user_input_set;

  if (IsKeyDown(KEY_RIGHT)) {
    user_input_set.insert(UserInput::MoveRight);
  }
  if (IsKeyDown(KEY_LEFT)) {
    user_input_set.insert(UserInput::MoveLeft);
  }
  if (IsKeyDown(KEY_UP)) {
    user_input_set.insert(UserInput::MoveUp);
  }
  if (IsKeyDown(KEY_DOWN)) {
    user_input_set.insert(UserInput::MoveDown);
  }
  if (IsKeyDown(KEY_SPACE)) {
    user_input_set.insert(UserInput::Kick);
  }

  return user_input_set;
}

void Renderer::RenderFrame() {
  BeginDrawing();
  BeginMode2D(GetCamera(pitch_, window_, scale_));

  ClearBackground(RAYWHITE);

  // Pitch
  DrawRectangle(0, 0, pitch_.length_cm, pitch_.width_cm, DARKGREEN);

  // Player
  DrawCircle(pitch_.player.position.x,
             pitch_.width_cm - pitch_.player.position.y, kPlayerSizeCm / 2,
             RED);
  DrawText(std::format("{:.0f}, {:.0f} cm", pitch_.player.position.x,
                       pitch_.player.position.y)
               .c_str(),
           pitch_.player.position.x + 100,
           pitch_.width_cm - pitch_.player.position.y - 100, 80, RED);
  DrawLine(pitch_.player.position.x, pitch_.width_cm - pitch_.player.position.y,
           pitch_.player.position.x + pitch_.player.direction.x * 100.0f,
           pitch_.width_cm -
               (pitch_.player.position.y + pitch_.player.direction.y * 100.0f),
           RED);

  // Bot
  DrawCircle(pitch_.bot.position.x, pitch_.width_cm - pitch_.bot.position.y,
             kPlayerSizeCm / 2, ORANGE);
  DrawText(std::format("{:.0f}, {:.0f} cm", pitch_.bot.position.x,
                       pitch_.bot.position.y)
               .c_str(),
           pitch_.bot.position.x + 100,
           pitch_.width_cm - pitch_.bot.position.y - 100, 80, ORANGE);
  DrawLine(pitch_.bot.position.x, pitch_.width_cm - pitch_.bot.position.y,
           pitch_.bot.position.x + pitch_.bot.direction.x * 100.0f,
           pitch_.width_cm -
               (pitch_.bot.position.y + pitch_.bot.direction.y * 100.0f),
           ORANGE);

  // Ball
  DrawCircle(pitch_.ball.position.x, pitch_.width_cm - pitch_.ball.position.y,
             kBallSizeCm / 2, kBallColor);
  DrawText(std::format("{:.0f}, {:.0f} cm", pitch_.ball.position.x,
                       pitch_.ball.position.y)
               .c_str(),
           pitch_.ball.position.x + 100,
           pitch_.width_cm - pitch_.ball.position.y - 100, 80, kBallColor);

  DrawText("0, 0 cm", 50, pitch_.width_cm - 100, 80, BLACK);

  DrawLine(pitch_.ball.position.x, pitch_.width_cm - pitch_.ball.position.y,
           pitch_.player.position.x, pitch_.width_cm - pitch_.player.position.y,
           BLACK);

  //   std::cout << "pitch_.player.direction: " << pitch_.player.direction << ",
  //   "
  //             << pitch_.player.position.y + pitch_.player.direction.y * 5.0f
  //             << "\n";

  //   const auto line_x = pitch_.ball.position.x -
  //                       (pitch_.ball.position.x - pitch_.player_position.x) /
  //                       2;
  //   const auto line_y = pitch_.ball.position.y -
  //                       (pitch_.ball.position.y - pitch_.player_position.y) /
  //                       2;
  //   DrawText(std::format("{:.0f}, {:.0f} cm", line_x, line_y).c_str(),
  //   line_x,
  //            pitch_.width_cm - line_y, 80, BLACK);

  EndDrawing();
}

}  // namespace football