#ifndef FILTER_UNIQ_INTS_CPP_HPP
#define FILTER_UNIQ_INTS_CPP_HPP

#include <vector>
#include <span>
#include <concepts>
#include <cstdint>
#include <limits>
#include <ranges>

namespace cpp {

[[nodiscard]] std::vector<int> filter_uniq(std::span<const int> in);
[[nodiscard]] std::vector<int> filter_uniq_sort(std::span<const int> in);


constexpr auto HASH_TABLE_SIZE = size_t{32769};
constexpr auto MOD_TABLE_SIZE = size_t{65536};

namespace detail {

template<std::integral Value_T>
requires (sizeof(Value_T) <= 4)
struct as_unsigned {
  using type = Value_T;
};

template<>
struct as_unsigned<std::int8_t> {
  using type = std::uint8_t;
};

template<>
struct as_unsigned<std::int16_t> {
  using type = std::uint16_t;
};

template<>
struct as_unsigned<std::int32_t> {
  using type = std::uint32_t;
};

template<std::integral Value_T>
struct LookupTableProperties {
  using unsigend_t = typename as_unsigned<Value_T>::type;
  static constexpr auto quotient_lookup_size = size_t{1 << std::min<size_t>(16, 8 * std::max<int>(0, sizeof(Value_T) - 2))};
  static constexpr auto modulus_lookup_size = size_t{1 << std::min<size_t>(16, (8 * sizeof(Value_T)))};
  static constexpr auto shift = 4 * sizeof(Value_T);
  static constexpr auto offset = (std::is_signed_v<Value_T> ? std::numeric_limits<Value_T>::max() : 0);
  static constexpr auto modulus_mask = (1 << 4 * sizeof(Value_T)) - 1;
};

}

template<std::ranges::sized_range Range_T>
requires(std::is_integral_v<typename Range_T::value_type>)
[[nodiscard]] std::vector<typename Range_T::value_type> filter_uniq_ht(Range_T&& in)
{
  if (in.empty()) {
    return {};
  }

  using Value_T = typename Range_T::value_type;
  using Properties = detail::LookupTableProperties<Value_T>;

  std::vector<bool> quotient_lookup_p[Properties::quotient_lookup_size / 2];
  std::vector<bool> quotient_lookup_n[Properties::quotient_lookup_size / 2];

  auto out = std::vector<Value_T>{};
  out.reserve(in.size());

  for(auto value : in) {
    const auto quotient = std::abs(value) >> Properties::shift;
    const auto modulus = std::abs(value) & Properties::modulus_mask;

    auto& quotient_lookup = value > 0 ? quotient_lookup_p : quotient_lookup_n;

    if(quotient_lookup[quotient].empty()) {
      quotient_lookup[quotient].resize(MOD_TABLE_SIZE, false);
    }

    if(not quotient_lookup[quotient][modulus]) {
      out.push_back(value);
      quotient_lookup[quotient][modulus] = true;
    }
  }

  out.shrink_to_fit();

  return out;
}

}

#endif //FILTER_UNIQ_INTS_CPP_HPP
