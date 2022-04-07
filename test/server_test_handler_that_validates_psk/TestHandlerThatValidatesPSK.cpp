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
        return header != "";
    }

    String getHeader(const String& name) const {
        return header;
    }

private:
    DC::Body body;
    String header;
};


class TestHandler : public DC::HandlerThatValidatesPSK, DC::Validator<DC::PSK> {
public:

    TestHandler() : HandlerThatValidatesPSK((DC::Validator<DC::PSK>&)*this) {

    }

    DC::Method getMethod() const {
        return DC::Method::GET;
    }
    
    const DC::URI& getURI() const {
        static DC::URI uri = "uri";
        return uri;
    }


    DC::Response onAuthorizedRequest(const DC::Request& request) override {
        return {STATUS_OK, "Response"};
    }

    bool isValid(const DC::PSK& psk) override {
        return psk == "1234";
    }

};

TestHandler handler;


void testMissingPSK() {
    TestRequest req;
    DC::Response response = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_UNAUTHORIZED, response.getStatusCode());
}


void testInvalidPSK() {
    TestRequest req("", "2345");
    DC::Response response = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_UNAUTHORIZED, response.getStatusCode());
}

void testCorrectPSK() {
    TestRequest req("", "1234");
    DC::Response response = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, response.getStatusCode());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testMissingPSK);
    RUN_TEST(testInvalidPSK);
    RUN_TEST(testCorrectPSK);

    UNITY_END();
}


void loop() {


}
