#ifndef PLACEMENT_INSTANCE_HPP
#define PLACEMENT_INSTANCE_HPP

#include <iostream>
#include <string>
#include <vector>

using Length = int;
using RectId = int;

struct Rectangle {
  Length width;
  Length height;
};

inline void error(std::string msg) {
  std::cerr << "Error: " << msg << std::endl;
  exit(1);
}

class PlacementInstance {

public:
  // Reads a placement instance from the given filename
  PlacementInstance(const std::string filename);

  RectId num_rect() const { return _rectangles.size(); }
  Length width(const RectId rect) const { return _rectangles.at(rect).width; }
  Length height(const RectId rect) const { return _rectangles.at(rect).height; }
  const Rectangle &chip_image() const { return _image; }

  // Checks whether the given placement is a feasible solution of the placement
  // instance
  bool check(const std::vector<std::pair<Length, Length>> &placement) const;

private:
  Rectangle _image;
  std::vector<Rectangle> _rectangles;
};
#endif
