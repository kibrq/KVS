#pragma once

#include <string>
#include <filesystem>
#include "Repository.hpp"

class RepositoryFactory {
public:
    explicit RepositoryFactory(std::filesystem::path base_path);

    Repository get();

private:
    std::filesystem::path base_path;
    int id = 0;
};
