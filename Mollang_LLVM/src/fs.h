#ifndef FS_H
#define FS_H

#include "portable_string.h"
#include "result.h"
#include "static_error.h"

#include <string>

namespace fs {
class FilesystemError : public Error<FilesystemError> {
private:
  String message;

public:
  FilesystemError(const StringView message,
                  const StringView path) noexcept;
  const char *what() const noexcept { return message.c_str(); }
};

Result<String, FilesystemError>
readFile(const StringView filepath) noexcept;

Result<size_t, FilesystemError> writeFile(const StringView filepath,
                                   const StringView content) noexcept;
} // namespace fs
#endif // FS_H
