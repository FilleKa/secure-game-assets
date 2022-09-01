#include "packer.hpp"
#include "header.hpp"
#include "log.hpp"
#include "writer.hpp"

#include <algorithm>
#include <filesystem>
#include <vector>

namespace sga {

bool Packer::Pack(const std::string &input_folder,
                  const std::string &output_file,
                  const std::string &encryption_key) {

    Header header;

    sga::Writer writer(output_file, encryption_key);
    header.WriteHeader(header, input_folder, writer);

    std::vector<Header::Entry> sorted_entries;

    const auto &entries = header.GetEntries();

    for (const auto &e : entries) {
        sorted_entries.push_back(e.second);
    }

    std::sort(sorted_entries.begin(), sorted_entries.end(),
              [](const Header::Entry &lhs, const Header::Entry &rhs) {
                  return lhs.offset < rhs.offset;
              });

    for (const auto &e : sorted_entries) {
        writer.WriteFile(e.path, e.file_size);
        writer.FlushEncryped();
    }

    return true;
}

} // namespace sga