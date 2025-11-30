#pragma once

#include <limits>
#include <random>

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

template <> inline double get(double min, double max) {
  return std::uniform_real_distribution<double>{min, max}(mt);
}
} // namespace Random
