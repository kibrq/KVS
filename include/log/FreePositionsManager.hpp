#pragma once

#include <bitset>

template<size_t size>
class FreePositionsManager {
public:
    FreePositionsManager() {
        set.set();
    }

    size_t get() {
        last = set._Find_next(last);
        if (last == size) {
            last = set._Find_first();
            assert(last != size);
        }
        set.reset(last);
        return last;
    }

    void free(size_t index) {
        assert(!set[index]);
        set.set(index);
    }

private:
    size_t last = static_cast<size_t>(-1);
    std::bitset<size> set{};
};
