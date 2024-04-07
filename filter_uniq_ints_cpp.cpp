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

    std::vector<int> hash_table_base_p[HASH_TABLE_SIZE];
    std::vector<int> hash_table_base_n[HASH_TABLE_SIZE];

    auto out = std::vector<int>{};
    out.reserve(in.size());

    for(auto value : in) {
        const auto quotient = abs(value / MOD_TABLE_SIZE);
        const auto modulus = abs(value % MOD_TABLE_SIZE);
        if(value > 0) {
            if(hash_table_base_p[quotient].empty()) {
                hash_table_base_p[quotient].resize(MOD_TABLE_SIZE, 0);
            }
            if(hash_table_base_p[quotient][modulus] != 0) {
                continue;
            }
        }
        else {
            if(hash_table_base_n[quotient].empty()) {
                hash_table_base_n[quotient].resize(MOD_TABLE_SIZE, 0);
            }
            if(hash_table_base_n[quotient][modulus] != 0) {
                continue;
            }
        }

        out.push_back(value);
        if(value > 0) {
            hash_table_base_p[quotient][modulus] = 1;
        }
        else {
            hash_table_base_n[quotient][modulus] = 1;
        }
    }

    out.shrink_to_fit();

    return out;
}
}