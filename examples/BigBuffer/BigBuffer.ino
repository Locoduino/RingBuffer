/*
 * Test of a big buffer (greater than 255)
 *
 * Output should be
 * Filled with 300 values
 * Buffer agrees !
 * Checking 300 values... 0 error(s) found
 * Popping the values... popped 300 values, 0 wrong
 */

#include <RingBuf.h>

RingBuf<uint16_t, 300> myBuffer;

void setup()
{
  Serial.begin(115200);

  uint16_t i = 0;

  /* fill the buffer */
  while (! myBuffer.isFull()) {
    myBuffer.push(i++);
  }
  Serial.print("Filled with ");
  Serial.print(i);
  Serial.println(" values");

  /* Check the buffer agrees */
  if (myBuffer.size() == i) {
    Serial.println("Buffer agrees !");
  }
  else {
    Serial.println("*** Error: Buffer does not agree !");
  }

  /* Check the values */
  Serial.print("Checking ");
  Serial.print(myBuffer.size());
  Serial.print(" values... ");
  uint16_t errorCount = 0;
  for (uint16_t j = 0; j < myBuffer.size(); j++) {
    if (myBuffer[j] != j) errorCount++;
  }
  Serial.print(errorCount);
  Serial.println(" error(s) found");

  /* Pop the values */
  Serial.print("Popping the values... ");
  errorCount = 0;
  uint16_t value;
  i = 0;
  while (myBuffer.pop(value)) {
    if (value != i++) errorCount++;
  }
  Serial.print("popped ");
  Serial.print(i);
  Serial.print(" values, ");
  Serial.print(errorCount);
  Serial.println(" wrong");
}

void loop()
{

}
