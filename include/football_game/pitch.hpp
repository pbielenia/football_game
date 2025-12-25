#pragma once

#include <map>

#include "football_game/helpers.hpp"

namespace football {

// enum class Direction {
//   Up,
//   UpRight,
//   Right,
//   DownRight,
//   Down,
//   DownLeft,
//   Left,
//   UpLeft,
// };

struct Ball {
  physics2d::Point<float> position;
  physics2d::Vector<float> velocity;
  float height_cm{0.0};
};

struct Player {
  physics2d::Point<float> position;
  //   Direction direction;
  physics2d::Vector<float> direction;
};

struct Pitch {
  float width_cm;
  float length_cm;
  Ball ball;
  std::map<unsigned, Player> players;

  // Player player;
  Player bot;
};

}  // namespace football