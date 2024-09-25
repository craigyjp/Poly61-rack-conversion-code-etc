#include <SPI.h>
#include <MIDI.h>

// Define chip select pins for each DAC
const int CS_DAC1 = 9;
const int CS_DAC2 = 8;

int bend = 2048;
int CC1 = 0;
int CC2 = 0;
int MIDI_CHANNEL = 1;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  // Initialize chip select pins as output
  pinMode(CS_DAC1, OUTPUT);
  pinMode(CS_DAC2, OUTPUT);
  
  // Set chip select pins to HIGH (inactive state)
  digitalWrite(CS_DAC1, HIGH);
  digitalWrite(CS_DAC2, HIGH);
  
  MIDI.begin(MIDI_CHANNEL);
  
  // Initialize DACs with 0 values
  setDAC(CS_DAC1, 0, 0); 
  setDAC(CS_DAC2, bend, 0);
}

void loop() {
  if (MIDI.read()) {
    byte type = MIDI.getType();
    int d1 = MIDI.getData1();
    int d2 = MIDI.getData2();

    switch (type) {
      case midi::PitchBend:
        handlePitchBend(d1, d2);
        break;

      case midi::ControlChange:
        handleControlChange(d1, d2);
        break;

      default:
        break;
    }
  }
}

void handlePitchBend(int d1, int d2) {
  // Combine LSB and MSB to form 14-bit pitch bend value
  int bendValue = (d2 << 7) | d1;

  // Centered value of pitch bend (8192 is no bend)
  int bendCenter = 8192;
  int potValue = analogRead(A0);  // Read A0 (potentiometer)
  
  // Map the potentiometer value (0 to 1023) to a scaling factor (0 to 100%)
  float bendScale = map(potValue, 0, 1023, 0, 100) / 100.0;
  bend = bendCenter + (bendValue - bendCenter) * bendScale;  // Scale around center

  // Map bend to DAC value range (0-4095)
  bend = map(bend, 0, 16383, 4095, 0);
  
  // Send to DAC2
  setDAC(CS_DAC2, bend, 0);
}

void handleControlChange(int d1, int d2) {
  switch (d1) {
    case 2:
      // CC range from 0 to 4095 mV, shift d2 by 4 to scale from 0 to 4095
      CC1 = d2 << 4;
      setDAC(CS_DAC1, CC1, CC2);
      break;

    case 3:
      // CC range from 0 to 4095 mV, shift d2 by 4 to scale from 0 to 4095
      CC2 = d2 << 4;
      setDAC(CS_DAC1, CC1, CC2);
      break;
  }
}

// Function to send data to the DAC
void setDAC(int CS, int valueA, int valueB) {
  // DAC A
  digitalWrite(CS, LOW); // Activate DAC
  SPI.transfer(0x30 | ((valueA >> 8) & 0x0F)); // Command byte for DAC A
  SPI.transfer(valueA & 0xFF);  // Data byte for DAC A
  digitalWrite(CS, HIGH); // Deactivate DAC

  // DAC B
  digitalWrite(CS, LOW); // Activate DAC
  SPI.transfer(0xB0 | ((valueB >> 8) & 0x0F)); // Command byte for DAC B
  SPI.transfer(valueB & 0xFF);  // Data byte for DAC B
  digitalWrite(CS, HIGH); // Deactivate DAC
}

