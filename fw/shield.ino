#define SNES_DATA 	4
#define SNES_CLOCK 	5
#define SNES_LATCH 	6

#define GB_A 		18
#define GB_B 		19
#define GB_UP		14
#define GB_DOWN	15
#define GB_LEFT	16
#define GB_RIGHT	17
#define GB_L		0
#define GB_R		1
#define GB_START	2
#define GB_SELECT	3

#define GB_PWR		7

bool shuttingOff = false;
int btn_pins[12] = {
	GB_B, 99, GB_SELECT, GB_START, GB_UP, GB_DOWN, GB_LEFT, GB_RIGHT, GB_A, 99, GB_L, GB_R
};

void setup() {
	pinMode(SNES_CLOCK, OUTPUT);
	digitalWrite(SNES_CLOCK, 1);

	pinMode(SNES_LATCH, OUTPUT);
	digitalWrite(SNES_LATCH, 1);
	
	pinMode(SNES_DATA, OUTPUT);
	pinMode(SNES_DATA, 1);
	pinMode(SNES_DATA, INPUT);

	for(int i=0; i<12; i++){
		if(btn_pins[i] == 99) continue;
		pinMode(btn_pins[i], OUTPUT);
		digitalWrite(btn_pins[i], 1);
	}

	pinMode(GB_PWR, OUTPUT);
	digitalWrite(GB_PWR, 1);
}



int getController(){
	digitalWrite(SNES_LATCH, 1);
	delayMicroseconds(12);
	digitalWrite(SNES_LATCH, 0);
	delayMicroseconds(6);
	int d = 0;
	for(int i=0; i<16; i++){
		if(i>=12) continue;
		digitalWrite(SNES_CLOCK, 0);
		delayMicroseconds(6);
		d |= (digitalRead(SNES_DATA) << i);
		digitalWrite(SNES_CLOCK, 1);
		delayMicroseconds(6);
	}

	// Map Y to A
	if(((d >> 0) & 1) && !((d >> 9) & 1)) d ^= 1 << 0;
	// Map X to B
	if(((d >> 8) & 1) && !((d >> 1) & 1)) d ^= 1 << 8;

	return d;
}



void loop() {
	int data = getController();
	for(int i=0; i<12; i++){
		if(btn_pins[i] == 99) continue;
		digitalWrite(btn_pins[i], (data >> i) & 1);
	}

	if(!((data >> 5) & 1) && !((data >> 2) & 1) && !shuttingOff && data<4095){
		shuttingOff = true;
		digitalWrite(GB_PWR, 0);
		delay(100);
		digitalWrite(GB_PWR, 1);
		shuttingOff = false;
	}
	delay(10);
}