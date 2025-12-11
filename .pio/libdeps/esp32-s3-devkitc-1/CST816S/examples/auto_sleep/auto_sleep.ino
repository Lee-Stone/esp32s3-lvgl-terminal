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

CST816S touch(21, 22, 5, 4);  // sda, scl, rst, irq

void setup() {
  Serial.begin(115200);
  
  touch.begin();

  // Print version information
  Serial.print("Touch Firmware Version: ");
  Serial.print(touch.data.version);
  Serial.print("\t");
  Serial.print(touch.data.versionInfo[0]);
  Serial.print("-");
  Serial.print(touch.data.versionInfo[1]);
  Serial.print("-");
  Serial.println(touch.data.versionInfo[2]);

  // Disable auto sleep to keep the device active (useful during debugging or testing)
  touch.disable_auto_sleep();
  Serial.println("Auto sleep disabled");

  // Optionally, set a custom auto sleep time (e.g., 10 seconds)
  touch.set_auto_sleep_time(10);
  Serial.println("Auto sleep timeout set to 10 seconds");

  // Setting the auto sleep time does not automatically reenable auto sleep
  touch.enable_auto_sleep();
}

void loop() {
  if (touch.available()) {
    Serial.print("Gesture: ");
    Serial.print(touch.gesture());
    Serial.print("\tPoints: ");
    Serial.print(touch.data.points);
    Serial.print("\tEvent: ");
    Serial.print(touch.data.event);
    Serial.print("\tX: ");
    Serial.print(touch.data.x);
    Serial.print("\tY: ");
    Serial.println(touch.data.y);
  }
}