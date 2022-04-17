#include "portable_string.h"

#include <bitset>

namespace portable_string {
StringIter::StringIter(const StringView __str) noexcept : view(__str), pos(0) {}

size_t StringIter::count_letter_bytes() const noexcept {
#ifdef USE_UTF8
  const auto first_byte = static_cast<unsigned char>(view[pos]);
  // ignore ill-formed utf-8 literals...
  if (first_byte >= 0b11110000) {
    return 4;
  } else if (first_byte >= 0b11100000) {
    return 3;
  } else if (first_byte >= 0b11000000) {
    return 2;
  } else {
    return 1;
  }
#else
  return 1;
#endif
}

void StringIter::advance_pos() noexcept {
  if (has_char()) {
    pos += count_letter_bytes();
  }
}

bool StringIter::has_char() const noexcept {
  return pos < view.size();
}

std::optional<StringView> StringIter::operator*() const noexcept {
  if (has_char())
    return view.substr(pos, count_letter_bytes());
  else
    return std::nullopt;
}

std::optional<StringView> StringIter::peek() const noexcept {
  auto itr = *this;
  return *++itr;
}

bool StringIter::match_current(const StringView c) const noexcept {
  return has_char() && **this == c;
}

bool StringIter::match_next(const StringView c) const noexcept {
  return peek() && *peek() == c;
}

StringIter StringIter::operator++(int) noexcept {
  auto prev = *this;
  advance_pos();
  return prev;
}

StringIter &StringIter::operator++() noexcept {
  advance_pos();
  return *this;
}

bool StringIter::operator==(const StringIter &other) const noexcept {
  return (view == other.view && pos == other.pos);
}

bool StringIter::operator!=(const StringIter &other) const noexcept {
  return !(*this == other);
}


} // namespace portable_string
