/*
   Simple test of the ring buffer with strings.
*/

#include <RingBuf.h>

RingBuf<String, 8> myBuffer;

void setup() {
  Serial.begin(9600);
  String szavak[8] = {"alma", "fa", "alatt", "ez", "az", "stb", "bbbbb", "cccc"};
  uint8_t i = 0;
  while (myBuffer.push(szavak[i++]))
    ;
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
  Serial.println("--------");
}

void loop() {}