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

//#define DEBUG 1

SA818::SA818(SomeSerial *serial, uint8_t PIN_PTT, uint8_t PIN_PD, uint8_t PIN_HL, uint8_t PIN_AMP = 0)
{
	this->serial = serial;
	//this->serial->begin(9600);

	this->PTT_PIN = PIN_PTT;
	this->PD_PIN = PIN_PD;
	this->HL_PIN = PIN_HL;
	this->AMP_PIN = PIN_AMP;

	pinMode(this->PTT_PIN, OUTPUT);
	pinMode(this->PD_PIN, OUTPUT);
	pinMode(this->HL_PIN, OUTPUT);
	if(this->AMP_PIN){
		pinMode(this->AMP_PIN, OUTPUT);
		digitalWrite(this->AMP_PIN, LOW);
	}

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
			Serial.write(this->serial->read());
#else
			this->serial->read();
#endif			
			readed = true;
		}
		if (readed == true)break;
	}
#ifdef DEBUG			
	if(!readed){
		Serial.println("Read timeout");
	}
#endif			
	return readed;
}

uint8_t SA818::begin()
{
	//not a commmand, but for sure
	this->serial->print("AT\r\n");
	delay(500);

	for(uint8_t r = 0; r<5; r++){
		this->serial->print("AT+DMOCONNECT\r\n");
		if(this->readSerialTimeout()){
			//got +DMOCONNECT:0<CR><LF>
			return true;
		}
	}
	return false;
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

	for(uint8_t r=0; r<5; r++){
		this->serial->print("AT+DMOSETGROUP=");
		this->serial->print(bw); // 0/1
		this->serial->print(",");
		this->serial->print(tx_f);//134-174/400-480, format to 415.1250
		this->serial->print(",");
		this->serial->print(rx_f);//format to 415.1250
		this->serial->print(",");
		this->serial->print(tx_ctcss);//format to 0000
		this->serial->print(",");
		this->serial->print(squelch); // <= 8
		this->serial->print(",");
		this->serial->print(rx_ctcss);
		this->serial->print("\r\n");

		if(this->readSerialTimeout()){
			return true;
		}
	}

	return false;
}

uint8_t SA818::setVolume(uint8_t volume)
{
	// Set PTT off, so we can communicate with the uC.
	// Delay for a bit, to finish TX
	digitalWrite(this->PTT_PIN, HIGH); 
	delay(100);

	if(volume>8) volume = 8;

	//sprintf(this->buffer,"AT+DMOSETVOLUME=%1d\r\n",this->volume);
	this->serial->print("AT+DMOSETVOLUME=");
	this->serial->print(volume);
	this->serial->print("\r\n");

	return this->readSerialTimeout();
}

uint8_t SA818::setFilters(boolean preemph, boolean highpass, boolean lowpass)
{
	// Set PTT off, so we can communicate with the uC.
	// Delay for a bit, to finish TX
	digitalWrite(this->PTT_PIN, HIGH); 
	delay(100);

	//sprintf(this->buffer,"AT+SETFILTER=%1d,%1d,%1d\r\n",this->preemph,this->highpass,this->lowpass);
	//this->serial->print(this->buffer);

	this->serial->print("AT+SETFILTER=");
	this->serial->print(preemph);
	this->serial->print(",");
	this->serial->print(highpass);
	this->serial->print(",");
	this->serial->print(lowpass);
	this->serial->print("\r\n");


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
	if(this->AMP_PIN){
		digitalWrite(this->AMP_PIN, !mode);
	}
}

void SA818::setModeTX(){
	this->changeMode(SA_MODE_TX);
}

void SA818::setModeRX(){
	this->changeMode(SA_MODE_RX);
}
