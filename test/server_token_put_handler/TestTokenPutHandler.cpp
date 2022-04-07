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

DC::TokenPutHandler handler(validator, db);


void testMethod() {
    TEST_ASSERT_EQUAL_INT(DC::Method::PUT, handler.getMethod());
}


void testURI() {
    TEST_ASSERT_EQUAL_STRING(SERVER_TOKEN_URI, handler.getURI().c_str());
}


void testUnableToParse() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": \"1234\",\n"
    "            \"toPut\": \"5678\"\n"
    "        },\n"
    "        {\n"
    "            \"toReplace\": \"2345\",\n"
    "            \"toPut\": \"6789\"\n"
    "        }\n"
    "    ]\n"
    "\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testTokensMissing() {
    String body = \
    "{\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testTokensNotArray() {
    String body = \
    "{\n"
    "    \"tokens\": \"This is no array\"\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testEntryNotObject() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": \"1234\",\n"
    "            \"toPut\": \"5678\"\n"
    "        },\n"
    "        \"This is no array\"\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testToReplaceNotString() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": 1234,\n"
    "            \"toPut\": \"5678\"\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testNoToPut() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": \"1234\"\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testToPutNotString() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": \"1234\",\n"
    "            \"toPut\": 5678\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testTokenToLong() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toPut\": \"0123456789012345678901234567890123456789012345678901234567890123456789\"\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testTokenInvalid() {
    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toPut\": \"0123p\"\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_BAD_REQUEST, resp.getStatusCode());
}


void testValid() {
    DC::Batch b;
    b.add("1234");
    b.add("2345");
    b.add("4567");

    db.clear();
    db.insert(b);

    String body = \
    "{\n"
    "    \"tokens\": [\n"
    "        {\n"
    "            \"toReplace\": \"1234\",\n"
    "            \"toPut\": \"5678\"\n"
    "        },\n"
    "        {\n"
    "            \"toReplace\": \"2345\",\n"
    "            \"toPut\": \"6789\"\n"
    "        }\n"
    "    ]\n"
    "}\n";
    TestRequest req(body);
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, resp.getStatusCode());
    TEST_ASSERT_EQUAL_STRING("dd73b12d5a7ffe10efb87cfcfb3aec24f7e4a642c671b5e7fe62c748796b491c", db.hash().c_str()); // hash of "456756786789"
}


void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testUnableToParse);
    RUN_TEST(testTokensMissing);
    RUN_TEST(testTokensNotArray);
    RUN_TEST(testEntryNotObject);
    RUN_TEST(testToReplaceNotString);
    RUN_TEST(testNoToPut);
    RUN_TEST(testToPutNotString);
    RUN_TEST(testTokenToLong);
    RUN_TEST(testTokenInvalid);
    RUN_TEST(testValid);

    UNITY_END();
}


void loop() {


}
