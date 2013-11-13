Remote Control for Arduino Yún
======================

This repo contains source code for the Arduino Yún project shown in this [Temboo demo video](https://vimeo.com/79331781
). The video demonstrates how you can remotely control an 
Arduino Yún from your mobile phone. The project uses a light dependent resistor (photocell) to read the light level
in a room. When the Yún senses that the light level has changed by 30% or more, it calls the user and asks if they'd like to turn on the LED connected to pin 13. It also logs each significant light change event to a Google Spreadsheet. All of this is achieved by the Temboo SDK that ships onboard the Yún. 

##Hardware Overview

###Requirements 

1 x light dependent resistor (photocell), like [this one](https://www.sparkfun.com/products/9088)  

1 x 10k Ohm resistor  

1 or more LEDs, we used [these ones](https://www.sparkfun.com/products/531)


###Instructions

Connecting an LED to an Arduino board [[instructions]](http://learn.adafruit.com/photocells/using-a-photocell)

Connecting a light dependent resistor to an Arduino board - [[instructions]](http://arduino.cc/en/Tutorial/Blink?from=Tutorial.BlinkingLED) 

##Get Set Up

As well as your Temboo account, you'll need a Google account and a Nexmo account to run this sketch successfully. Over on our website you can find tutorials that deal with Google Spreadsheets and Nexmo separately, and they contain all the info you need to get set up with these services. 

- [Temboo's Nexmo tutorial](https://www.temboo.com/arduino/remote-control)
- [Temboo's Google Spreadsheets tutorial](https://www.temboo.com/arduino/update-google-spreadsheet) 

##Running The Sketch

To run the sketch: 

1. Replace the placeholder values in `TembooAccount.h` with your Temboo account name, app key name, and app key value.

2. Replace the placeholders in `RemoteControl.ino` with your Google and Nexmo account information.

The placeholders in both files are commented, so it should be easy to find where everything should go.

When you upload the sketch to your Yún, make sure to open the serial monitor so that you can see the output. To simulate changes in light, we find that placing a hand over the light sensor works just fine. 

##Questions? 

If you have any questions about this tutorial, or anything to do with Temboo, then please don't hesitate to get in touch with us any time - <support@temboo.com>

###Copyright and License
Copyright 2013, Temboo Inc.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.











