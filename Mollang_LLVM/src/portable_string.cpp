#include "portable_string.h"

#include <bitset>

namespace {
unsigned char remove_char_sign(const char __c) noexcept {
  return static_cast<unsigned char>(__c);
}

#ifdef USE_CP949
// are both using EUC-KR or CP949?
bool use_same_encoding(const StringView __op1,
                       const StringView __op2) noexcept {
  const auto op1_first_byte = remove_char_sign(__op1[0]);
  const auto op1_second_byte = remove_char_sign(__op1[1]);
  const auto op2_first_byte = remove_char_sign(__op2[0]);
  const auto op2_second_byte = remove_char_sign(__op2[1]);

  const auto op1_is_EUCKR = op1_first_byte >= 0xB0 && op1_second_byte >= 0xA1;
  const auto op2_is_EUCKR = op2_first_byte >= 0xB0 && op2_second_byte >= 0xA1;
  return op1_is_EUCKR == op2_is_EUCKR;
}
#endif // USE_CP949
}

namespace portable_string {
StringIter::StringIter(const StringView __str) noexcept : view(__str), pos(0) {}

size_t StringIter::count_letter_bytes() const noexcept {
  const auto first_byte = remove_char_sign(view[pos]);

#ifdef USE_UTF8
  if (first_byte >= 0b11110000) {
    return 4;
  } else if (first_byte >= 0b11100000) {
    return 3;
  } else if (first_byte >= 0b11000000) {
    return 2;
  } else {
    return 1;
  }
#endif // USE_UTF8

#ifdef USE_CP949
  if (first_byte >= 0b10000000) {
    return 2;
  } else {
    return 1;
  }
#endif // USE_CP949
}

void StringIter::advance_pos() noexcept {
  if (has_char()) {
    pos += count_letter_bytes();
  }
}

bool StringIter::has_char() const noexcept { return pos < view.size(); }

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

bool lexi_lt(const StringView __c, const char *__str) noexcept {
#ifdef USE_UTF8
  // UTF-8 is well-ordered
  return __c < __str;
#endif // USE_UTF8

#ifdef USE_CP949
  // CP949 is a complete mess
  if (__c.size() == 1) {
    return __c < __str;
  } else {
    if (use_same_encoding(__c, __str)) {
      return __c < __str;
    } else {
      // virtually unable to compare
      return false;
    }
  }
#endif // USE_CP949
}

bool lexi_le(const StringView __c, const char *__str) noexcept {
  return lexi_lt(__c, __str) || __c == __str;
}

bool lexi_gt(const StringView __c, const char *__str) noexcept {
#ifdef USE_UTF8
  return __c > __str;
#endif // USE_UTF8

#ifdef USE_CP949
  if (__c.size() == 1) {
    return __c > __str;
  } else {
    if (use_same_encoding(__c, __str)) {
      return __c > __str;
    } else {
      return false;
    }
  }
#endif // USE_CP949
}

bool lexi_ge(const StringView __c, const char *__str) noexcept {
  return lexi_gt(__c, __str) || __c == __str;
}
} // namespace portable_string
