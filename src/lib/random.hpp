#pragma once

#include <limits>
#include <random>
#include <type_traits>

namespace Random {
inline std::random_device rd;
inline std::seed_seq seed{rd()};
inline std::mt19937 mt{seed};

template <typename T> inline T get() {
  T min = std::numeric_limits<T>::min();
  T max = std::numeric_limits<T>::max();
  return std::uniform_int_distribution<T>{min, max}(mt);
}

template <typename T> inline T get(T min, T max) {
  return std::uniform_int_distribution<T>{min, max}(mt);
}

template <typename U, typename V>
inline std::common_type<U, V> get(U min, V max) {
  using Common = std::common_type<U, V>;
  return std::uniform_int_distribution<Common>{static_cast<Common>(min),
                                               static_cast<Common>(max)}(mt);
}

template <> inline double get(double min, double max) {
  return std::uniform_real_distribution<double>{min, max}(mt);
}
} // namespace Random
