#include <unity.h>


#include <NFCTokenValidator.hpp>
#include <database.hpp>


DC::SortedDatabase db("/test/tokens");
DC::NFCTokenValidator validator(TOKEN_SEED, db);


void setUp() {
    db.clear();
}


void testValid() {
    DC::Batch b;
    b.add("9ff15610b4765ed84c182dd4c804ef7a9d2bfc7d8dd320fdcca8e4fef496b713"); // sha256(1234abcdef01)
    db.insert(b);
    TEST_ASSERT_TRUE(validator.isValid("abcdef01"));
}


void testInvalid() {
    TEST_ASSERT_FALSE(validator.isValid("abcdef01"));
}


void setup() {
    delay(200);
    UNITY_BEGIN();

    RUN_TEST(testValid);

    RUN_TEST(testInvalid);

    UNITY_END();
}

void loop() {

}
