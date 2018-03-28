/*
	SA818.cpp - SA818U/SA818V Communication Library.

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

#include "Arduino.h"
#include "SA818.h"
#include <stdio.h>
#include <Stream.h>
#include <avr/wdt.h>


SA818::SA818(Stream *serial, uint8_t PIN_PTT, uint8_t PIN_PD, uint8_t PIN_HL)
{
	this->serial = serial;
	//this->serial->begin(9600);

	this->PTT_PIN = PIN_PTT;
	this->PD_PIN = PIN_PD;
	this->HL_PIN = PIN_HL;

	pinMode(this->PTT_PIN, OUTPUT);
	pinMode(this->PD_PIN, OUTPUT);
	pinMode(this->HL_PIN, OUTPUT);

	digitalWrite(this->PTT_PIN, HIGH);
	digitalWrite(this->PD_PIN, HIGH);
	digitalWrite(this->HL_PIN, LOW);
}

uint8_t SA818::readSerialTimeout()
{
	uint8_t readed = false;
	unsigned long timeout = millis();
	while (millis() - timeout < 500) {
		delay(100);
		wdt_reset();
		while (this->serial->available() > 0) {
#ifdef DEBUG			
			if(this->serial != Serial){
				Serial.write(this->serial->read());
			}else{
				this->serial->read();
			}
#else
			this->serial->read();
#endif			
			readed = true;
		}
		if (readed == true)break;
	}
	return readed;
}

uint8_t SA818::begin()
{
	//not a commmand, but for sure
	this->serial->write("AT\r\n");
	delay(100);

	this->serial->write("AT+DMOCONNECT\r\n");

	//got +DMOCONNECT:0<CR><LF>
	return this->readSerialTimeout();
}

uint8_t SA818::setPower(uint8_t is_high)
{
	//HL to ground - low power, HL float - high power
	if(is_high){
		pinMode(this->HL_PIN, INPUT);
	}else{
		pinMode(this->HL_PIN, OUTPUT);
	}
	digitalWrite(this->HL_PIN, LOW); //for sure
}

uint8_t SA818::setConfig(uint8_t bw, char* tx_f, char* rx_f, char* tx_ctcss, char* rx_ctcss, uint8_t squelch)
{
	// Set PTT off, so we can communicate with the uC.
	// Delay for a bit, to finish TX
	digitalWrite(this->PTT_PIN, HIGH);
	delay(100); 

	//char freq_buffer[10];
	//dtostrf(this->tx_freq,8,4,freq_buffer);
	//sprintf(this->buffer,"AT+DMOSETGROUP=0,%s,%s,%d,%1d,%d,1\r\n",freq_buffer,freq_buffer,this->tx_ctcss,this->squelch,this->rx_ctcss);
	//this->serial->write(this->buffer);
	this->serial->write("AT+DMOSETGROUP=");
	this->serial->write(bw); // 0/1
	this->serial->write(",");
	this->serial->write(tx_f);//134-174/400-480, format to 415.1250
	this->serial->write(",");
	this->serial->write(rx_f);//format to 415.1250
	this->serial->write(",");
	this->serial->write(tx_ctcss);//format to 0000
	this->serial->write(",");
	this->serial->write(squelch); // <= 8
	this->serial->write(",");
	this->serial->write(rx_ctcss);
	this->serial->write("\r\n");

//AT+DMOSETGROUP=0,415.1250,415.1250,0012,4, 0013 

  	return this->readSerialTimeout();
}

uint8_t SA818::setVolume(uint8_t volume)
{
	// Set PTT off, so we can communicate with the uC.
	// Delay for a bit, to finish TX
	digitalWrite(this->PTT_PIN, HIGH); 
	delay(100);

	if(volume>8) volume = 8;

	//sprintf(this->buffer,"AT+DMOSETVOLUME=%1d\r\n",this->volume);
	this->serial->write("AT+DMOSETVOLUME=");
	this->serial->write(volume);
	this->serial->write("\r\n");

	return this->readSerialTimeout();
}

uint8_t SA818::setFilters(boolean preemph, boolean highpass, boolean lowpass)
{
	// Set PTT off, so we can communicate with the uC.
	// Delay for a bit, to finish TX
	digitalWrite(this->PTT_PIN, HIGH); 
	delay(100);

	//sprintf(this->buffer,"AT+SETFILTER=%1d,%1d,%1d\r\n",this->preemph,this->highpass,this->lowpass);
	//this->serial->write(this->buffer);

	this->serial->write("AT+SETFILTER=");
	this->serial->write(preemph);
	this->serial->write(",");
	this->serial->write(highpass);
	this->serial->write(",");
	this->serial->write(lowpass);
	this->serial->write("\r\n");


	//return true; //must be +DMOSETFILTER: X (X=0->ok, X=1-fail)
	return this->readSerialTimeout();
}

void SA818::powerDown(uint8_t powerdown)
{
	digitalWrite(this->PTT_PIN, HIGH);
	digitalWrite(this->PD_PIN, powerdown);
}

void SA818::changeMode(uint8_t mode)
{
	digitalWrite(this->PTT_PIN, mode);
}

void SA818::setModeTX(){
	this->changeMode(SA_MODE_TX);
}

void SA818::setModeRX(){
	this->changeMode(SA_MODE_RX);
}
