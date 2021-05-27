#ifndef FEASIBLE_PLACEMENT_FINDER_HPP
#define FEASIBLE_PLACEMENT_FINDER_HPP

#include "PlacementInstance.hpp"
#include <optional>

namespace FeasiblePlacementFinder {
#ifdef __cpp_concepts
template <typename F>
concept EdgeBetweenFunct = requires(F f, RectId a, RectId b) {
  { f(a, b) }
  ->std::convertible_to<bool>;
};

template <typename F> concept LengthInDimFunct = requires(F f, RectId a) {
  { f(a) }
  ->std::convertible_to<Length>;
};
#else
#define EdgeBetweenFunct typename
#define LengthInDimFunct typename
#endif

std::optional<std::vector<std::pair<Length, Length>>>
find_feasible_placement(const PlacementInstance &placement_instance);
} // namespace FeasiblePlacementFinder

#endif
