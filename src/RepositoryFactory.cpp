#include "RepositoryFactory.hpp"
#include <string>

RepositoryFactory::RepositoryFactory(std::filesystem::path base_path) : base_path{std::move(base_path)} {}

Repository RepositoryFactory::get() {
    return Repository(std::filesystem::path(base_path).append(std::to_string(id++)));
}
