#include "packer.hpp"
#include "header.hpp"
#include "log.hpp"
#include "writer.hpp"

#include <filesystem>
#include <vector>

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

    std::vector<Header::Entry> entries;

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(input_path)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        auto path = entry.path().lexically_relative(base);

        log::LogInfo(path.string());

        Header::Entry header_entry;
        header_entry.path = entry.path().string();
        header_entry.packaged_path = path.string();

        entries.push_back(std::move(header_entry));
    }

    sga::Writer writer(output_file);

    writer.WriteString(Header::FORMAT_SIGNATURE);

    auto entry_count = entries.size();

    writer.Write<uint64_t>(entry_count);

    size_t offset = 0;
    for (auto &entry : entries) {
        std::ifstream in_file(entry.path, std::ios::ate | std::ios::binary);

        if (!in_file.is_open()) {
            log::LogError("Failed to read file " + entry.path);
            return false;
        }

        entry.file_size = in_file.tellg();
        entry.offset = offset;
        offset += entry.file_size;
    }

    for (const auto &entry : entries) {
        size_t path_size = entry.packaged_path.size();
        // out_stream << path_size;
        writer.Write<uint64_t>(path_size);
        writer.WriteString(entry.packaged_path);
        writer.Write<uint64_t>(entry.file_size);
        writer.Write<uint64_t>(entry.offset);
    }

    for (const auto &entry : entries) {
        if (!writer.WriteFile(entry.path)) {
            sga::log::LogError("Failed to read file: " + entry.path);
        }
    }

    return true;
}

} // namespace sga