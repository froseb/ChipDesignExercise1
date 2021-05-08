#include "FeasiblePlacementFinder.hpp"
#include <functional>
#include <iostream>

// Computes a vector that contains a topological order of the rectangles, i.e. a
// rectangle r1 being before another rectangle r2 means that r1 is "above" r2 in
// the topological order
template <EdgeBetweenFunct EBF>
std::optional<std::vector<RectId>> topological_order(const int num_rect,
                                                     EBF edge_between) {
  // store the time in the DFS when all children have been iterated through ->
  // indices in the topological order
  struct NodeInfo {
    bool reached = false;
    int current_child = 0;
    int topological_index;
  };

  std::vector<NodeInfo> node_info(num_rect);
  int cur_top_idx = 0;

  for (RectId root = 0; root < num_rect; ++root) {
    if (node_info[root].reached) {
      continue;
    }
    std::vector<RectId> stack = {root};
    while (stack.size() > 0) {
      RectId cur_rect = stack.back();
      node_info[cur_rect].reached = true;
      while (node_info[cur_rect].current_child < num_rect and
             (not edge_between(cur_rect, node_info[cur_rect].current_child) or
              node_info[node_info[cur_rect].current_child].reached)) {
        ++node_info[cur_rect].current_child;
      }
      if (node_info[cur_rect].current_child < num_rect) {
        stack.push_back(node_info[cur_rect].current_child);
      } else {
        node_info[cur_rect].topological_index = cur_top_idx;
        ++cur_top_idx;
        stack.pop_back();
      }
    }
  }

  // Check for violations of the topological order, if there is one this means
  // that there is a directed cycle
  for (RectId i = 0; i < num_rect; ++i) {
    for (RectId j = 0; j < num_rect; ++j) {
      if (edge_between(i, j) and
          node_info[i].topological_index < node_info[j].topological_index) {
        return std::nullopt;
      }
    }
  }

  std::vector<RectId> result(num_rect);
  for (RectId i = 0; i < num_rect; ++i) {
    result[num_rect - node_info[i].topological_index - 1] = i;
  }
  return result;
}

// Finds a feasible potential in the DAG generated by edge_between with
// edge_between(r1, r2) == true iff there is an edge from r1 to r2 or returns
// std::nullopt if the graph is not acyclic or the total length exceeds
// length_in_dim
template <EdgeBetweenFunct EBF, LengthInDimFunct LF>
std::optional<std::vector<Length>>
find_feasible_potential(const int num_rect, EBF edge_between, LF length_in_dim,
                        const Length max_length) {
  const auto top_order = topological_order(num_rect, edge_between);
  if (not top_order) {
    return std::nullopt;
  }

  std::vector<Length> result(num_rect);
  Length total_length = 0;
  for (const RectId r : *top_order) {
    total_length = std::max(total_length, result[r] + length_in_dim(r));
    if (r == top_order->back()) {
      break;
    }
    for (RectId t = 0; t < num_rect; ++t) {
      if (edge_between(r, t)) {
        result[t] = std::max(result[t], result[r] + length_in_dim(r));
      }
    }
  }
  if (total_length <= max_length) {
    return result;
  } else {
    return std::nullopt;
  }
}

std::optional<std::vector<std::pair<Length, Length>>>
FeasiblePlacementFinder::find_feasible_placement() {
  std::vector<RectId> south_or_west_perm(_placement_instance.num_rect());
  std::vector<RectId> north_or_west_perm(_placement_instance.num_rect());
  for (RectId i = 0; i < _placement_instance.num_rect(); ++i) {
    south_or_west_perm[i] = i;
    north_or_west_perm[i] = i;
  }

  std::optional<std::vector<Length>> res_x_pot = std::nullopt;
  std::optional<std::vector<Length>> res_y_pot = std::nullopt;

  // Iterate through all pairs of permutations
  do {
    do {
      auto west = [&](RectId a, RectId b) {
        return south_or_west_perm[a] < south_or_west_perm[b] and
               north_or_west_perm[a] < north_or_west_perm[b];
      };

      auto south = [&](RectId a, RectId b) {
        return south_or_west_perm[a] < south_or_west_perm[b] and
               north_or_west_perm[a] > north_or_west_perm[b];
      };

      // Check if each pair of rectangles has a relation
      for (RectId i = 0; i < _placement_instance.num_rect(); ++i) {
        for (RectId j = 0; j < _placement_instance.num_rect(); ++j) {
          if (not(west(i, j) or west(j, i) or south(i, j) or south(j, i))) {
            continue;
          }
        }
      }

      // Compute feasible potential in x direction
      auto x_pot = find_feasible_potential(
          _placement_instance.num_rect(), west,
          [&](RectId a) { return _placement_instance.width(a); },
          _placement_instance.chip_image().width);
      if (not x_pot) {
        continue;
      }

      // Compute feasible potential in y direction
      auto y_pot = find_feasible_potential(
          _placement_instance.num_rect(), south,
          [&](RectId a) { return _placement_instance.height(a); },
          _placement_instance.chip_image().height);
      if (not y_pot) {
        continue;
      }
      res_x_pot = x_pot;
      res_y_pot = y_pot;
      break;
    } while (std::next_permutation(south_or_west_perm.begin(),
                                   south_or_west_perm.end()));
    if (res_x_pot) {
      break;
    }
  } while (std::next_permutation(north_or_west_perm.begin(),
                                 north_or_west_perm.end()));

  // Combine the potentials in x and y direction to a placement
  if (res_x_pot) {
    std::vector<std::pair<Length, Length>> result;
    result.reserve(_placement_instance.num_rect());

    for (RectId i = 0; i < _placement_instance.num_rect(); ++i) {
      result.push_back({res_x_pot->at(i), res_y_pot->at(i)});
    }
    return result;
  }

  return std::nullopt;
}