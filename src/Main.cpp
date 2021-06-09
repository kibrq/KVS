#include <vector>

#include <KeyValue.hpp>
#include <RepositoryFactory.hpp>
#include "ValueSerializer.hpp"
#include "TypedRepository.hpp"
#include "LazyUnalignedKeyArray.hpp"
#include "LazyUnalignedIntArray.hpp"
#include "TableBlock.hpp"
#include "TableBlockSerializer.hpp"

int main() {
    RepositoryFactory factory{"dev"};
    TypedRepository<Value<80>> s{factory.get()};
    auto tmp = std::make_unique<char[]>(80);
    Value<80> val{std::move(tmp)};
    tmp = std::make_unique<char[]>(80);
    s.write(0, val);
    s.read(0);
    s.close();
    LazyUnalignedArray<Key<10>>::get(tmp.get(), 3);
    LazyUnalignedArray<LimitedUnsignedInt<8u>>::get(tmp.get(), 4);
    TableBlock<800, 3, 3> block{std::vector<TableBlock<800, 3, 3>::Entry>{}};
    Serializer<TableBlock<800, 3, 3>>::consume(std::move(tmp));
}
