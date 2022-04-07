#include <Arduino.h>
#include <unity.h>


#include <nfc.hpp>


class TestValidator : public DC::Validator<DC::NFCToken> {
public:
    bool isValid(const DC::NFCToken& token) override {
        return token == "abcdef01";
    }
};


DC::DoorOpener opener(DOOR_OPEN_PIN, DOOR_OPEN_DURATION_MILLIS);
TestValidator validator;

DC::NFCTokenHandler handler(opener, validator);


void tearDown() {
    delay(DOOR_OPEN_DURATION_MILLIS);
}


void testValid() {
    handler.processToken("abcdef01");
    TEST_ASSERT_EQUAL(HIGH, digitalRead(DOOR_OPEN_PIN));
}


void testInvalid() {
    handler.processToken("abcdef02");
    TEST_ASSERT_EQUAL(LOW, digitalRead(DOOR_OPEN_PIN));
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
