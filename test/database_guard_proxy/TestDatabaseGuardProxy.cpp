#include <unity.h>


#include <database.hpp>

DC::DatabaseGuardProxy<DC::SortedDatabase> db("/test/tokens");


void setUp() {
    db.clear();
}


void tearDown() {
    db.clear();
}


volatile bool letsgo = false;
volatile int arrived = 0;
SemaphoreHandle_t sem;

static void taskRoutine(void* arg) {
    DC::Batch b;
    b.add((const char*)arg);

    while(!letsgo);
    db.insert(b);
    xSemaphoreTake(sem, portMAX_DELAY);
    arrived++;
    xSemaphoreGive(sem);

    vTaskDelete(NULL);
}

void testInsert() {
    letsgo = false;
    arrived = 0;
    sem = xSemaphoreCreateMutex();
    xTaskCreate(taskRoutine, "1", 4096, (void*)"1234", tskIDLE_PRIORITY, NULL);
    xTaskCreate(taskRoutine, "2", 4096, (void*)"3456", tskIDLE_PRIORITY, NULL);
    xTaskCreate(taskRoutine, "3", 4096, (void*)"2345", tskIDLE_PRIORITY, NULL);
    delay(200);
    letsgo = true;
    while(arrived != 3);
    DC::Hash hash = db.hash();
    TEST_ASSERT_EQUAL_STRING("d2d075a5b92c4e3e1e1e8becb9f1b82173d6bb937be054adf19b95eb5e6e59c2", hash.c_str());
}


void testRemove() {
    DC::Batch b;
    b.add("1234");
    db.insert(b);
    db.remove(b);
    TEST_ASSERT_FALSE(db.contains("1234"));
}


void testClear() {
    DC::Batch b;
    b.add("1234");
    db.insert(b);
    db.clear();
    TEST_ASSERT_FALSE(db.contains("1234"));
}


void testHash() {
    DC::Batch b;
    b.add("1234");
    b.add("3456");
    b.add("2345");
    db.insert(b);
    DC::Hash hash = db.hash();
    TEST_ASSERT_EQUAL_STRING("d2d075a5b92c4e3e1e1e8becb9f1b82173d6bb937be054adf19b95eb5e6e59c2", hash.c_str());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testInsert);
    RUN_TEST(testRemove);
    RUN_TEST(testClear);
    RUN_TEST(testHash);

    UNITY_END();
}


void loop() {


}
