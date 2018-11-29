/*
 * Communication between the interrupt handler and the main program
 *
 * The interrupt handler is executed when a falling edge appear on pin 2
 * It pushes the current date in microseconds.
 *
 * Use it with a push button to see the bounces
 */
#include <RingBuf.h>

RingBuf<uint32_t, 10> comBuffer;

void externalInterruptHandler()
{
  digitalWrite(LED_BUILTIN, ! comBuffer.push(micros()));
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("Let's go !");

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), externalInterruptHandler, FALLING);
}

void loop()
{
  uint32_t pushDate;
  /* Print the current size of the buffer */
  Serial.print('<');
  Serial.print(comBuffer.size());
  Serial.print("> ");
  /* If the buffer is full, reset the overflow alert */
  if (comBuffer.isFull()) digitalWrite(LED_BUILTIN, LOW);
  if (comBuffer.lockedPop(pushDate)) {
    /* Print the popped element */
    Serial.println(pushDate);
  }
  else {
    Serial.println();
  }
  delay(3000);
}
