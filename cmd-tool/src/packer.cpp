#include "packer.hpp"
#include "log.hpp"

#include <filesystem>
#include <fstream>

namespace sga {

bool Packer::Pack(const std::string &input_folder,
                  const std::string &output_file,
                  const std::string &encryption_key) {

  std::string stripped_path = input_folder;
  if (stripped_path.back() == '\\' || stripped_path.back() == '/') {
    stripped_path.pop_back();
  }

  std::filesystem::path input_path(stripped_path);
  std::filesystem::path base = input_path.parent_path();

  log::LogInfo("Packaging resources in: " + base.string());

  std::vector<HeaderEntry> entries;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(input_path)) {

    if (!entry.is_regular_file()) {
      continue;
    }

    auto path = entry.path().lexically_relative(base);

    log::LogInfo(path.string());

    HeaderEntry header_entry;
    header_entry.path = entry.path().string();
    header_entry.packaged_path = path.string();

    entries.push_back(std::move(header_entry));
  }

  std::ofstream out_stream(output_file, std::ios::binary | std::ios::out);

  auto entry_count = entries.size();
  out_stream << entry_count;
  out_stream.write(reinterpret_cast<const char *>(&entry_count),
                   sizeof(entry_count));

    size_t offset = 0;
  for (auto& entry : entries) {
    std::ifstream in_file(entry.path, std::ios::in | std::ios::binary);

    if (!in_file.is_open()) {
        log::LogError("Failed to read file " + entry.path);
        return false;
    }

    in_file.seekg(in_file.end);
    entry.file_size = in_file.tellg();
    entry.offset = offset;
    offset += entry.file_size;
  }

  for (const auto &entry : entries) {
    out_stream.write(entry.packaged_path.c_str(), entry.packaged_path.size());
    out_stream.write("\0", 1);
    out_stream.write(reinterpret_cast<const char *>(&entry.file_size), sizeof(entry.file_size));
    out_stream.write(reinterpret_cast<const char *>(&entry.offset), sizeof(entry.offset));
  }

  for (const auto &entry : entries) {
    std::ifstream in_file(entry.path, std::ios::in | std::ios::binary);

    if (!in_file.is_open()) {
        log::LogError("Failed to read file " + entry.path);
        return false;
    }

    std::copy(std::istreambuf_iterator<char>(in_file),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(out_stream));
  }

  return true;
}

} // namespace sga