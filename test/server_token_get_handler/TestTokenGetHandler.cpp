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

DC::TokenGetHandler handler(validator, db);


void testMethod() {
    TEST_ASSERT_EQUAL_INT(DC::Method::GET, handler.getMethod());
}


void testURI() {
    TEST_ASSERT_EQUAL_STRING(SERVER_TOKEN_URI, handler.getURI().c_str());
}


void testHash() {
    DC::Batch b;
    b.add("1234");
    b.add("2345");
    b.add("3456");
    db.clear();
    db.insert(b);

    TestRequest req;
    DC::Response resp = handler.onRequest(req);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, resp.getStatusCode());
    
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, resp.getBody());

    TEST_ASSERT_TRUE(doc.containsKey("hash"));
    auto hashEntry = doc.getMember("hash");
    TEST_ASSERT_TRUE(hashEntry.is<String>());
    TEST_ASSERT_EQUAL_STRING(db.hash().c_str(), hashEntry.as<String>().c_str());
}


void setup() {
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(testMethod);
    RUN_TEST(testURI);
    RUN_TEST(testHash);

    UNITY_END();
}


void loop() {


}
