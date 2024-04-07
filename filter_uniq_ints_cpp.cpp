#include "filter_uniq_ints_cpp.hpp"

#include <unordered_set>

std::vector<int> filter_uniq_cpp(std::span<const int> in) {
  const auto filtered = std::unordered_set<int>{in.begin(), in.end()};
  return {filtered.begin(), filtered.end()};
}