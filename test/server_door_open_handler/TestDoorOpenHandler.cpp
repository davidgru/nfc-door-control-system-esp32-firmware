#include <unity.h>


#include <server.hpp>


class TestRequest : public DC::Request {
public:

    TestRequest(const DC::Body& body = "", const String& header = "") 
        : body(body), header(header) {

    }

    DC::Method getMethod() const {
        return DC::Method::GET;
    }

    DC::Body getBody() const {
        return body;
    }

    bool hasHeader(const String& name) const {
        return true;
    }

    String getHeader(const String& name) const {
        return header;
    }

private:
    DC::Body body;
    String header;
};


class DummyValidator : public DC::Validator<DC::PSK> {
public:
    bool isValid(const DC::PSK& psk) {
        return true;
    }
};


#define DOOR_TEST_PIN DOOR_OPEN_PIN
#define DOOR_TEST_DURATION DOOR_OPEN_DURATION_MILLIS


DummyValidator validator;
DC::DoorOpener doorOpener(DOOR_TEST_PIN, DOOR_TEST_DURATION);

DC::DoorOpenHandler doorOpenHandler(validator, doorOpener);


void testMethod() {
    TEST_ASSERT_EQUAL_INT(DC::Method::POST, doorOpenHandler.getMethod());
}


void testURI() {
    TEST_ASSERT_EQUAL_STRING(SERVER_DOOR_URI, doorOpenHandler.getURI().c_str());
}


void testOpen() {
    TestRequest req;
    DC::Response resp = doorOpenHandler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, resp.getStatusCode());
    TEST_ASSERT_EQUAL_INT(HIGH, digitalRead(DOOR_TEST_PIN));
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testMethod);
    RUN_TEST(testURI);
    RUN_TEST(testOpen);

    UNITY_END();
}


void loop() {


}
