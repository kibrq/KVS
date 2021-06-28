#include <vector>

#include <KeyValue.hpp>
#include <repositories/RepositoryFactory.hpp>
#include "serialization/ValueSerializer.hpp"
#include "repositories/TypedRepository.hpp"
#include "unalignedArray/UnalignedKeyArray.hpp"
#include "unalignedArray/UnalignedIntArray.hpp"
#include "index/TableBlock.hpp"
#include "serialization/TableBlockSerializer.hpp"

int main() {
    RepositoryFactory factory{"dev"};
    TypedRepository<Value<80>> s{factory.get()};
    auto tmp = std::make_unique<char[]>(80);
    Value<80> val{std::move(tmp)};
    tmp = std::make_unique<char[]>(80);
    s.write(0, val);
    s.read(0);
    s.close();
    UnalignedArray<Key<10>>::get(tmp.get(), 3);
    UnalignedArray<LimitedUnsignedInt<8u>>::get(tmp.get(), 4);
    TableBlock<800, 3, 3> block{std::vector<TableBlock<800, 3, 3>::Entry>{}};
    Serializer<TableBlock<800, 3, 3>>::consume(std::move(tmp));
}
