#include <unity.h>


#include <database.hpp>

DC::SortedDatabase db("/test/tokens");


void setUp() {
    db.clear();
}


void tearDown() {
    db.clear();
}


void testInsert() {
    DC::Batch b;
    b.add("1234");
    db.insert(b);
    TEST_ASSERT_TRUE(db.contains("1234"));
    TEST_ASSERT_FALSE(db.contains("3456"));
}


void testRemove() {
    DC::Batch b;
    b.add("1234");
    db.insert(b);
    db.remove(b);
    TEST_ASSERT_FALSE(db.contains("1234"));
}


void testClear() {
    DC::Batch b;
    b.add("1234");
    db.insert(b);
    db.clear();
    TEST_ASSERT_FALSE(db.contains("1234"));
}


void testHash() {
    DC::Batch b;
    b.add("1234");
    b.add("3456");
    b.add("2345");
    db.insert(b);
    DC::Hash hash = db.hash();
    TEST_ASSERT_EQUAL_STRING("d2d075a5b92c4e3e1e1e8becb9f1b82173d6bb937be054adf19b95eb5e6e59c2", hash.c_str());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testInsert);
    RUN_TEST(testRemove);
    RUN_TEST(testClear);
    RUN_TEST(testHash);

    UNITY_END();
}


void loop() {


}
