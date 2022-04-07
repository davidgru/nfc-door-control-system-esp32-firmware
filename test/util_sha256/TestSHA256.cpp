
#include <Arduino.h>
#include <unity.h>


#include <util.hpp>


void testEmpty() {
    DC::SHA256 f;
    TEST_ASSERT_EQUAL_STRING("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", f.digestToHex().c_str());
}


void testChunks() {
    DC::SHA256 f;
    f.update("1234");
    f.update("5678");
    f.update("9012");
    TEST_ASSERT_EQUAL_STRING("2a33349e7e606a8ad2e30e3c84521f9377450cf09083e162e0a9b1480ce0f972", f.digestToHex().c_str());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testEmpty);
    RUN_TEST(testChunks);

    UNITY_END();
}

void loop() {
    // No need.
}
