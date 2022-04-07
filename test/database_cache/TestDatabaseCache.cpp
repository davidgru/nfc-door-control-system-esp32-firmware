#include <unity.h>


#include <database.hpp>

DC::DatabaseCache cache(DC_DATABASE_CAPACITY);


void setUp() {
    cache.clear();
}


void tearDown() {
    cache.clear();
}


void testInsert() {
    cache.insert("1234");
    TEST_ASSERT_TRUE(cache.contains("1234"));
    TEST_ASSERT_FALSE(cache.contains("3456"));
}


void testRemove() {
    cache.insert("1234");
    cache.remove("1234");
    TEST_ASSERT_FALSE(cache.contains("1234"));
}


void testClear() {
    cache.insert("1234");
    cache.clear();
    TEST_ASSERT_FALSE(cache.contains("1234"));
}


void testForeach() {
    cache.insert("1234");
    cache.insert("2345");
    cache.insert("3456");

    size_t c = 0;

    cache.foreach([&](const DC::NFCToken& token){
        if (c == 0) {
            TEST_ASSERT_EQUAL_STRING("1234", token.toHexRepr().c_str());
        }
        if (c == 1) {
            TEST_ASSERT_EQUAL_STRING("2345", token.toHexRepr().c_str());
        }
        if (c == 2) {
            TEST_ASSERT_EQUAL_STRING("3456", token.toHexRepr().c_str());
        }
        c++;
        return false;
    });
    TEST_ASSERT_EQUAL(3, c);
}


void testLoadFromFile() {
    DC::DatabaseImplFile file("/test/tokens");
    file.clear();
    file.insert("1234");
    file.insert("2345");
    file.insert("3456");
    cache.loadFromFile(file);

    size_t c = 0;

    cache.foreach([&](const DC::NFCToken& token){
        if (c == 0) {
            TEST_ASSERT_EQUAL_STRING("1234", token.toHexRepr().c_str());
        }
        if (c == 1) {
            TEST_ASSERT_EQUAL_STRING("2345", token.toHexRepr().c_str());
        }
        if (c == 2) {
            TEST_ASSERT_EQUAL_STRING("3456", token.toHexRepr().c_str());
        }
        c++;
        return false;
    });
    TEST_ASSERT_EQUAL(3, c);

}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testInsert);
    RUN_TEST(testRemove);
    RUN_TEST(testClear);
    RUN_TEST(testForeach);
    RUN_TEST(testLoadFromFile);

    UNITY_END();
}


void loop() {


}
