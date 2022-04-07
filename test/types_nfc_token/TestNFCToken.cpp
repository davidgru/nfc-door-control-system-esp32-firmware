#include <unity.h>


#include <types.hpp>



void testValidRepr() {
    TEST_ASSERT_TRUE(DC::NFCToken::isValidTokenRepr(String("abcdef01")));
    TEST_ASSERT_FALSE(DC::NFCToken::isValidTokenRepr(String("abcdef015"))); // length not even
    TEST_ASSERT_FALSE(DC::NFCToken::isValidTokenRepr(String("abcdefl12"))); // invalid digit
}


void testGetLengthInBytes() {
    DC::NFCToken token = "abcdef01";
    TEST_ASSERT_EQUAL(4, token.getLengthInBytes());
}


void testGetBuffer() {
    DC::NFCToken token = "abcdef01";
    byte expected[] = {0xab, 0xcd, 0xef, 0x01};
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, token.getBuffer(), 4);
}

void testToHex() {
    byte buf[] = {0xab, 0xcd, 0xef, 0x01};
    DC::NFCToken token((const uint8_t*)buf, 4);
    TEST_ASSERT_EQUAL_STRING("abcdef01", token.toHexRepr().c_str());
}


void testEquals() {
    DC::NFCToken token = "abcdef01";
    TEST_ASSERT_TRUE(token == "abcdef01");
    TEST_ASSERT_FALSE(token == "abcdef02");
}


void testNotEquals() {
    DC::NFCToken token = "abcdef01";
    TEST_ASSERT_FALSE(token != "abcdef01");
    TEST_ASSERT_TRUE(token != "abcdef02");
}


void testLessThan() {
    DC::NFCToken token = "abcdef01";
    TEST_ASSERT_TRUE(token < "abcdef02");
    TEST_ASSERT_TRUE(token < "fbcdef01");
    TEST_ASSERT_FALSE(token < "abcdef01");
    TEST_ASSERT_FALSE(token < "0bcdef01");
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testValidRepr);
    RUN_TEST(testGetLengthInBytes);
    RUN_TEST(testGetBuffer);
    RUN_TEST(testToHex);
    RUN_TEST(testEquals);
    RUN_TEST(testNotEquals);
    RUN_TEST(testLessThan);

    UNITY_END();
}


void loop() {

}
