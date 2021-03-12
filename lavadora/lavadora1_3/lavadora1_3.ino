#include "Nextion.h"

int char1,char2,char3;
int charflag = 0;
int ledPin = 13;
int charflag2 = 0;

void setup() {
	nexInit();
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
}

void loop() {
	if(Serial.available()>1){
		if(charflag){
			char2 = Serial.read();
			if(char2 == 0x07){
				char3 = Serial.read();
				if(char3 == 0x0D){
					digitalWrite(ledPin, HIGH);
				}else if(char3 == 0x0E){
					digitalWrite(ledPin, LOW);
				}
			}	
		}else{
			char1 = Serial.read();
			if(char1 == 0x65) charflag = 1;
		}
	}
}
