#include <unity.h>


#include <validator.hpp>



DC::PSKValidator validator("PSK");


void testValid() {
    TEST_ASSERT_TRUE(validator.isValid("PSK"));
}


void testInvalid() {
    TEST_ASSERT_FALSE(validator.isValid("QSK"));
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testValid);
    RUN_TEST(testInvalid);

    UNITY_END();
}


void loop() {


}
