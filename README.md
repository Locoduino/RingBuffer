[![GitHub release](https://img.shields.io/github/release/Locoduino/RingBuffer.svg)](https://github.com/Locoduino/RingBuffer/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/Locoduino/RingBuffer/1.0.4.svg)](https://github.com/Locoduino/RingBuffer/compare/1.0.4...master) [![Build Status](https://travis-ci.org/Locoduino/RingBuffer.svg?branch=master)](https://travis-ci.org/Locoduino/RingBuffer)


# RingBuffer 

A simple and easy to use ring buffer library for Arduino. Interrupt safe functions are provided too.

## Changelog

- 1.0.5 Add the possibility of preserving the buffer through a reset. Thanks to Vitalii Stepchyk (@vstepchik) for the example.
- 1.0.4 Added methods to push an element and overwrite the oldest data.
- 1.0.3 Changed the way templates are instanciated. Now, a size greater than 255 is allowed and leads to a uint16_t datatype used for size and index. In addition, wrong size are detected and a compilation error is emited. Added example ```BigBuffer``` with a size over 255.
- 1.0.2 Changed the name of the template from RingBuffer to RingBuf in order to avoid a name conflict with and internal RingBuffer class used in the ARM version of the Arduino core.
- 1.0.1 Fix a mistake in pop documentation
- 1.0   Initial release.

## Limitation

The size of the ring buffer is limited to 65535 elements. The compiler will emit an error is the buffer size is 0 or if the buffer size is greated than 65535.

## Using the library

First include the header in your sketch

```
#include <RingBuf.h>
```

Instantiate a ring buffer by using the following syntax:

```
RingBuf<type, size> myRingBuffer;
```

```type``` is the type name of each element of the ring buffer. ```size``` is the size, from 1 to 65535, of the ring buffer. For instance the declaration shown below instantiate a ring buffer where each element is a ```byte``` with a size of 20.

```
RingBuf<byte, 20> aBuffer;
```

It is also possible to instantiate a buffer by passing a function pointer to the constructor. The function whose pointer is passed must have the following prototype:

```
bool f();
```

The constructor calls the function to determine whether the buffer should have its buffer management member data (current size and read/write index) initialized. If the function returns ```false``` then the members are initialized, otherwise they are not.

This is useful if the buffer is housed in permanent memory and needs to retain its data during a reset. Usually, the cause of the reset is available, allowing you to find out whether the startup is the result of the machine powering up, or whether it is the result of some other cause (deep sleep, reset button, watchdog, ...).

For example, the ESP32 is reset when it comes out of deep sleep. If we wish to keep data in the buffer, we'll allocate it in the RTC RAM, which remains powered during deep sleep. To prevent the buffer from being reset when it comes out of deep sleep, we'll use its constructor as follows:

```
RTC_DATA_ATTR RingBuf<uint32_t, 8> myBuffer([]() -> bool {
  return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED; 
});
```

The function is passed in lamba form and returns ```true``` if the cause is a normal exit from deep sleep and ```false``` otherwise. The result will be that, in the event of a normal exit from deep sleep, the buffer will be preserved as it was at the time of entry into deep sleep, and will be initialized if the cause of the reset is something else. A complete example is given at the end of this document.

## Member functions

The following member unctions are available to manage the ring buffer.

### isEmpty()
```isEmpty()``` returns a ```bool``` which is ```true``` if the buffer is empty and ```false``` otherwise.

### isFull()
```isFull()``` returns a ```bool``` which is ```true``` if the buffer is full and ```false``` otherwise.

### maxSize()

```maxSize()``` returns an ```uint8_t``` for buffers of size lower or equal to 255 and an ```uint16_t``` for buffers of size in the [256, 65535] interval. It is the value set when the ring buffer has been instantiated.

### size()

```size()``` returns an ```uint8_t``` for buffers of size lower or equal to 255 and an ```uint16_t``` for buffers of size in the [256, 65535] interval, which is the current size of the ring buffer. It is between 0 and ```maxSize()```.

### clear()

```clear()``` empties the ring buffer resetting its size to 0.

### push(data)

```push(data)``` pushes ```data``` at the end of the ring buffer if there is room available. ```data``` should be of the type declared when the ring buffer has been instanciated. If the data has been successfully added to the ring buffer ```true``` is returned and ```false``` otherwise. A second form exists where the argument is a pointer to the data. The use of this second form allows to reduce the number of copies. Using argument passing by value or pointer is left to your discretion

### pushOverwrite(data)

```pushOverwrite(data)``` pushes ```data``` at the end of the ring buffer if there is room available or overwrite the oldest data if there is no room. ```data``` should be of the type declared when the ring buffer has been instanciated. If the data has been added to the ring buffer without overwriting the oldest data ```true``` is returned and ```false``` if an overwrite occured. A second form exists where the argument is a pointer to the data. The use of this second form allows to reduce the number of copies. Using argument passing by value or pointer is left to your discretion

### lockedPush(data)

```lockedPush(data)``` works as ```push(data)``` except interrupts are disabled during the update of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the main program and popped by the interrupt handler.

### lockedPushOverwrite(data)

```lockedPushOverwrite(data)``` works as ```pushOverwite(data)``` except interrupts are disabled during the update of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the main program and popped by the interrupt handler.

### pop(data)

If the buffer is not empty, ```pop(data)``` pops a data from the beginning of the ring buffer, puts it in ```data``` and return ```true```. If the buffer is empty ```data``` is unchanged and ```false``` is returned.

### lockedPop(data)

```lockedPop(data)``` works as ```pop(data)```. In addition interrupts are disabled during the update of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the interrupt handler and popped by the main program.

### peek(data, distance)

If the buffer is not empty, ```peek(data, distance)``` peeks a data from the next x-elements without poping them and puts it in ```data``` and return ```true```. If the buffer is empty ```data``` is unchanged and ```false``` is returned.

### lockedPeek(data, distance)

```lockedPeek(data, distance)``` works as ```peek(data, distance)```. In addition interrupts are disabled during the read of the ring buffer. You should use this function in your main program if the ring buffer is shared between the main program and an interrupt handler and data are pushed by the interrupt handler and popped by the main program.

### operator[]

The standard array element access syntax allows for direct access of elements of the ring buffer. For instance, if a buffer is declared like that:

```
RingBuf<uint32_t, 10> aBuffer;
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

```ino
RingBuf<uint8_t, 10> myBuffer;

void setup()
{
  if (! myBuffer.push(32)) {
    // oups error, push failed because the buffer is full
    someErrorProcessing();
  }
}
```

### Process an element if available

```ino
RingBuf<uint8_t, 10> myBuffer;

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

```ino
#include <RingBuf.h>

RingBuf<uint8_t, 10> myBuffer;

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
### Fill a buffer with values from 10 to 1 then pop and peek the next element

```ino
#include <RingBuf.h>

RingBuf<uint8_t, 10> myBuffer;

void setup()
{
  uint8_t i = 10;
  while(myBuffer.push(i--));
  Serial.begin(115200);

  while(myBuffer.pop(i)){
    Serial.printf("Value: %d ", i);
    if (myBuffer.peek(i, 0)) {
      Serial.printf("Next: %d\n", i);
    } else {
      Serial.println("Next: - ");
    }
  }
}

void loop()
{
}
```
### Skip buffer initialization in ESP32 RTC memory when executed after wakeup from deep sleep

```ino
#include <RingBuf.h>

RTC_DATA_ATTR uint32_t wakeupCounter = 0;
RTC_DATA_ATTR RingBuf<uint32_t, 8> myBuffer([]() -> bool {
  return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED; 
});

void setup()
{
  Serial.begin(115200);
  Serial.print("Wakeup! #");
  Serial.println(++wakeupCounter);
  myBuffer.pushOverwrite(wakeupCounter);

  Serial.print("myBuffer: ");
  for (uint8_t i = 0; i < myBuffer.size(); ++i) {
    Serial.print(myBuffer[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.flush();
  esp_deep_sleep(5000000);
}

void loop()
{
}
```

## Note about Travis CI

The RingBuffer Library examples are built on Travis CI for the following boards:

- Arduino Leonardo
- Arduino Uno
- Arduino Mega 2560
- Arduino Zero
- Arduino Due
