/*
 * Communication between the interrupt handler and the main program
 */
#include <RingBuffer.h>
 
RingBuffer<uint32_t, 10> comBuffer;

void externalInterruptHandler()
{
  digitalWrite(LED_BUILTIN, ! comBuffer.push(millis()));
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
  Serial.print('<');
  Serial.print(comBuffer.size());
  Serial.print("> ");
  if (comBuffer.lockedPop(pushDate)) {
    Serial.println(pushDate);
  }
  else {
    Serial.println();
  }
  delay(3000);
}
