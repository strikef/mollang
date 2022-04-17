#ifndef PORTABLE_STRING_H
#define PORTABLE_STRING_H

#include <optional>
#include <string>

namespace portable_string {
#if defined(_WIN32) || defined(_WIN64)
using String = std::wstring;
using StringView = std::wstring_view;
#else // defined(_WIN32) || defined(_WIN64)
#define USE_UTF8
using String = std::string;
using StringView = std::string_view;
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
#ifdef USE_UTF8
  return std::to_string(item);
#else  // USE_UTF8
  return std::to_wstring(item);
#endif // USE_UTF8
}
} // namespace portable_string

using String = portable_string::String;
using StringView = portable_string::StringView;
using StringIter = portable_string::StringIter;
#endif // PORTABLE_STRING_H
