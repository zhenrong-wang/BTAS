#include "filter_uniq_ints_cpp.hpp"

#include <unordered_set>
#include <algorithm>

namespace {
struct IdentityHash {
  [[nodiscard]] std::size_t operator()(int i) const noexcept { return i; }
};
}

std::vector<int> filter_uniq_cpp(std::span<const int> in) {
  const auto filtered = std::unordered_set<int, IdentityHash>{in.begin(), in.end()};
  return {filtered.begin(), filtered.end()};
}