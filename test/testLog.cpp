#include "gtest/gtest.h"
#include "Log.hpp"
#include "TestUtils.hpp"

TEST(Log, Simple) {
    Log<10, 4, 32> log{};
    Key<4> key1 = createKey<4>("abcd");
    Key<4> key2 = createKey<4>("doge");
    Key<4> key3 = createKey<4>("coin");
    Key<4> key4 = createKey<4>(")!?(");
    log.add(key1, 1);
    ASSERT_TRUE(log.get(key1).is_found());
    ASSERT_EQ(log.get(key1).get(), 1);
    ASSERT_TRUE(log.get(key2).is_not_found());
    log.remove(key2);
    ASSERT_TRUE(log.get(key2).is_removed());
    log.remove(key3);
    log.add(key4, 0);
    auto summary = log.summarize();
    ASSERT_EQ(4, summary.size());
    ASSERT_EQ(1, summary[0].id());
    ASSERT_EQ(key1, summary[0].consume_key());
    ASSERT_TRUE(summary[1].is_removed());
    ASSERT_TRUE(summary[2].is_removed());
    ASSERT_EQ(0, summary[3].id());
    ASSERT_EQ(key4, summary[3].consume_key());
}