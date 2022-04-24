#ifndef PORTABLE_STRING_H
#define PORTABLE_STRING_H

#include <optional>
#include <string>

namespace portable_string {
using String = std::string;
using StringView = std::string_view;

#if !defined(FORCE_UTF8) && (defined(_WIN32) || defined(_WIN64))
#define USE_CP949
#else // defined(_WIN32) || defined(_WIN64)
#define USE_UTF8
#endif // defined(_WIN32) || defined(_WIN64)

class StringIter {
private:
  StringView view;
  size_t pos;

  StringIter(const StringIter &other) = default;
  StringIter &operator=(const StringIter &other) = default;
  size_t count_letter_bytes() const noexcept;
  void advance_pos() noexcept;

public:
  StringIter(const StringView str) noexcept;
  bool has_char() const noexcept;
  std::optional<StringView> operator*() const noexcept;
  std::optional<StringView> peek() const noexcept;
  bool match_current(const StringView c) const noexcept;
  bool match_next(const StringView c) const noexcept;
  StringIter operator++(int) noexcept;
  StringIter &operator++() noexcept;
  bool operator==(const StringIter &other) const noexcept;
  bool operator!=(const StringIter &other) const noexcept;
};

template <typename T> String to_string(const T &item) {
  return std::to_string(item);
}

bool lexi_lt(const StringView c, const char *str) noexcept;
bool lexi_le(const StringView c, const char *str) noexcept;
bool lexi_gt(const StringView c, const char *str) noexcept;
bool lexi_ge(const StringView c, const char *str) noexcept;
} // namespace portable_string

using String = portable_string::String;
using StringView = portable_string::StringView;
using StringIter = portable_string::StringIter;
#endif // PORTABLE_STRING_H
