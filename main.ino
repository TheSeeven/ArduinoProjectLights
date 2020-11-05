#include <IRremote.h>

#define DEBUG false

IRsend irsend;

unsigned int PC_LIGHT = 0;
unsigned int LIGHT = 0;
unsigned int AMBIENTAL = 0;

bool checkPcLight() {
	if(PC_LIGHT > 100) {
		return true;
	}
	return false;
}

bool checkAmbientalLight() {
	if(AMBIENTAL > 22) {
		return true;
	}
	return false;
}

bool checkLights() {
	if(LIGHT > 40) {
		return true;
	}
	return false;
}

void getData() {
	// int64_t sensorVal = analogRead(0);
	
	float LIGHT_VOUT = float(analogRead(0)) * (5/float(1023));
	LIGHT = 500/(((9620 * (5-LIGHT_VOUT))/LIGHT_VOUT)/1000);

	float PC_LIGHT_VOUT = float(analogRead(1)) * (5/float(1023));
	PC_LIGHT = 500/(((10400 * (5-PC_LIGHT_VOUT))/PC_LIGHT_VOUT)/1000);

	float AMBIENTAL_VOUT = float(analogRead(2)) * (5/float(1023));
	AMBIENTAL = 500/(((10000 * (5-AMBIENTAL_VOUT))/AMBIENTAL_VOUT)/1000);
}

void setLightOff() {
	while(checkLights()) {
		irsend.sendNEC(0x00FF02FD, 32);
		delay(100);
		getData();
		
	}
}

void setLightOn() {
	while(!checkLights()) {
		irsend.sendNEC(0x00FF02FD, 32);
		delay(100);
		getData();
		
	}
}

void setup() { 
	Serial.begin(9600); 
	Serial.println("Program Started!");
	getData();
} 

void goToSleep() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF1AE5, 32);
		delay(50);
	}
	for(int i=0; i<5; i++) {
		irsend.sendNEC(0x00FFBA45, 32);
		delay(1000);
	}
}

void setDefaultProfile() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF708F, 32);
		delay(50);
	}
}

void goodMorning() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF18E7, 32);
		delay(50);
	}
	delay(4000);
}

void setBlue() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FFA25D, 32);
		delay(50);
	}
	delay(2000);
}

void loop() { 

	getData();
	#if !DEBUG
		if(!checkPcLight() & checkLights() & !checkAmbientalLight()) {
			goToSleep();
			setLightOff();
		}
		else if (!checkPcLight() & checkLights() & checkAmbientalLight()) {
			goodMorning();
			setLightOff();
		}
		else if (checkPcLight() & !checkLights() & !checkAmbientalLight()) {
			setLightOn();
			delay(50);
			setDefaultProfile();
		}
		else if (checkPcLight() & checkLights() & checkAmbientalLight()) {
			setBlue();
			setLightOff();
		}
		delay(10000);
	
	#elif DEBUG
	
	Serial.print("PC Light: ");
	Serial.print(PC_LIGHT);
	Serial.print("   ");
	Serial.println(checkPcLight());
	
	Serial.print("Ambiental light: ");
	Serial.print(AMBIENTAL);
	Serial.print("   ");
	Serial.println(checkAmbientalLight());

	Serial.print("Light: ");
	Serial.print(LIGHT);
	Serial.print("   ");
	Serial.println(checkLights());

	delay(2000);

	#endif

}