/*
    SA818.h - SA818U/SA818V Communication Library.

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

#ifndef SA818_h
#define SA818_h

#include <SomeSerial.h>
#include <Stream.h>

#define SA_BANDWIDTH_12_5KHZ	0
#define SA_BANDWIDTH_25KHZ		1

#define SA_CTCSS_OFF	"0000"
#define SA_SQUELCH_OFF	0

#define SA_POWER_LOW	0
#define SA_POWER_HIGH	1

#define SA_VOLUME_MIN	0
#define SA_VOLUME_DEFAULT	4
#define SA_VOLUME_MAX	8

#define SA_FILTER_OFF	0
#define SA_FILTER_ON	1

#define SA_POWER_OFF	0
#define SA_POWER_ON		1

#define SA_MODE_TX		0
#define SA_MODE_RX		1

class SA818 {
    public:
        SA818(SomeSerial *serial, uint8_t PIN_PTT, uint8_t PIN_PD, uint8_t PIN_HL, uint8_t PIN_AMP = 0);

        uint8_t begin();
        uint8_t setConfig(uint8_t bw, char* tx_f, char* rx_f, char* tx_ctcss, char* rx_ctcss, uint8_t squelch);
        uint8_t setVolume(uint8_t volume);
        uint8_t setPower(uint8_t is_high);
		uint8_t setFilters(boolean preemph, boolean highpass, boolean lowpass);

		void powerDown(uint8_t powerdown);
		void changeMode(uint8_t mode);

		void setModeTX();
		void setModeRX();

    private:
        //Stream *serial;
        //NewSoftSerial *serial;
        SomeSerial *serial;
	
        uint8_t PTT_PIN, PD_PIN, HL_PIN, AMP_PIN;

		uint8_t readSerialTimeout();

        //char buffer[60];
};
#endif