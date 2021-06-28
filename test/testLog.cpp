#include "gtest/gtest.h"
#include "log/Log.hpp"
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
    ASSERT_EQ(0, summary[0].id());
    ASSERT_EQ(key4, summary[0].consume_key());
    ASSERT_TRUE(summary[1].is_removed());
    ASSERT_TRUE(summary[2].is_removed());
    ASSERT_EQ(1, summary[3].id());
    ASSERT_EQ(key1, summary[3].consume_key());
}

TEST(Log, AddRemove) {
    Log<10, 4, 32> log{};
    Key<4> key = createKey<4>("abcd");
    log.add(key, 1);
    log.remove(key);

    auto summary1 = log.summarize();
    ASSERT_EQ(1, summary1.size());
    EXPECT_EQ(true, summary1[0].is_removed());

    log.add(key, 2);
    auto summary2 = log.summarize();
    ASSERT_EQ(1, summary2.size());
    EXPECT_EQ(key, summary2[0].consume_key());
}