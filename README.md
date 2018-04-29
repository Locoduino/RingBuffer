# RingBuffer

A simple and easy to use ring buffer library for Arduino. Interrupt safe functions are provided too.

## Limitation

The size of the ring buffer is limited to 255 elements. The compiler will not prevent you from declaring a buffer size 0 but a size 0 is not supported (and otherwise silly). Among the quirks with a size of 0 is the fact that the buffer is both empty and full.

## Using the library

First include the header in your sketch

```
#include <RingBuffer.h>
```

Instantiate a ring buffer by using the following syntax:

```
RingBuffer<type, size> myRingBuffer;
```

```type``` is the type name of each element of the ring buffer. ```size``` is the size, from 1 to 255, of the ring buffer. For instance the declaration shown below instantiate a ring buffer where each element is a ```byte``` with a size of 20.

```
RingBuffer<byte, 20> aBuffer;
```

## Functions

The following functions are available to manage the ring buffer.

### isEmpty()
```isEmpty()``` returns a ```bool``` which is ```true``` if the buffer is empty and ```false``` otherwise.

### isFull()
```isFull()``` returns a ```bool``` which is ```true``` if the buffer is full and ```false``` otherwise.

### maxSize()

```maxSize()``` returns an ```uint8_t``` which is the maximum size of the ring buffer. It is the value set when the ring buffer has been instantiated.

### size()

```size()``` returns an ```uint8_t``` which is the current size of the ring buffer. It is between 0 and ```maxSize()```.

### clear()

```clear()``` empties the ring buffer resetting its size to 0.

### push(data)

```push(data)``` pushes ```data``` at the end of the ring buffer if there is room available. ```data``` should be of the type declared when the ring buffer has been instanciated. If the data has been successfully added to the ring buffer ```true``` is returned and ```false``` otherwise. A second form exists where the argument is a pointer to the data. The use of this second form allows to reduce the number of copies. Using argument passing by value or pointer is left to your discretion

### lockedPush(data)

```lockedPush(data)``` works as ```push(data)``` except interrupts are disabled during the update of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the main program and popped by the interrupt handler.

### pop(data)

If the buffer is not empty, ```pop(data)``` pops a data from the beginning of the ring buffer, puts it in ```data``` and return ```true```. If the buffer is empty ```data``` is unchanged and ```false``` is returned.

### lockedPop(data)

```lockedPop(data)``` works as ```pop(data)```. In addition interrupts are disabled during the update of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the interrupt handler and popped by the main program.

### operator[]

The standard array element access syntax allows for direct access of elements of the ring buffer. For instance, if a buffer is declared like that:

```
RingBuffer<uint32_t, 10> aBuffer;
```

one can write:

```
uint32_t v = aBuffer[3];
```

to get the element at index 3 in the buffer.

index 0 corresponds to the first element in the buffer and index ```size() - 1``` to the last element.

If the index provided is greater than or equal to ```size()``` the element at index 0 is returned even if it does not exist. This avoids making an access outside the buffer. It is therefore up to you to verify that the index is valid. 

Note: this operator is not interrupt safe. If you need to access a circular buffer in your main program while the buffer is being manipulated by an interrupt handler, it is up to you to inhibit and restore interrupts before and after access.

## Some examples

### Add an element with error handling

```
RingBuffer<uint8_t, 10> myBuffer;

void setup()
{
  if (! myBuffer.push(32)) {
    // oups error, push failed because the buffer is full
    someErrorProcessing();
  }
}
```

### Process an element if available

```
RingBuffer<uint8_t, 10> myBuffer;

void setup()
{
  uint8_t data;
  if (myBuffer.pop(data)) {
    // pop put something in data
    Serial.println(data);
  }
}
```

### Fill a buffer with values from 10 to 1 then print all the elements

```
#include <RingBuffer.h>

RingBuffer<uint8_t, 10> myBuffer;

void setup()
{
  uint8_t i = 10;
  while(myBuffer.push(i--));
  Serial.begin(115200);
  for (uint8_t j = 0; j < myBuffer.size(); j++) {
    Serial.println(myBuffer[j]);
  }
}

void loop()
{
}
```
