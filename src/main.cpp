#include "FeasiblePlacementFinder.hpp"
#include <cassert>
#include <iostream>

using namespace FeasiblePlacementFinder;

int main(int argc, char *args[]) {
  if (argc != 2) {
    throw(std::runtime_error(
        "Please provide exactly one input file as a parameter"));
  }
  PlacementInstance inst{args[1]};
  auto placement = find_feasible_placement(inst);
  if (placement) {
    for (auto pos : *placement) {
      std::cout << pos.first << " " << pos.second << std::endl;
    }
    assert(inst.check(*placement));
  } else {
    std::cout << "no feasible placement exists" << std::endl;
  }
}
