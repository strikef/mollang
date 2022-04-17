#ifndef SEALED_CLASS_H
#define SEALED_CLASS_H

#include <functional>
#include <variant>

namespace sealed_class {
template <typename Self, typename K, typename T, typename... Ts>
class SealedClass {
private:
  using DataTy = std::variant<T, Ts...>;
  K kind;
  DataTy data;

  constexpr SealedClass(const K __kind, DataTy&& __data) noexcept
      : kind(__kind), data(std::move(__data)) {}

  constexpr bool is_of_kind(const K __kind) const noexcept {
    return (__kind == kind);
  }

  friend Self;

public:
  constexpr SealedClass(SealedClass &&other) noexcept = default;
  constexpr SealedClass &operator=(SealedClass &&other) noexcept = default;
};
} // namespace sealed_class

template <typename Self, typename K, typename T, typename... Ts>
using SealedClass = sealed_class::SealedClass<Self, K, T, Ts...>;
#endif // SEALED_CLASS_H
