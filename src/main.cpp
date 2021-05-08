#include "FeasiblePlacementFinder.hpp"
#include "PlacementInstance.hpp"
#include <cassert>
#include <iostream>

int main(int argc, char *args[]) {
  if (argc != 2) {
    throw(std::runtime_error(
        "Please provide exactly one input file as a parameter"));
  }
  PlacementInstance inst{args[1]};
  FeasiblePlacementFinder finder{inst};
  auto placement = finder.find_feasible_placement();
  if (placement) {
    for (auto pos : *placement) {
      std::cout << pos.first << " " << pos.second << std::endl;
    }
    assert(inst.check(*placement));
  } else {
    std::cout << "no feasible placement exists" << std::endl;
  }
}
