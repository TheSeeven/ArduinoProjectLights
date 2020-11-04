#include <IRremote.h>

IRsend irsend;

int64_t PC_LIGHT = 0;
int64_t LIGHT = 0;
int64_t AMBIENTAL = 0;

#define debug false
#define ndebug true

bool checkPcLight() {
	return true;
}

bool checkAmbientalLight() {
	return true;
}

bool checkLights() {
	return true;
}



void getData() {

}

void setLightOff() {
	while(checkLights()) {
		irsend.sendNEC(0x00FF02FD, 32);
		getData()
		delay(50);
	}
}

void setLightOn() {
	while(!checkLights()) {
		irsend.sendNEC(0x00FF02FD, 32);
		getData()
		delay(50);
	}
}

void setup() { 
	Serial.begin(9600); 
	Serial.println("Program Started!");
	getData(){}
} 

void goToSleep() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF1AE5, 32);
		delay(50);
	}
	for(int i=0; i<5) {
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

	// pc off light off ambiental off   					0
	// pc off light off ambiental on	   					1
	// pc off light on ambiental off -> setLight off		1
	// pc off light on ambiental on -> setLight off			2
	// pc on light off ambintal off -> setLight on			1
	// pc on light off ambiental on							2
	// pc on light on ambiental off							2
	// pc on light on ambiental on -> setLight off			3
	#if debug
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
			setDefaultProfile();
		}
		else if (checkPcLight() & checkLights() & checkAmbientalLight()) {
			setBlue();
			setLightOff();
		}
		delay(60000);
		
	#endif
	getData()
}