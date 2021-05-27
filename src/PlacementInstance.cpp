#include "PlacementInstance.hpp"
#include <cstdio>
#include <iostream>
#include <optional>

namespace FeasiblePlacementFinder {
PlacementInstance::PlacementInstance(const std::string filename) {
  FILE *file = std::fopen(filename.c_str(), "r");

  if (file == NULL) {
    error("Could not open file");
  }

  auto bad_input = []() { error("Bad input"); };

  // Reads the next rectangle from the file, or returns nullopt, if the input is
  // bad
  auto read_rect = [&]() -> std::optional<Rectangle> {
    Length width, height;

    int result = fscanf(file, "%d%d", &width, &height);
    if (result < 2) {
      return std::nullopt;
    }
    return Rectangle{width, height};
  };

  // Read chip image dimensions
  auto first_rect = read_rect();
  if (not first_rect) {
    bad_input();
  } else {
    _image = *first_rect;
  }

  // Read number of rectangles
  int num_rect;
  int result = fscanf(file, "%d\n", &num_rect);
  if (result == 0) {
    bad_input();
  }

  // Read rectangles one by one
  for (int idx = 0; idx < num_rect; ++idx) {
    auto rect = read_rect();
    if (not rect) {
      bad_input();
    }
    _rectangles.push_back(*rect);
  }
}

bool PlacementInstance::check(
    const std::vector<std::pair<Length, Length>> &placement) const {
  for (RectId i = 0; i < num_rect(); ++i) {
    if (placement.at(i).first < 0 or placement.at(i).second < 0 or
        placement.at(i).first + _rectangles.at(i).width > _image.width or
        placement.at(i).second + _rectangles.at(i).height > _image.height) {
      return false;
    }
    for (RectId j = i + 1; j < num_rect(); ++j) {
      if (not(placement.at(i).first + _rectangles.at(i).width <=
                  placement.at(j).first or
              placement.at(j).first + _rectangles.at(j).width <=
                  placement.at(i).first or
              placement.at(i).second + _rectangles.at(i).height <=
                  placement.at(j).second or
              placement.at(j).second + _rectangles.at(j).height <=
                  placement.at(i).second)) {
        return false;
      }
    }
  }
  return true;
}
} // namespace FeasiblePlacementFinder
