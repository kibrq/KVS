#pragma once

#include <cstddef>
#include <fstream>
#include <filesystem>

class Repository {
public:
    explicit Repository(std::filesystem::path path);

    void read(std::streamoff offset, char *data, std::streamsize length);

    void write(std::streamoff offset, const char *data, std::streamsize length);

    void close();

private:
    std::fstream file;
    std::filesystem::path path;
};
