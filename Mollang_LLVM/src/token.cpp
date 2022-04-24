#include "token.h"

namespace {
using namespace mollang::token;
Token parse_binary_ops(StringIter &itr, const size_t __row,
                       size_t &__column_cur) noexcept {
  const size_t column_begin = __column_cur;
  if (itr.match_next(".")) {
    itr++;
    __column_cur++;
    if (itr.match_next(".")) {
      itr++;
      __column_cur++;
      if (itr.match_next(".")) {
        itr++;
        __column_cur++;
        return Token::ModOperator(Location(__row, column_begin, __column_cur));
      }
      return Token::IDivOperator(Location(__row, column_begin, __column_cur));
    }
    return Token::DivOperator(Location(__row, column_begin, __column_cur));
  }
  return Token::MulOperator(Location(__row, column_begin, __column_cur));
}

Result<Token, InvalidSyntaxError>
try_parse_return(StringIter &itr, const size_t __row,
                 size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("ㅅ")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("0")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::Return(Location(__row, column_begin, __column_cur)));
    }
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Token parse_stdio(StringIter &itr, const size_t __row,
                  size_t &__column_cur) noexcept {
  const size_t column_begin = __column_cur;
  if (itr.match_next("?")) {
    itr++;
    __column_cur++;
    return Token::StandardInput(Location(__row, column_begin, __column_cur));
  } else if (itr.match_next("!")) {
    itr++;
    __column_cur++;
    return Token::StandardOutputPrec(
        Location(__row, column_begin, __column_cur));
  } else {
    return Token::StandardOutput(Location(__row, column_begin, __column_cur));
  }
}

Result<Token, InvalidSyntaxError>
try_parse_block_start(StringIter &itr, const size_t __row,
                      size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("?")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("행")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::BlockStart(Location(__row, column_begin, __column_cur)));
    }
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Result<Token, InvalidSyntaxError>
try_parse_then(StringIter &itr, const size_t __row,
               size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("!")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("자")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::Then(Location(__row, column_begin, __column_cur)));
    }
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Result<Token, InvalidSyntaxError>
try_parse_repeat(StringIter &itr, const size_t __row,
                 size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("!")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("자")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::Repeat(Location(__row, column_begin, __column_cur)));
    }
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Result<Token, InvalidSyntaxError>
try_parse_go_or_call(StringIter &itr, const size_t __row,
                     size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("!")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("자")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::Call(Location(__row, column_begin, __column_cur)));
    }
    if (itr.peek()) {
      tok.append(*itr.peek());
      __column_cur++;
    }
    return RetType::Err(
        InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
  } else {
    return RetType::Ok(Token::Go(Location(__row, column_begin, __column_cur)));
  }
}

Result<Token, InvalidSyntaxError>
try_parse_decl(StringIter &itr, const size_t __row,
               size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("!")) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_next("자")) {
      itr++;
      __column_cur++;
      return RetType::Ok(
          Token::Decl(Location(__row, column_begin, __column_cur)));
    }
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Result<Token, InvalidSyntaxError> try_parse_to(StringIter &itr,
                                               const size_t __row,
                                               size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  if (itr.match_next("!")) {
    tok.append(*itr++.peek());
    __column_cur++;
    return RetType::Ok(Token::To(Location(__row, column_begin, __column_cur)));
  }
  if (itr.peek()) {
    tok.append(*itr.peek());
    __column_cur++;
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}

Result<Token, InvalidSyntaxError>
try_parse_long_var_name(StringIter &itr, const size_t __row,
                        size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  String tok(**itr);

  const size_t column_begin = __column_cur;
  while (true) {
    tok.append(*itr++.peek());
    __column_cur++;
    if (itr.match_current("올")) {
      return RetType::Ok(
          Token::VarName(Location(__row, column_begin, __column_cur), tok));
    } else if (!itr.match_current("오")) {
      return RetType::Err(
          InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
    }
  }
}

Result<Token, InvalidSyntaxError>
try_parse_func_name(StringIter &itr, const size_t __row,
                    size_t &__column_cur) noexcept {
  using RetType = Result<Token, InvalidSyntaxError>;
  namespace ps = portable_string;
  String tok(**itr);

  const size_t column_begin = __column_cur;

  auto c = **itr;
  // due to irregularity in CP949, number of comparisons should be made
  if ((ps::lexi_ge(c, "마") || ps::lexi_ge(c, "맊")) &&
      (ps::lexi_lt(c, "바") || ps::lexi_lt(c, "밙")) && c != "몰") {
    tok.append(*itr++.peek());
    __column_cur++;
    c = **itr;
    if ((ps::lexi_ge(c, "라") || ps::lexi_ge(c, "띾")) &&
        (ps::lexi_lt(c, "마") || ps::lexi_lt(c, "맊")) && c != "루") {
      return RetType::Ok(
          Token::FuncName(Location(__row, column_begin, __column_cur), tok));
    }
  }
  return RetType::Err(
      InvalidSyntaxError(Location(__row, column_begin, __column_cur), tok));
}
} // namespace

namespace mollang::token {
const String StandardOutput::str = "루";
const String StandardOutputMode::str = "아";
const String StandardOutputPrec::str = "루!";
const String StandardInput::str = "루?";
const String BlockStart::str = "은?행";
const String Then::str = "털!자";
const String Repeat::str = "돌!자";
const String Go::str = "가";
const String To::str = "자!";
const String Decl::str = "짓!자";
const String Call::str = "가!자";
const String Separator::str = ",";
const String MulOperator::str = ".";
const String DivOperator::str = "..";
const String IntDivOperator::str = "...";
const String ModOperator::str = "....";
const String IncOperator::str = "?";
const String DecOperator::str = "!";
const String Return::str = "0ㅅ0";

VarName::VarName(const StringView __name) noexcept : name(__name) { id = 0; }

FuncName::FuncName(const StringView __name) noexcept : name(__name) {}

Location::Location(const size_t __row, const size_t __column_begin,
                   const size_t __column_end) noexcept {
  loc_str = String("[")
                .append(portable_string::to_string(__row))
                .append(":")
                .append(portable_string::to_string(__column_begin))
                .append("..")
                .append(portable_string::to_string(__column_end))
                .append("]");
}

Token::Token(const Kind __kind, decltype(BaseSC::data) &&__data,
             Location &&__loc) noexcept
    : BaseSC(__kind, std::move(__data)), loc(std::move(__loc)) {}

Token Token::StandardOutput(Location &&__loc) noexcept {
  return Token(Kind::STDOUT, token::StandardOutput(), std::move(__loc));
}

Token Token::StandardOutputMode(Location &&__loc) noexcept {
  return Token(Kind::STDOUT_MODE, token::StandardOutputMode(),
               std::move(__loc));
}

Token Token::StandardOutputPrec(Location &&__loc) noexcept {
  return Token(Kind::STDOUT_PREC, token::StandardOutputPrec(),
               std::move(__loc));
}

Token Token::StandardInput(Location &&__loc) noexcept {
  return Token(Kind::STDIN, token::StandardInput(), std::move(__loc));
}

Token Token::BlockStart(Location &&__loc) noexcept {
  return Token(Kind::BLOCK_START, token::BlockStart(), std::move(__loc));
}

Token Token::Then(Location &&__loc) noexcept {
  return Token(Kind::THEN, token::Then(), std::move(__loc));
}

Token Token::Repeat(Location &&__loc) noexcept {
  return Token(Kind::REPEAT, token::Repeat(), std::move(__loc));
}

Token Token::Go(Location &&__loc) noexcept {
  return Token(Kind::GO, token::Go(), std::move(__loc));
}

Token Token::To(Location &&__loc) noexcept {
  return Token(Kind::TO, token::To(), std::move(__loc));
}

Token Token::Decl(Location &&__loc) noexcept {
  return Token(Kind::DECL, token::Decl(), std::move(__loc));
}

Token Token::Call(Location &&__loc) noexcept {
  return Token(Kind::CALL, token::Call(), std::move(__loc));
}

Token Token::Separator(Location &&__loc) noexcept {
  return Token(Kind::SEPARATOR, token::Separator(), std::move(__loc));
}

Token Token::MulOperator(Location &&__loc) noexcept {
  return Token(Kind::MUL_OP, token::MulOperator(), std::move(__loc));
}

Token Token::DivOperator(Location &&__loc) noexcept {
  return Token(Kind::DIV_OP, token::DivOperator(), std::move(__loc));
}

Token Token::IDivOperator(Location &&__loc) noexcept {
  return Token(Kind::IDIV_OP, token::IntDivOperator(), std::move(__loc));
}

Token Token::ModOperator(Location &&__loc) noexcept {
  return Token(Kind::MOD_OP, token::ModOperator(), std::move(__loc));
}

Token Token::IncOperator(Location &&__loc) noexcept {
  return Token(Kind::INC_OP, token::IncOperator(), std::move(__loc));
}

Token Token::DecOperator(Location &&__loc) noexcept {
  return Token(Kind::DEC_OP, token::DecOperator(), std::move(__loc));
}

Token Token::Return(Location &&__loc) noexcept {
  return Token(Kind::RETURN, token::Return(), std::move(__loc));
}

Token Token::VarName(Location &&__loc, const StringView __name) noexcept {
  return Token(Kind::VAR_NAME, token::VarName(__name), std::move(__loc));
}

Token Token::FuncName(Location &&__loc, const StringView __name) noexcept {
  return Token(Kind::FUNC_NAME, token::FuncName(__name), std::move(__loc));
}

InvalidSyntaxError::InvalidSyntaxError(Location &&__loc,
                                       const StringView __tok) noexcept {
  message = String("Invalid syntax \"")
                .append(__tok)
                .append("\"")
                .append(__loc.as_str());
}

ParserError::ParserError(const ParserErrorKind __kind,
                         decltype(BaseSC::data) &&__err) noexcept
    : BaseSC(__kind, std::move(__err)) {}

ParserError ParserError::from(InvalidSyntaxError &&__err) noexcept {
  return ParserError(ParserErrorKind::INVALID_SYNTAX, std::move(__err));
}

#define PUSH_OR_RET_PARSER_ERR(res, tokens)                                    \
  {                                                                            \
    if (res.is_ok()) {                                                         \
      tokens.push_back(decltype(res)::unwrap(std::move(res)));                 \
    } else {                                                                   \
      return RetType::Err(                                                     \
          ParserError::from(decltype(res)::inspect(std::move(res))));          \
    }                                                                          \
  }

Result<List, ParserError> tokenize(const StringView code) noexcept {
  using RetType = Result<List, ParserError>;
  List tokens;
  size_t row = 1;
  size_t column = 1;

  auto itr = StringIter(code);
  while (itr.has_char()) {
    if (itr.match_current("\n")) {
      row += 1;
      column = 1;
    } else if (itr.match_current(".")) {
      tokens.push_back(parse_binary_ops(itr, row, column));
    } else if (itr.match_current(",")) {
      tokens.push_back(Token::Separator(Location(row, column, column)));
    } else if (itr.match_current("?")) {
      tokens.push_back(Token::IncOperator(Location(row, column, column)));
    } else if (itr.match_current("!")) {
      tokens.push_back(Token::DecOperator(Location(row, column, column)));
    } else if (itr.match_current("0")) {
      auto parse_result = try_parse_return(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("루")) {
      tokens.push_back(parse_stdio(itr, row, column));
    } else if (itr.match_current("아")) {
      tokens.push_back(
          Token::StandardOutputMode(Location(row, column, column)));
    } else if (itr.match_current("은")) {
      auto parse_result = try_parse_block_start(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("털")) {
      auto parse_result = try_parse_then(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("돌")) {
      auto parse_result = try_parse_repeat(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("가")) {
      auto parse_result = try_parse_go_or_call(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("자")) {
      auto parse_result = try_parse_to(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("짓")) {
      auto parse_result = try_parse_decl(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else if (itr.match_current("몰")) {
      tokens.push_back(Token::VarName(Location(row, column, column), **itr));
    } else if (itr.match_current("모")) {
      auto parse_result = try_parse_long_var_name(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    } else {
      auto parse_result = try_parse_func_name(itr, row, column);
      PUSH_OR_RET_PARSER_ERR(parse_result, tokens);
    }

    itr++;
    column++;
  }

  return RetType::Ok(std::move(tokens));
}
} // namespace mollang::token
