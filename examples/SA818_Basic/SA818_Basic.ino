/*
    DRA818_Basic - Basic example of how to use the DRA818 Configuration Library

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a full copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.
*/

#include "SA818.h"

//
// IO Definitions
//
#define PIN_AMP     3  // Direct control of output PA (need hardware mod)
#define PIN_HL      4  // High/Low power pin (Input - HIGH, OutputLow - Low)
#define PIN_MIC     5  // DAC pin for tone out
#define PIN_PTT     7  // PTT pin. This is active low.
#define PIN_PD      8  // Power Down pin. This need to start low, then be set high before programming.

//Comment to use hardware Serial instead
#include <SomeSerial.h>
#define SOFTSERIAL 1

#ifdef SOFTSERIAL
#define SA_RXD 9
#define SA_TXD 6
SomeSerial serial(SA_RXD, SA_TXD);
#else
SomeSerial serial(&Serial);
#endif

// Instantiate the DRA818 Control Object, using the SoftwareSerial interface.
SA818 radio(&serial, PIN_PTT, PIN_PD, PIN_HL, PIN_AMP);

void setup()
{
  // Initialise Serial interface.
#ifdef SOFTSERIAL
  Serial.begin(115200);
  Serial.println("Init");
#endif  
  serial.begin(9600);
  radio.begin();
  radio.setConfig(SA_BANDWIDTH_12_5KHZ, "144.3500", "144.3500", SA_CTCSS_OFF, SA_CTCSS_OFF, SA_SQUELCH_OFF);
  radio.setPower(SA_POWER_LOW);
  //radio.setVolume(SA_VOLUME_DEFAULT);
  //radio.setFilters(SA_FILTER_OFF, SA_FILTER_OFF, SA_FILTER_OFF);
  pinMode(PIN_AMP, OUTPUT);


}

void loop() 
{
  // Key the PTT on and off repeatedly.
  radio.setModeTX();
  delay(3000);

  radio.setModeRX();
  delay(3000);
}
