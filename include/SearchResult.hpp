#pragma once

#include <variant>

template<typename T>
class SearchResult {
private:
    static constexpr bool removed_v = true;
    static constexpr bool notFound_v = true;
public:
    static SearchResult found(T val) {
        return SearchResult{std::move(val)};
    }

    static SearchResult notFound() {
        return SearchResult{notFound_v};
    }

    static SearchResult removed() {
        return SearchResult{removed_v};
    }

    bool is_found() {
        return result.index() == 0;
    }

    bool is_removed() {
        return result.index() == 1 && (std::get<bool>(result) == removed_v);
    }

    bool is_not_found() {
        return result.index() == 1 && (std::get<bool>(result) == notFound_v);
    }

    T get() {
        return std::get<T>(result);
    }

private:
    explicit SearchResult(T val) : result{val} {}

    explicit SearchResult(bool val) : result{val} {}

private:
    std::variant<T, bool> result;
};
