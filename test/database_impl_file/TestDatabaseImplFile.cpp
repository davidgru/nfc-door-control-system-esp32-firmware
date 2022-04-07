#include <unity.h>


#include <database.hpp>


DC::DatabaseImplFile file("/test/tokens");


void setUp() {
    file.clear();
}


void tearDown() {
    file.clear();
}


void testInsert() {
    file.insert("1234");
    TEST_ASSERT_TRUE(file.contains("1234"));
    TEST_ASSERT_FALSE(file.contains("2345"));
}


void testRemove() {
    file.insert("1234");
    file.remove("1234");
    TEST_ASSERT_FALSE(file.contains("1234"));
}


void testClear() {
    file.insert("1234");
    file.clear();
    TEST_ASSERT_FALSE(file.contains("1234"));
}


void testForeach() {
    file.insert("1234");
    file.insert("2345");
    file.insert("3456");

    size_t c = 0;

    file.foreach([&](const DC::NFCToken& token){
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


    UNITY_END();
}


void loop() {


}
