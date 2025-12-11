/*
   MIT License

  Copyright (c) 2021 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <CST816S.h>

CST816S touch(21, 22, 5, 4);	// sda, scl, rst, irq

// Flag to indicate a touch interrupt has occurred
volatile bool touchEventFlag = false;

// Interrupt function to set the flag
void IRAM_ATTR onTouchInterrupt() {
  touchEventFlag = true;
}

void setup() {
  Serial.begin(9600);
  touch.begin();
  touch.attachUserInterrupt(onTouchInterrupt);
  touch.enable_double_click(); // Enable double-click detection
}

void loop() {
  // Check if the interrupt flag has been set by the interrupt handler
  if (touchEventFlag) {
    touchEventFlag = false; // Clear the flag

    // Check if there’s touch data available
    if (touch.available()) {
      // Check if the gesture is a double touch
      if (touch.gesture() == "DOUBLE CLICK") {
        Serial.println("Double touch detected!");
      } else {
        Serial.println("Touch event detected, but not a double touch.");
      }
    }
  }
}
