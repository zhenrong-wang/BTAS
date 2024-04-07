#ifndef FILTER_UNIQ_INTS_CPP_HPP
#define FILTER_UNIQ_INTS_CPP_HPP

#include <vector>
#include <span>

namespace cpp {

[[nodiscard]] std::vector<int> filter_uniq(std::span<const int> in);
[[nodiscard]] std::vector<int> filter_uniq_sort(std::span<const int> in);
[[nodiscard]] std::vector<int> filter_uniq_ht(std::span<const int> in);

}

#endif //FILTER_UNIQ_INTS_CPP_HPP
