#include "fs.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>

namespace stdfs = std::filesystem;

namespace fs {
FilesystemError::FilesystemError(const StringView __message,
                                 const StringView __path) noexcept {
  message = "filesystem error: ";
  message.reserve(message.size() + __message.size() + __path.size() + 3);
  message.append(__message).append(" [").append(__path).append("]");
}

Result<String, FilesystemError>
readFile(const StringView __filepath) noexcept {
  using RetType = Result<String, FilesystemError>;

  const auto path = stdfs::absolute(stdfs::path(__filepath));
  std::error_code ec;
  const auto status = stdfs::status(path, ec);

  if (!stdfs::exists(status)) {
    return RetType::Err(FilesystemError("file does not exist", path.string()));
  } else if (!stdfs::is_regular_file(status)) {
    return RetType::Err(
        FilesystemError("tried to read nonregular file", path.string()));
  }

  std::ifstream ifstr(path);
  std::stringstream buffer;
  buffer << ifstr.rdbuf();
  return RetType::Ok(buffer.str());
}

Result<size_t, FilesystemError>
writeFile(const StringView __filepath,
          const StringView __content) noexcept {
  using RetType = Result<size_t, FilesystemError>;
  const auto path = stdfs::absolute(stdfs::path(__filepath));
  std::error_code ec, parent_ec;
  const auto status = stdfs::status(path, ec);
  const auto parent_status = stdfs::status(path.parent_path(), parent_ec);

  if (!stdfs::is_directory(parent_status)) {
    return RetType::Err(FilesystemError("parent path is not a directory",
                                        path.parent_path().string()));
  } else if (!stdfs::exists(status)) {
    ; // create new file. no problem!
  } else if (!stdfs::is_regular_file(status)) {
    return RetType::Err(
        FilesystemError("tried to write at nonregular file", path.string()));
  }

  std::ofstream ofstr(path);
  ofstr.write(__content.data(), __content.size());
  return RetType::Ok(__content.size());
}
} // namespace fs
