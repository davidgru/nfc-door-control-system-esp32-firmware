#include <unity.h>


#include <server.hpp>



void test() {
    DC::Response response(200, "blabla");
    TEST_ASSERT_EQUAL_INT(200, response.getStatusCode());
    TEST_ASSERT_EQUAL_STRING("blabla", response.getBody().c_str());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test);

    UNITY_END();
}


void loop() {


}
