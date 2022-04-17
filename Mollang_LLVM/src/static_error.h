#ifndef STATIC_ERROR_H
#define STATIC_ERROR_H

#include <exception>
#include <memory>
#include <string>

namespace static_error {
template <typename E> class Error : public std::exception {
private:
  Error() noexcept {}
  friend E;

public:
  const char *what() const noexcept override {
    return static_cast<const E *>(this)->what();
  }

  Error(Error &&other) noexcept = default;
  Error &operator=(Error &&other) noexcept = default;
};
} // namespace static_error

template <typename E> using Error = static_error::Error<E>;
#endif // STATIC_ERROR_H
