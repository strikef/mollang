#include "fs.h"
#include "portable_string.h"
#include "result.h"
#include "token.h"

#include <iostream>

int main(int argc, char *argv[]) {
  auto read_result = fs::readFile(argv[1]);
  const auto code = decltype(read_result)::unwrap(std::move(read_result));

  auto parse_result = mollang::token::tokenize(code);
  if (parse_result.is_err()) {
    auto err = decltype(parse_result)::inspect(std::move(parse_result));
    std::cout << err.what() << "\n";
    return 1;
  } else {
    auto token_list = decltype(parse_result)::unwrap(std::move(parse_result));
    for (const auto &token : token_list) {
      std::cout << token.as_str() << " ";
    }
    std::cout << "\n";
    return 0;
  }
}
