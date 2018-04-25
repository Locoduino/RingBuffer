/*
 * Ring Buffer Library for Arduino
 *
 * Copyright Jean-Luc Béchennec 2018
 *
 * This software is distributed under the GNU Public Licence v2 (GPLv2)
 *
 * Please read the LICENCE file
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <Arduino.h>

template <typename rg_element_t, uint8_t maxSize>
class RingBuffer
{
private:
  rg_element_t mBuffer[maxSize];
  uint8_t mReadIndex;
  uint8_t mSize;

  uint8_t writeIndex();

public:
  RingBuffer();
  bool push(const rg_element_t inElement);
  bool push(const rg_element_t * const inElement);
  bool lockedPush(const rg_element_t inElement);
  bool lockedPush(const rg_element_t * const inElement);
  bool pop(rg_element_t &outElement);
  bool lockedPop(rg_element_t &outElement);
  bool isFull()  { return mSize == maxSize; }
  bool isEmpty() { return mSize == 0; }
  void clear()   { mSize = 0; }
};

template <typename rg_element_t, uint8_t maxSize>
uint8_t RingBuffer<rg_element_t, maxSize>::writeIndex()
{
 uint8_t wi = mReadIndex + mSize;
 if (wi >= maxSize) wi -= maxSize;
 return wi;
}

template <typename rg_element_t, uint8_t maxSize>
RingBuffer<rg_element_t, maxSize>::RingBuffer() :
mReadIndex(0),
mSize(0)
{
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::push(const rg_element_t inElement)
{
  if (isFull()) return false;
  mBuffer[writeIndex()] = inElement;
  mSize++;
  return true;
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::push(const rg_element_t * const inElement)
{
  if (isFull()) return false;
  mBuffer[writeIndex()] = *inElement;
  mSize++;
  return true;
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::lockedPush(const rg_element_t inElement)
{
  if (isFull()) return false;
  noInterrupts();
  mBuffer[writeIndex()] = inElement;
  mSize++;
  interrupts();
  return true;
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::lockedPush(const rg_element_t * const inElement)
{
  if (isFull()) return false;
  noInterrupts();
  mBuffer[writeIndex()] = *inElement;
  mSize++;
  interrupts();
  return true;
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::pop(rg_element_t &outElement)
{
  if (isEmpty()) return false;
  outElement = mBuffer[mReadIndex];
  mReadIndex++;
  if (mReadIndex >= maxSize) mReadIndex = 0;
  return true;
}

template <typename rg_element_t, uint8_t maxSize>
bool RingBuffer<rg_element_t, maxSize>::lockedPop(rg_element_t &outElement)
{
  if (isEmpty()) return false;
  noInterrupts();
  outElement = mBuffer[mReadIndex];
  mReadIndex++;
  if (mReadIndex >= maxSize) mReadIndex = 0;
  interrupts();
  return true;
}

#endif /* __RINGBUFFER_H__ */
