#include <unity.h>


#include <database.hpp>


void testInsertSingle() {
    DC::Batch batch;
    batch.add("1234");
    batch.add("2345");
    TEST_ASSERT_EQUAL(2, batch.size());
    TEST_ASSERT_TRUE(batch.contains("1234"));
}


void testInsertRange() {
    DC::Batch batch;
    std::vector<DC::NFCToken> v = {"1234", "2345", "3456"};
    batch.add(v.begin(), v.end());
    TEST_ASSERT_EQUAL(3, batch.size());
    TEST_ASSERT_TRUE(batch.contains("3456"));
    TEST_ASSERT_FALSE(batch.contains("4567"));
}


void testSort() {
    DC::Batch batch;
    batch.add("4567");
    batch.add("1234");
    batch.add("2345");
    batch.sort();
    batch.sort();
    auto it = batch.begin();
    TEST_ASSERT_EQUAL_STRING(it->toHexRepr().c_str(), "1234");
    it++;
    TEST_ASSERT_EQUAL_STRING(it->toHexRepr().c_str(), "2345");
    it++;
    TEST_ASSERT_EQUAL_STRING(it->toHexRepr().c_str(), "4567");
    it++;

    TEST_ASSERT_TRUE(batch.contains("4567"));
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testInsertSingle);
    RUN_TEST(testInsertRange);
    RUN_TEST(testSort);


    UNITY_END();
}


void loop() {


}
