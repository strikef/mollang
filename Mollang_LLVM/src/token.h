#ifndef MOLLANG_TOKEN_H
#define MOLLANG_TOKEN_H

#include "portable_string.h"
#include "result.h"
#include "sealed_class.h"
#include "static_error.h"

#include <string>
#include <string_view>
#include <vector>

namespace mollang {
namespace token {
template <typename Self> class Base {
private:
  Base() noexcept {};
  friend Self;

public:
  Base(Base &&other) noexcept = default;
  Base &operator=(Base &&other) noexcept = default;

  StringView as_str() const noexcept {
    return static_cast<Self *>(this)->as_str();
  }
};

class StandardOutput final : public Base<StandardOutput> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class StandardOutputMode final : public Base<StandardOutputMode> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class StandardOutputPrec final : public Base<StandardOutputPrec> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class StandardInput final : public Base<StandardInput> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class BlockStart final : public Base<BlockStart> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Then final : public Base<Then> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Repeat final : public Base<Repeat> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Go final : public Base<Go> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class To final : public Base<To> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Decl final : public Base<Decl> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Call final : public Base<Call> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Separator final : public Base<Separator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class MulOperator final : public Base<MulOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class DivOperator final : public Base<DivOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class IntDivOperator final : public Base<IntDivOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class ModOperator final : public Base<ModOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class IncOperator final : public Base<IncOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class DecOperator final : public Base<DecOperator> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

class Return final : public Base<Return> {
private:
  const static String str;

public:
  StringView as_str() const noexcept { return StringView(str); };
};

using VarIDTy = size_t;
class VarName final : public Base<VarName> {
private:
  VarIDTy id;
  String name;

public:
  VarName(const StringView name) noexcept;
  StringView as_str() const noexcept { return StringView(name); }
};

class FuncName final : public Base<FuncName> {
private:
  String name;

public:
  FuncName(const StringView name) noexcept;
  StringView as_str() const noexcept { return StringView(name); }
};

enum class Kind {
  STDOUT,      // 루
  STDOUT_MODE, // 아
  STDOUT_PREC, // 루!
  STDIN,       // 루?
  BLOCK_START, // 은?행
  THEN,        // 털!자
  REPEAT,      // 돌!자
  GO,          // 가
  TO,          // 자!
  DECL,        // 짓!자
  CALL,        // 가!자
  SEPARATOR,   // ,
  MUL_OP,      // .
  DIV_OP,      // ..
  IDIV_OP,     // ...
  MOD_OP,      // ....
  INC_OP,      // ?
  DEC_OP,      // !
  RETURN,      // 0ㅅ0
  VAR_NAME,
  FUNC_NAME
};

class Location {
private:
  String loc_str;

public:
  Location(const size_t row, const size_t column_begin,
           const size_t column_end) noexcept;
  Location(Location &&other) = default;
  Location &operator=(Location &&other) = default;

  StringView as_str() const noexcept { return StringView(loc_str); }
};

class Token
    : public SealedClass<Token, Kind, StandardOutput, StandardOutputMode,
                         StandardOutputPrec, StandardInput, BlockStart, Then,
                         Repeat, Go, To, Decl, Call, Separator, MulOperator,
                         DivOperator, IntDivOperator, ModOperator, IncOperator,
                         DecOperator, Return, VarName, FuncName> {
private:
  using BaseSC =
      SealedClass<Token, Kind, StandardOutput, StandardOutputMode,
                  StandardOutputPrec, StandardInput, BlockStart, Then, Repeat,
                  Go, To, Decl, Call, Separator, MulOperator, DivOperator,
                  IntDivOperator, ModOperator, IncOperator, DecOperator, Return,
                  VarName, FuncName>;
  Location loc;

  Token(const Kind kind, decltype(BaseSC::data) &&data,
        Location &&loc) noexcept;

public:
  const Location *get_location() const noexcept { return &loc; }
  StringView as_str() const noexcept {
    return std::visit([](const auto &__data) { return __data.as_str(); }, data);
  }

  static Token StandardOutput(Location &&loc) noexcept;
  static Token StandardOutputMode(Location &&loc) noexcept;
  static Token StandardOutputPrec(Location &&loc) noexcept;
  static Token StandardInput(Location &&loc) noexcept;
  static Token BlockStart(Location &&loc) noexcept;
  static Token Then(Location &&loc) noexcept;
  static Token Repeat(Location &&loc) noexcept;
  static Token Go(Location &&loc) noexcept;
  static Token To(Location &&loc) noexcept;
  static Token Decl(Location &&loc) noexcept;
  static Token Call(Location &&loc) noexcept;
  static Token Separator(Location &&loc) noexcept;
  static Token MulOperator(Location &&loc) noexcept;
  static Token DivOperator(Location &&loc) noexcept;
  static Token IDivOperator(Location &&loc) noexcept;
  static Token ModOperator(Location &&loc) noexcept;
  static Token IncOperator(Location &&loc) noexcept;
  static Token DecOperator(Location &&loc) noexcept;
  static Token Return(Location &&loc) noexcept;
  static Token VarName(Location &&loc, const StringView name) noexcept;
  static Token FuncName(Location &&loc, const StringView name) noexcept;
};

class InvalidSyntaxError : public Error<InvalidSyntaxError> {
private:
  String message;

public:
  InvalidSyntaxError(Location &&loc, const StringView tok) noexcept;
  const char *what() const noexcept { return message.c_str(); }
};

enum class ParserErrorKind { INVALID_SYNTAX };

class ParserError
    : public SealedClass<ParserError, ParserErrorKind, InvalidSyntaxError> {
private:
  using BaseSC = SealedClass<ParserError, ParserErrorKind, InvalidSyntaxError>;
  ParserError(const ParserErrorKind kind, decltype(BaseSC::data) &&err) noexcept;

public:
  const char *what() const noexcept {
    return std::visit([](const auto &__data) { return __data.what(); }, data);
  }

  static ParserError from(InvalidSyntaxError &&err) noexcept;
};

using List = std::vector<Token>;
Result<List, ParserError> tokenize(const StringView code) noexcept;
} // namespace token

using Token = token::Token;
using TokenList = token::List;
} // namespace mollang

#endif // MOLLANG_TOKEN_H
