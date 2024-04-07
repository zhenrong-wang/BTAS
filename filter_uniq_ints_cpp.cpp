#include "filter_uniq_ints_cpp.hpp"

#include <unordered_set>
#include <algorithm>

namespace {
struct IdentityHash {
  [[nodiscard]] std::size_t operator()(int i) const noexcept { return i; }
};
}

namespace cpp {
std::vector<int> filter_uniq(std::span<const int> in) {
    const auto filtered = std::unordered_set<int, IdentityHash>{in.begin(), in.end()};
    return {filtered.begin(), filtered.end()};
}


std::vector<int> filter_uniq_sort(std::span<const int> in) {
    auto out = std::vector<int>{in.begin(), in.end()};

    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());

    return out;
}

constexpr auto HASH_TABLE_SIZE = size_t{32769};
constexpr auto MOD_TABLE_SIZE = size_t{65536};

[[nodiscard]] std::vector<int> filter_uniq_ht(std::span<const int> in)
{
    if (in.empty()) {
        return {};
    }

    std::vector<bool> hash_table_base_p[HASH_TABLE_SIZE];
    std::vector<bool> hash_table_base_n[HASH_TABLE_SIZE];

    auto out = std::vector<int>{};
    out.reserve(in.size());

    for(auto value : in) {
        const auto quotient = (value & 0x7fff'ffff) >> 16;
        const auto modulus = value & 0x0000'ffff;

        auto& lookup = value > 0 ? hash_table_base_p : hash_table_base_n;
        if(lookup[quotient].empty()) {
            lookup[quotient].resize(MOD_TABLE_SIZE, false);
        }
        if(lookup[quotient][modulus] != 0) {
            continue;
        }

        out.push_back(value);

        lookup[quotient][modulus] = true;
    }

    out.shrink_to_fit();

    return out;
}
}