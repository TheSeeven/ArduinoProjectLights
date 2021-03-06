#include <IRremote.h>

#define DEBUG false

IRsend irsend;

struct DataPack {
	unsigned int PC_LIGHT = 0;
	unsigned int LIGHT = 0;
	unsigned int AMBIENTAL = 0;

	DataPack(unsigned int PC_LIGHT, unsigned int LIGHT, unsigned int AMBIENTAL) {
		this->PC_LIGHT = PC_LIGHT;
		this->LIGHT = LIGHT;
		this->AMBIENTAL = AMBIENTAL;
	}

	DataPack() {
		this->PC_LIGHT = 0;
		this->LIGHT = 0;
		this->AMBIENTAL = 0;
	}

	~DataPack() {}

	void normalize() {
		this.AMBIENTAL = this.AMBIENTAL / 4;
		this.LIGHT = this.LIGHT / 4;
		this.PC_LIGHT = this.PC_LIGHT / 4;
	}

	void write() {
		PC_LIGHT = this.PC_LIGHT;
		LIGHT = this.LIGHT;
		AMBIENTAL = this.AMBIENTAL;
	}
}


unsigned int PC_LIGHT = 0;
unsigned int LIGHT = 0;
unsigned int AMBIENTAL = 0;

DataPack DATA_SAMPLES[] = new DataPack[4];
int DATA_SAMPLES_COUNTER = 0;

void addSample(DataPack data) {
	DATA_SAMPLES[DATA_SAMPLES_COUNTER] = data;
	DATA_SAMPLES_COUNTER++;
}

void setAverageData() {
	DataPack result = new DataPack();
	for (int i = 0; i < sizeof(DATA_SAMPLES) / sizeof(DataPack); i++) {
		result->AMBIENTAL += DATA_SAMPLES[i]->AMBIENTAL;
		result->LIGHT += DATA_SAMPLES[i]->LIGHT;
		result->PC_LIGHT += DATA_SAMPLES[i]->PC_LIGHT;
		DATA_SAMPLES[i].~DataPack();
	}
	result.normalize();
	result.write();
	DATA_SAMPLES_COUNTER = 0;
	result.~DataPack();
}


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
	for (i = 0; i < 4;i++)
	{
		float LIGHT_VOUT = float(analogRead(0)) * (5 / float(1023));
		unsigned int temp_LIGHT = 500 / (((9620 * (5 - LIGHT_VOUT)) / LIGHT_VOUT) / 1000);

		float PC_LIGHT_VOUT = float(analogRead(1)) * (5/float(1023));
		unsigned int temp_PC_LIGHT = 500 / (((10400 * (5 - PC_LIGHT_VOUT)) / PC_LIGHT_VOUT) / 1000);

		float AMBIENTAL_VOUT = float(analogRead(2)) * (5/float(1023));
		unsigned int temp_AMBIENTAL = 500 / (((10000 * (5 - AMBIENTAL_VOUT)) / AMBIENTAL_VOUT) / 1000);
		addSample(new DataPack(temp_PC_LIGHT, temp_LIGHT, temp_AMBIENTAL))
		delay(275);
	}
	setAverageData();
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

void goodMorning() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF18E7, 32);
		delay(50);
	}
	delay(4000);
}

void setDefaultProfile() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FF708F, 32);
		delay(50);
	}
}

void setBlue() {
	for(int i = 0; i<3 ; i++) {
		irsend.sendNEC(0x00FFA25D, 32);
		delay(50);
	}
	delay(2000);
}

void setMaxRed() {
	for(int i = 0; i<3; i++) {
		irsend.sendNEC(0x00FF1AE5, 32);
		delay(15);
	}
	for(int i = 0; i<9; i++) {
		irsend.sendNEC(0x00FF3AC5, 32);
		delay(10);
	}
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
			setMaxRed();
			delay(50);
			setDefaultProfile();
		}
		else if (checkPcLight() & checkLights() & checkAmbientalLight()) {
			setBlue();
			setLightOff();
		}
		delay(7000);
	
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