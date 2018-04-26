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

template <typename rg_element_t, uint8_t __maxSize__>
class RingBuffer
{
private:
  rg_element_t mBuffer[__maxSize__];
  uint8_t mReadIndex;
  uint8_t mSize;

  uint8_t writeIndex();

public:
  RingBuffer();
  bool push(const rg_element_t inElement) __attribute__ ((noinline));
  bool push(const rg_element_t * const inElement) __attribute__ ((noinline));
  bool lockedPush(const rg_element_t inElement);
  bool lockedPush(const rg_element_t * const inElement);
  bool pop(rg_element_t &outElement) __attribute__ ((noinline));
  bool lockedPop(rg_element_t &outElement);
  bool isFull()  { return mSize == __maxSize__; }
  bool isEmpty() { return mSize == 0; }
  void clear()   { mSize = 0; }
  uint8_t size() { return mSize; }
  uint8_t maxSize() { return __maxSize__; }
  rg_element_t &operator[](uint8_t inIndex);
};

template <typename rg_element_t, uint8_t __maxSize__>
uint8_t RingBuffer<rg_element_t, __maxSize__>::writeIndex()
{
 uint16_t wi = (uint16_t)mReadIndex + (uint16_t)mSize;
 if (wi >= (uint16_t)__maxSize__) wi -= (uint16_t)__maxSize__;
 return (uint8_t)wi;
}

template <typename rg_element_t, uint8_t __maxSize__>
RingBuffer<rg_element_t, __maxSize__>::RingBuffer() :
mReadIndex(0),
mSize(0)
{
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::push(const rg_element_t inElement)
{
  if (isFull()) return false;
  mBuffer[writeIndex()] = inElement;
  mSize++;
  return true;
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::push(const rg_element_t * const inElement)
{
  if (isFull()) return false;
  mBuffer[writeIndex()] = *inElement;
  mSize++;
  return true;
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::lockedPush(const rg_element_t inElement)
{
  noInterrupts();
  bool result = push(inElement);
  interrupts();
  return result;
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::lockedPush(const rg_element_t * const inElement)
{
  noInterrupts();
  bool result = push(inElement);
  interrupts();
  return result;
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::pop(rg_element_t &outElement)
{
  if (isEmpty()) return false;
  outElement = mBuffer[mReadIndex];
  mReadIndex++;
  mSize--;
  if (mReadIndex == __maxSize__) mReadIndex = 0;
  return true;
}

template <typename rg_element_t, uint8_t __maxSize__>
bool RingBuffer<rg_element_t, __maxSize__>::lockedPop(rg_element_t &outElement)
{
  noInterrupts();
  bool result = pop(outElement);
  interrupts();
  return result;
}

template <typename rg_element_t, uint8_t __maxSize__>
rg_element_t &RingBuffer<rg_element_t, __maxSize__>::operator[](uint8_t inIndex)
{
  if (inIndex >= mSize) return mBuffer[0];
  uint16_t index = (uint16_t)mReadIndex + (uint16_t)inIndex;
  if (index >= (uint16_t)__maxSize__) index -= (uint16_t)__maxSize__;
  return mBuffer[(uint8_t)index];
}

#endif /* __RINGBUFFER_H__ */
