//Lock Blue Laser Diode
//LabView measures Laser diode wavelength and calculates feedback to lock it.
//LabView send 12-bit value to Nano to set DAC via serial comm.
//Nano provides output voltage to lock the laser.
#include <arduino.h>
uint32_t baudrate = 115200;
#include "SerialChecker.h"
SerialChecker sc;
int serialLength;
#include "MCP4822_SPI.h" /////////////
const int cs = 8; ////////////////////
MCP4822 dac(cs); /////////////////////

float T, prevT;
int16_t j,k;
int32_t Ai;
long t0;


int16_t DacInput = 0;
void setup() {
	Serial.begin(baudrate);
	sc.init();
    sc.enableACKNAK('%', '*');
    sc.enableSTX(false, '£');

    //pinMode(6,OUTPUT);
    //digitalWrite(6,LOW);
    //Serial.println("NanoLockBlueLD");
    
    dac.begin();
    dac.setVoltage(0,0);		//Temperature
    dac.setVoltage(1,4000);		//Current
}

void loop() {
    t0 = micros();
  	//CHECK SERIAL
	serialLength = sc.check();
    if(serialLength){
        if(sc.contains("t")){ // t for temperature control
        	//DacInput = sc.toInt16();
        	dac.setVoltage(0,sc.toInt16());
        }
        else if(sc.contains("c")){ // c for current control
        	dac.setVoltage(1,sc.toInt16());
        }
        else if(sc.contains("ID")){ // 
            Serial.println("BLD");
        }
        else if(sc.contains("GT")){ // 
            Serial.println(T);
        }
    }

    Ai = 0;
    for (k = 0; k<16; k++) 
    {
        for (j = 0; j < 8; j++) {
            Ai = Ai + analogRead(j);
        }
    }

    Ai = (Ai>>7); //Average temperature
    T = 50.0*float(Ai)/1023.0; //Convert to degree celcius
	//Serial.print(micros()-t0);
    //Serial.print(" ");
    //Serial.println(T);	
    prevT = T;
}
