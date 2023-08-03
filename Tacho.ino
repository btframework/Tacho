/**
 * Simple tachometer
 * Copyrigth (C) Mike Petrichenko
 * mike@btframework.com
 * 
 * Arduino nano (Atmega 168P).
*/

// Digit segment pins.
// NOTE: Segment A uses A0 Arduino PIN. It is because
// we need PIN D2 for interrupt.
#define SEG_A	14
#define SEG_B   3
#define SEG_C   4
#define SEG_D	5
#define SEG_E	6
#define SEG_F	7
#define SEG_G	8

// Point segment pin.
#define SEG_P	9

// Digit position selection pins.
#define POS_1	10
#define POS_2	11
#define POS_3	12
#define POS_4	13

// Segment levels.
// NOTE: My indicator had reversed levels!
#define SEG_ON		LOW
#define SEG_OFF		HIGH

// Digit selection levels.
// NOTE: My indicator has reversed levels!
#define POS_ON		HIGH
#define POS_OFF		LOW

// Hall pin number
#define	SENSE_PIN	2
#define SENSE_INT	0

// Some RPM constants.
// Maximum displayable RPM value.
#define MAX_RPM		9999
// Used to identify the high RPM value
#define HIGH_RPM	10000
// Indicates no RPM data.
#define NO_RPM		65535
// Simple zero RPM value.
#define ZERO_RPM	0

// Delays.
// Delay used after degit position selected.
#define POSITION_DELAY	0
// Delay used after digit displayed.
#define DIGIT_DELAY		2
// Delay used during "test".
#define TEST_DELAY		200

// Delays used during RPM calculation.
// The RPM data timeout.
#define RPM_PING_DELAY	5000
// The RPM reset (zero) timeout.
#define ZERO_RPM_DELAY	(RPM_PING_DELAY + 2000)
// Indication udpates rate.
#define UPDATE_RATE		1000

/* Delay helpers. */

void delayPosition()
{
	if (POSITION_DELAY > 0)
		delay(POSITION_DELAY);
}

void delayDigit()
{
	if (DIGIT_DELAY > 0)
		delay(DIGIT_DELAY);
}

void delayTest()
{
	if (TEST_DELAY > 0)
		delay(TEST_DELAY);
}

/* Clearing helpers. */

void clearPositions()
{
	digitalWrite(POS_1, POS_OFF);
	digitalWrite(POS_2, POS_OFF);
	digitalWrite(POS_3, POS_OFF);
	digitalWrite(POS_4, POS_OFF);
}

void clearDigit()
{
	digitalWrite(SEG_A, SEG_OFF);
	digitalWrite(SEG_B, SEG_OFF);
	digitalWrite(SEG_C, SEG_OFF);
	digitalWrite(SEG_D, SEG_OFF);
	digitalWrite(SEG_E, SEG_OFF);
	digitalWrite(SEG_F, SEG_OFF);
	digitalWrite(SEG_G, SEG_OFF);

	digitalWrite(SEG_P, SEG_OFF);
}

/* Display helpers. */

void selectPosition(byte pos)
{
	// Clear current selected position.
	clearDigit();
	// Turns all positions off.
	clearPositions();

	// And only now switch to requested one.
	switch (pos)
  {
		case 1:
			digitalWrite(POS_1, POS_ON);
			break;
		
		case 2:
			digitalWrite(POS_2, POS_ON);
			break;
		
		case 3:
			digitalWrite(POS_3, POS_ON);
			break;
		
		case 4:
			digitalWrite(POS_4, POS_ON);
			break;
	}

	delayPosition();
}

void togglePoint(bool show)
{
	digitalWrite(SEG_P, show ? SEG_ON : SEG_OFF);
	delayDigit();
}

void displayDigit(byte pos, byte dig, bool point)
{
	// Select digit position first.
	selectPosition(pos);

	// Now display the digit.
	switch (dig)
  {
		case 0:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_E, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			break;
		
		case 1:
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			break;
		
		case 2:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_E, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 3:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 4:
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 5:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 6:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_E, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 7:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			break;
		
		case 8:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_E, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
		
		case 9:
			digitalWrite(SEG_A, SEG_ON);
			digitalWrite(SEG_B, SEG_ON);
			digitalWrite(SEG_C, SEG_ON);
			digitalWrite(SEG_D, SEG_ON);
			digitalWrite(SEG_F, SEG_ON);
			digitalWrite(SEG_G, SEG_ON);
			break;
	}
	
	togglePoint(point);
}

void displaySign(byte pos)
{
	selectPosition(pos);
	digitalWrite(SEG_G, SEG_ON);
	delayDigit();
}

/* Latters. */

void displayH(byte pos)
{
	selectPosition(pos);

	digitalWrite(SEG_B, SEG_ON);
	digitalWrite(SEG_C, SEG_ON);
	digitalWrite(SEG_E, SEG_ON);
	digitalWrite(SEG_F, SEG_ON);
	digitalWrite(SEG_G, SEG_ON);
	
	togglePoint(false);
}

void displayI(byte pos)
{
	selectPosition(pos);

	digitalWrite(SEG_B, SEG_ON);
	digitalWrite(SEG_C, SEG_ON);

	togglePoint(false);
}

void displayG(byte pos)
{
	selectPosition(pos);

	digitalWrite(SEG_A, SEG_ON);
	digitalWrite(SEG_C, SEG_ON);
	digitalWrite(SEG_D, SEG_ON);
	digitalWrite(SEG_E, SEG_ON);
	digitalWrite(SEG_F, SEG_ON);

	togglePoint(false);
}

void displayHigh()
{
	displayH(4);
	displayI(3);
	displayG(2);
	displayH(1);
}

/* Setup */

void setupPositionPins()
{
	pinMode(POS_1, OUTPUT);
	pinMode(POS_2, OUTPUT);
	pinMode(POS_3, OUTPUT);
	pinMode(POS_4, OUTPUT);

	clearPositions();
}

void setupDigitPins()
{
	pinMode(SEG_A, OUTPUT);
	pinMode(SEG_B, OUTPUT);
	pinMode(SEG_C, OUTPUT);
	pinMode(SEG_D, OUTPUT);
	pinMode(SEG_E, OUTPUT);
	pinMode(SEG_F, OUTPUT);
	pinMode(SEG_G, OUTPUT);
	pinMode(SEG_P, OUTPUT);

	clearDigit();
}

void setupSensorPins()
{
	pinMode(SENSE_PIN, INPUT);

	attachInterrupt(SENSE_INT, intCount, RISING);
}

void testDisplay()
{
	// Signs
	for (byte pos = 1; pos < 5; pos++)
  {
		displaySign(pos);
		delayTest();
	}
	for (byte pos = 3; pos > 0; pos--)
  {
		displaySign(pos);
		delayTest();
	}

	// Points
	for (byte pos = 1; pos < 5; pos++)
  {
		selectPosition(pos);
		togglePoint(true);
		delayTest();
	}
	for (byte pos = 3; pos > 0; pos--)
  {
		selectPosition(pos);
		togglePoint(true);
		delayTest();
	}

	// Show all eights
	for (byte pos = 1; pos < 5; pos++)
  {
		displayDigit(pos, 8, false);
		delayTest();
	}
	for (byte pos = 3; pos > 0; pos--)
  {
		displayDigit(pos, 8, false);
		delayTest();
	}
}

void setup()
{
	Serial.begin(115200);

  setupPositionPins();
	setupDigitPins();

	testDisplay();
	
	setupSensorPins();
}

/* Main program. */

void displayRPM(unsigned short rpm)
{
	switch (rpm)
  {
		case ZERO_RPM:
			displayDigit(1, 0, false);
			break;

		case HIGH_RPM:
			displayHigh();
			break;;

		case NO_RPM:
			for (byte pos = 1; pos < 5; pos++)
				displaySign(pos);
			break;
			
		default:
			byte pos = 1;
			while (rpm > 0)
			{
				byte dig = rpm % 10;
				rpm = rpm / 10;
				displayDigit(pos, dig, false);
				pos++;
			}
			break;
	}
}

// RPM detector indicator.
volatile bool RpmDetected = false;
// RPM ping indicator.
volatile bool RpmPing = false;
// Current measure RPM.
volatile unsigned int CurrentRpm = 0;

void loop()
{
	// Ticks when RPM data was last received.
	static unsigned long lastRpmPing;
	// Ticks when RPM was sets to zero.
	static unsigned long lastZeroRpm;
	// Ticks when RPM was updated.
	static unsigned long lastUpdate;
	// Indicates that zero RPM was detected.
	static bool zeroRpmDetected = false;
	// Last RPM value.
	static unsigned short lastRpm = NO_RPM;
	
	// Current RPM value.
	unsigned short rpm;

	if (!RpmDetected)
		rpm = NO_RPM;
	else
  {
		if (RpmPing)
    {
			RpmPing = false;
			lastRpmPing = millis();
			zeroRpmDetected = false;
		}
    else
    {
			if (CurrentRpm == 0)
      {
				if (!zeroRpmDetected)
        {
					zeroRpmDetected = true;
					lastZeroRpm = millis();
				}
        else
        {
					if (millis() - lastZeroRpm >= ZERO_RPM_DELAY)
						RpmDetected = false;
				}
			}
      else
      {
				if (millis() - lastRpmPing > RPM_PING_DELAY)
					CurrentRpm = 0;
			}
		}

		if (CurrentRpm > MAX_RPM)
			rpm = HIGH_RPM;
		else
			rpm = CurrentRpm;
	}

	if (millis() - lastUpdate < UPDATE_RATE)
		rpm = lastRpm;
	else
  {
		lastRpm = rpm;
		lastUpdate = millis();
	}

	displayRPM(rpm);
}

/* Sensor interrupt. */

void intCount() {
	// Ticks (micro!) when RPM was detected first time.
	static unsigned long LastRpmTime;

	cli();
	if (!RpmDetected) {
		RpmDetected = true;
		LastRpmTime = micros();
	} else {
		CurrentRpm = 60 / ((float)(micros() - LastRpmTime) / 1000000);
		LastRpmTime = micros();
	}
	RpmPing = true;
	sei();
}
