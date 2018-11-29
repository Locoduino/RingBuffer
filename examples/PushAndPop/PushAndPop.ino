/*
   Simple test of the ring buffer

   Expected output:

  0
  1
  2
  3
  4
  5
  6
  7
  --------
  0
  1
  2
  3
  4
  5
  6
  7
  --------
  9
  10
  11
  12
  13
  --------
  9
  --------
  10
  11
  12
  13
  --------
  10
  11
  12
  13
  9

*/

#include <RingBuf.h>

RingBuf<uint8_t, 10> myBuffer;

void setup()
{
  Serial.begin(115200);

  uint8_t i = 0;
  while (myBuffer.push(i++));
  for (uint8_t j = 0; j < myBuffer.size(); j++) Serial.println(myBuffer[j]);
  Serial.println("--------");
  uint8_t k;
  while (myBuffer.pop(k)) Serial.println(k);
  Serial.println("--------");
  for (uint8_t j = 0; j < 5; j++) myBuffer.push(i++);
  for (uint8_t j = 0; j < myBuffer.size(); j++) Serial.println(myBuffer[j]);
  Serial.println("--------");
  Serial.flush();
  myBuffer.pop(k);
  Serial.println(k);
  Serial.println("--------");
  for (uint8_t j = 0; j < myBuffer.size(); j++) Serial.println(myBuffer[j]);
  Serial.println("--------");
  myBuffer.push(k);
  for (uint8_t j = 0; j < myBuffer.size(); j++) Serial.println(myBuffer[j]);
}

void loop()
{

}
