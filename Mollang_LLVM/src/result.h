#ifndef RESULT_H
#define RESULT_H

#include "sealed_class.h"
#include "static_error.h"

#include <functional>

namespace result {
class BadUnwrapError : public Error<BadUnwrapError> {
public:
  const char *what() const noexcept {
    return "Result error: tried to unwrap Err";
  }
};

class BadInspectError : public Error<BadInspectError> {
public:
  const char *what() const noexcept {
    return "Result error: tried to inspect Ok";
  }
};

enum class Kind { OK, ERR };

template <typename T, typename E>
class Result : public SealedClass<Result<T, E>, Kind, T, E> {
private:
  using BaseSC = SealedClass<Result<T, E>, Kind, T, E>;

  constexpr Result(const Kind __kind, decltype(BaseSC::data) &&__data) noexcept
      : BaseSC(__kind, std::move(__data)) {}

public:
  constexpr bool is_ok() const noexcept {
    return BaseSC::is_of_kind(Kind::OK);
  }
  constexpr bool is_err() const noexcept {
    return BaseSC::is_of_kind(Kind::ERR);
  }

  constexpr static Result Ok(T &&__data) noexcept {
    return Result(Kind::OK, std::move(__data));
  }

  constexpr static Result Err(E &&__data) noexcept {
    return Result(Kind::ERR, std::move(__data));
  }

  template <typename U>
  constexpr static Result<U, E> map(Result &&src,
                                    std::function<U(T &&)> &&fn) noexcept {
    if (src.is_ok()) {
      auto data = std::get<T>(std::move(src.data));
      return Result<U, E>::Ok(fn(std::move(data)));
    } else {
      auto data = std::get<E>(std::move(src.data));
      return Result<U, E>::Err(std::move(data));
    }
  }

  template <typename F>
  constexpr static Result<T, F> map_err(Result &&src,
                                        std::function<F(E &&)> &&fn) noexcept {
    if (src.is_err()) {
      auto data = std::get<E>(std::move(src.data));
      return Result<T, F>::Err(fn(std::move(data)));
    } else {
      auto data = std::get<T>(std::move(src.data));
      return Result<T, F>::Ok(std::move(data));
    }
  }

  template <typename U>
  constexpr static Result<U, E>
  and_then(Result &&src, std::function<Result<U, E>(T &&)> &&fn) noexcept {
    if (src.is_ok()) {
      auto data = std::get<T>(std::move(src.data));
      return fn(std::move(data));
    } else {
      auto data = std::get<E>(std::move(src.data));
      return Result<U, E>::Err(std::move(data));
    }
  }

  template <typename F>
  constexpr static Result<T, F>
  or_else(Result &&src, std::function<Result<T, F>(E &&)> &&fn) noexcept {
    if (src.is_err()) {
      auto data = std::get<E>(std::move(src.data));
      return fn(std::move(data));
    } else {
      auto data = std::get<T>(std::move(src.data));
      return Result<T, F>::Ok(std::move(data));
    }
  }

  template <typename U>
  constexpr static U map_or_else(Result &&src, std::function<U(E &&)> &&fb,
                                 std::function<U(T &&)> &&fn) noexcept {
    if (src.is_ok()) {
      auto data = std::get<T>(std::move(src.data));
      return fn(std::move(data));
    } else {
      auto data = std::get<E>(std::move(src.data));
      return fb(std::move(data));
    }
  }

  constexpr static T unwrap(Result &&src) {
    if (src.is_err()) {
      throw BadUnwrapError();
    }
    return std::get<T>(std::move(src.data));
  }

  constexpr static E inspect(Result &&src) {
    if (src.is_ok()) {
      throw BadInspectError();
    }
    return std::get<E>(std::move(src.data));
  }

  constexpr static T unwrap_or_else(Result &&src,
                                    std::function<T(E &&)> &&fn) noexcept {
    if (src.is_ok()) {
      return std::get<T>(std::move(src.data));
    } else {
      auto data = std::get<E>(std::move(src.data));
      return fn(std::move(data));
    }
  }
};
} // namespace result

template <typename T, typename E> using Result = result::Result<T, E>;
#endif // RESULT_H
