#include <unity.h>


#include <ArduinoJson.h>
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


DummyValidator validator;
DC::SortedDatabase db("/test/token");

DC::TokenDeleteHandler handler(validator, db);


void testMethod() {
    TEST_ASSERT_EQUAL_INT(DC::Method::DELETE, handler.getMethod());
}


void testURI() {
    TEST_ASSERT_EQUAL_STRING(SERVER_TOKEN_URI, handler.getURI().c_str());
}


void testUnableToParse() {
    String body = \
    "{\n"
    "    \"deleteAll\": true\n"
    "\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testDeleteAllNotBool()  {
    String body = \
    "{\n"
    "    \"deleteAll\": fgfg\n"
    "\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testTokenNotString() {
    String body = \
    "{\n"
    "   \"tokens\": [\n"
    "       \"1234\",\n"
    "       2345\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testDeleteAll() {
    DC::Batch b;
    b.add("1234");
    b.add("2345");
    b.add("4567");

    String body = \
    "{\n"
    "    \"deleteAll\": true\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, resp.getStatusCode());
    TEST_ASSERT_EQUAL_STRING("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", db.hash().c_str()); // hash of ""
}


void testDeleteSome() {
    DC::Batch b;
    b.add("1234");
    b.add("2345");
    b.add("4567");
    db.clear();
    db.insert(b);

    String body = \
    "{\n"
    "   \"tokens\": [\n"
    "       \"1234\",\n"
    "       \"4567\"\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, resp.getStatusCode());
    TEST_ASSERT_EQUAL_STRING("38083c7ee9121e17401883566a148aa5c2e2d55dc53bc4a94a026517dbff3c6b", db.hash().c_str()); // hash of "2345"
}


void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testMethod);
    RUN_TEST(testURI);
    RUN_TEST(testUnableToParse);
    RUN_TEST(testDeleteAllNotBool);
    RUN_TEST(testTokenNotString);
    RUN_TEST(testDeleteAll);
    RUN_TEST(testDeleteSome);

    UNITY_END();
}


void loop() {


}
