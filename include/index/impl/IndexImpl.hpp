
#include <algorithm>

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
Index<key_size, hash_size, block_size, id_bits>::Index(std::unique_ptr<Filter<key_size>> &&filter,
                                                       std::unique_ptr<TypedRepositoryFactory<TBlock>> &&factory)
        : filter_m{std::move(filter)}, factory_m{std::move(factory)}, repository_m{factory_m->get()} {
    sparse_table_m.emplace_back();
}


// Пока просто обрезаем ключ до нужного числа байт
template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::calculate_hash(const Key<key_size> &key) -> Hash {
    return std::string(key.getKey(), hash_size);
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
auto Index<key_size, hash_size, block_size, id_bits>::get(const Key<key_size> &key) -> std::optional<Id> {

    if (filter_m->is_absent(key)) {
        return std::nullopt;
    }

    Hash key_hash = calculate_hash(key);

    auto it = std::lower_bound(sparse_table_m.begin(), sparse_table_m.end(), key_hash);
    if (it == sparse_table_m.end())
        return std::nullopt;

    //Инвариант: всегда вначале ветора лежит пустая строка.
    assert(it != sparse_table_m.begin());

    long index = it - sparse_table_m.begin();
    std::optional<Id> left = std::nullopt, right = std::nullopt;

    auto read_left = [&index, &left, &key, this]() {
        assert(index - 1 >= 0);
        left = repository_m.read(static_cast<std::size_t>(index - 1)).get(key);
    };

    auto read_right = [&index, &right, &key, this]() {
        assert(index >= 0);
        right = repository_m.read(static_cast<std::size_t>(index)).get(key);
    };

    // Если хэш ключа совпал с хэшом ключа, который в спарстэйбл, то ключ может быть как слева, так и справа
    if (key_hash == *it && it != sparse_table_m.end() - 1) {
        read_left(), read_right();
    } else {
        read_left();
    }

    return left.has_value() || right.has_value() ? (left.has_value() ? left : right) : std::nullopt;
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
bool Index<key_size, hash_size, block_size, id_bits>::compare_keys_by_hash(const Key<key_size> &key1,
                                                                           const Key<key_size> &key2) {
    return calculate_hash(key1) < calculate_hash(key2);
}

template<std::size_t key_size, std::size_t hash_size, std::size_t block_size, std::size_t id_bits>
void Index<key_size, hash_size, block_size, id_bits>::apply(std::vector<KeyAction<key_size>> &&keyActions) {

    std::sort(keyActions.begin(), keyActions.end(), [this](const KeyAction<key_size> &key1,
                                                           const KeyAction<key_size> &key2) {
        return compare_keys_by_hash(key1.view_key(), key2.view_key());
    });

    filter_m->clear();

    TypedRepository<TBlock> new_repository = factory_m->get();

    std::vector<typename TBlock::Entry> current_block;
    current_block.reserve(TBlock::max_size);

    std::vector<Hash> new_sparse;
    new_sparse.reserve(sparse_table_m.size() + keyActions.size());
    new_sparse.emplace_back();

    std::size_t current_block_index = 0;

    auto index = begin(), index_end = end();
    auto log = keyActions.begin(), log_end = keyActions.end();

    auto take = [&current_block, this](typename TBlock::Entry &&e) {
        current_block.push_back(std::move(e)), filter_m->add(current_block.back().key);
    };

    auto take_from_index = [take, &index]() {
        take(*index), ++index;
    };

    auto take_from_log = [take, &log]() {
        take({log->consume_key(), static_cast<unsigned int>(log->id())}), ++log;
    };

    auto flush_current_block = [this, &current_block, &current_block_index, &new_repository, &new_sparse]() {
        new_repository.write(current_block_index++, TBlock(current_block));
        new_sparse.push_back(calculate_hash(current_block.back().key));
        current_block.clear();
    };

    auto check_current_block = [&current_block, flush_current_block]() {
        if (current_block.size() >= TBlock::max_size - 1) {
            flush_current_block();
        }
    };

    while (index != index_end && log != log_end) {

        // Если ключи совпали, то делаем выбор в пользу log
        if (index->key == log->view_key()) {
            if (!log->is_removed()) {
                take_from_log(), ++index;
            } else {
                ++log, ++index;
            }
            continue;
        }

        if (compare_keys_by_hash(index->key, log->view_key())) {
            take_from_index();
        } else if (!log->is_removed()) {
            take_from_log();
        } else {
            ++log;
        }

        check_current_block();
    }

    while (index != index_end) {
        take_from_index();
        check_current_block();
    }

    while (log != log_end) {
        if (log->is_removed()) {
            ++log;
            continue;
        }
        take_from_log();
        check_current_block();
    }

    if (!current_block.empty())
        flush_current_block();

    sparse_table_m = std::move(new_sparse);

    repository_m.close();
    repository_m = std::move(new_repository);
}
