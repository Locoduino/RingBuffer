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

#endif /* __RINGBUFFER_H__ */
