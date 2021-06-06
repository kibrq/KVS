#include <filesystem>

#include "Repository.hpp"

Repository::Repository(std::filesystem::path path)
        : file{path.c_str(), std::ios::trunc | std::ios::binary | std::ios::in | std::ios::out},
          path{std::move(path)} {}

void Repository::read(std::streamoff offset, char *data, std::streamsize length) {
    file.seekg(offset);
    if (file.bad()) {
        throw std::exception{};
    }
    file.read(data, length);
}

void Repository::write(std::streamoff offset, const char *data, std::streamsize length) {
    file.seekg(offset);
    if (file.bad()) {
        throw std::exception{};
    }
    file.write(data, length);
}

void Repository::close() {
    file.close();
    std::filesystem::remove(path.c_str());
}
