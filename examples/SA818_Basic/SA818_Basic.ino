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

#include <SoftwareSerial.h>
#include "SA818.h"

//
// IO Definitions
//
#define PIN_AMP     3  // Direct control of output PA
#define PIN_HL      4  // High/Low power pin (Input - HIGH, OutputLow - Low)
#define PIN_MIC     5  // DAC pin for tone out
#define PIN_PTT     7  // PTT pin. This is active low.
#define PIN_PD      8  // Power Down pin. This need to start low, then be set high before programming.

//Comment to use hardware Serial instead
#define SOFTSERIAL 1

#ifdef SOFTSERIAL
#include <SoftwareSerial.h>
#define SA_RXD 9
#define SA_TXD 6
SoftwareSerial serial(SA_RXD, SA_TXD);
#define SERIAL serial
#else
#define SERIAL Serial
#endif

// Instantiate the DRA818 Control Object, using the SoftwareSerial interface.
SA818 radio(&SERIAL, PIN_PTT, PIN_PD, PIN_HL);

void setup()
{
  // Initialise Serial interface.
  SERIAL.begin(9600);
  radio.begin();
  radio.setConfig(SA_BANDWIDTH_12_5KHZ, "145.4750", "145.4750", SA_CTCSS_OFF, SA_CTCSS_OFF, SA_SQUELCH_OFF);
  radio.setPower(SA_POWER_HIGH);
  //radio.setVolume(SA_VOLUME_DEFAULT);
  //radio.setFilters(SA_FILTER_OFF, SA_FILTER_OFF, SA_FILTER_OFF);


}

void loop() {
  // Key the PTT on and off repeatedly.
  radio.changeMode(SA_MODE_TX);
  delay(1000);
  radio.changeMode(SA_MODE_RX);
  delay(1000);
}