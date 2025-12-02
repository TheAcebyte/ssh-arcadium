#pragma once

template <class... T> struct Overload : T... {
  using T::operator()...;
};

template <class... T> Overload(T...) -> Overload<T...>;
