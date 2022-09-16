#include "sga_container.hpp"
#include "reader.hpp"

#include <fstream>

namespace sga {

SGAContainer::SGAContainer(const std::string& asset_file_path,
                           const std::string& encryption_key,
                           bool keep_in_memory)
    : status_(sga::Status::kSuccess) {

    if (keep_in_memory) {

        std::ifstream input_file_stream(asset_file_path,
                                        std::ios::binary | std::ios::ate);

        if (!input_file_stream.is_open()) {
            status_ = Status::kBadFile;
            return;
        }

        auto file_size = input_file_stream.tellg();
        input_file_stream.seekg(0);

        std::vector<uint8_t> file_contents;
        file_contents.reserve(file_size);
        file_contents.assign(std::istreambuf_iterator<char>(input_file_stream),
                             std::istreambuf_iterator<char>());

        reader_ =
            std::make_unique<Reader>(std::move(file_contents), encryption_key);
    } else {
        reader_ = std::make_unique<Reader>(asset_file_path, encryption_key);
    }

    auto status = header_.ReadHeader(*reader_.get());

    header_.SetAssetFilename(asset_file_path);
    header_.SetEncryptionKey(encryption_key);

    if (status != Status::kSuccess) {
        status_ = status;
        return;
    }
}

Status SGAContainer::GetStatus() const { return status_; }

const Header& SGAContainer::GetHeader() const { return header_; }

std::shared_ptr<SGAFile> SGAContainer::GetFile(const std::string& filepath) {
    Header::Entry entry;
    if (!header_.GetEntry(entry, filepath)) {
        return nullptr;
    }

    auto header_size = header_.GetHeaderSize();
    auto offset = entry.offset;

    reader_->JumpToPosition(header_size + offset);
    reader_->PrepareSize(entry.file_size, entry.index);

    std::unique_ptr<uint8_t[]> data;

    reader_->ReadData(data, entry.file_size);
    return std::make_shared<SGAFile>(std::move(data), entry.file_size);
}

} // namespace sga
