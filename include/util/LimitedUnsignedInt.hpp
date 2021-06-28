#pragma once

#include <cstddef>

template<size_t bits>
class LimitedUnsignedInt {
public:
    using PlainType = unsigned int;
    static constexpr unsigned int max_value = (1ull << bits) - 1;
private:
    static_assert(bits <= sizeof(PlainType) * 8);

    LimitedUnsignedInt(PlainType) {
        throw std::exception();
    }

    operator PlainType() const {
        throw std::exception();
    }
};
