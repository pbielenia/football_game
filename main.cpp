#include <iostream>

#include "football_game/agent.hpp"
#include "football_game/pitch.hpp"
#include "football_game/pitch_controller.hpp"
#include "football_game/renderer.hpp"
#include "raylib.h"
//
//  ┼───────────────────── x
//  │  w  ┏━━━━━━┯━━━━━━┓
//  │  i  ┠ ┐    │    ┌ ┨
//  │  d  ┃ │    │    │ ┃
//  │  t  ┠ ┘    │    └ ┨
//  │  h  ┗━━━━━━┷━━━━━━┛
//  y          length
//

// TODO:
// [ ] Kicking the ball using user input.

int main() {
  football::Pitch pitch{.width_cm = 2750,
                        .length_cm = 3650,
                        .ball{
                            .position = {.x = 600, .y = 2000},
                        },
                        .player =
                            {
                                .position = {.x = 2000, .y = 1600},
                                .direction = {.x = 1.0f, .y = 0.0f},
                            },
                        .bot = {
                            .position = {.x = 500, .y = 1500},
                            .direction = {.x = 1.0f, .y = 0.0f},
                        }};

  football::Renderer renderer{pitch,
                              {
                                  .width = 1280,
                                  .height = 720,
                                  .name = "Football Game",
                                  .target_fps = 15,
                              }};

  football::PitchController pitch_controller{pitch};
  football::Agent agent{pitch};

  // TODO: move to renderer
  while (!WindowShouldClose()) {
    pitch_controller.ApplyUserInput(renderer.GetUserInput());
    pitch_controller.ApplyAgentDecisions(agent.MakeDecisions());
    pitch_controller.UpdatePitch();
    renderer.RenderFrame();
  }

  CloseWindow();
}
