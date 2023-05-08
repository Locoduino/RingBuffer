
/* 
 *  Test of the ring buffer with overwrite
 *  
 *  Expected output:

0
1
2
3
4
--------
2
3
4
6
7
--------
4
6
7
--------
6
7
0
1
2
--------
1
2
3
4
5

 */
#include <RingBuf.h>

RingBuf<uint8_t, 5> myBuffer;

void setup() {
  Serial.begin(115200);

  uint8_t i = 0;
  /* Fill the buffer */
  while (myBuffer.push(i++))
    ;
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
  Serial.println("--------");
  /* Overwrite it with 2 elements */
  myBuffer.pushOverwrite(i++);
  myBuffer.pushOverwrite(i++);
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
  Serial.println("--------");
  /* pop 2 values */
  uint8_t val;
  myBuffer.pop(val);
  myBuffer.pop(val);
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
  Serial.println("--------");
  /* push and overwrite 3 values */
  for (i = 0; i < 3; i++)
    myBuffer.pushOverwrite(i);
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
  Serial.println("--------");
  /* push and overwrite 3 more values */
  for (i = 3; i < 6; i++)
    myBuffer.pushOverwrite(i);
  for (uint8_t j = 0; j < myBuffer.size(); j++)
    Serial.println(myBuffer[j]);
}

void loop() {}
