/*
  This demo showcases the use of the constructor callback initHelper
  to control whether RingBuf should initialize its internal counters.
  This is useful when you need to put the controller to deep sleep
  and whant the data in the buffer to be preserved in the RTC memory.
  In this example ESP32 functions are used to put the controller to
  ultra low power mode, and to check whether it is initial run or
  the code executes after wake-up.

  Without it the buffer will start over after each wakeup, like this:

  Wakeup! #1
  myBuffer-before: []
  myBuffer-after: [1, ]
  Sleeping...

  Wakeup! #2
  myBuffer-before: []
  myBuffer-after: [2, ]
  Sleeping...

  Wakeup! #3
  myBuffer-before: []
  myBuffer-after: [3, ]
  Sleeping...

  But with the callback configured, the output shows that the data
  is persisted between weakeups:

  Wakeup! #1
  myBuffer-before: []
  myBuffer-after: [1, ]
  Sleeping...

  Wakeup! #2
  myBuffer-before: [1, ]
  myBuffer-after: [1, 2, ]
  Sleeping...

  Wakeup! #3
  myBuffer-before: [1, 2, ]
  myBuffer-after: [1, 2, 3, ]
  Sleeping...
 */

#include <RingBuf.h>

// store the counter in the RTC memory
RTC_DATA_ATTR uint32_t wakeupCounter = 0;

// store the buffer in the RTC memory
RTC_DATA_ATTR RingBuf<uint32_t, 8> myBuffer([]() -> bool {
  // Check whether the code is being executed after wakeup
  return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED; 
});

void setup() {
  Serial.begin(115200);
  Serial.print("Wakeup! #");
  Serial.println(++wakeupCounter);

  Serial.print("myBuffer-before: ["); 
  for (uint8_t i = 0; i < myBuffer.size(); ++i) {
    Serial.print(myBuffer[i]); Serial.print(", ");
  }
  Serial.println("]");

  myBuffer.pushOverwrite(wakeupCounter);

  Serial.print("myBuffer-after: [");
  for (uint8_t i = 0; i < myBuffer.size(); ++i) {
    Serial.print(myBuffer[i]); Serial.print(", ");
  } 
  Serial.println("]");

  Serial.println("Sleeping...");
  Serial.flush();

  // sleep 5 seconds
  esp_deep_sleep(5000000);
}

void loop() {
  // never executed
}
