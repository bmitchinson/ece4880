// A digital frequency selective filter
// A. Kruger, 2019

// The following defines are used for setting and clearing register bits
// on the Arduino processor. Low-level stuff: leave alone.
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int analogPin = A0;     // Analog input pin. Make sure to keep between 0 and 5V.


void setup() {
  sbi(ADCSRA,ADPS2);     // Next three lines make the ADC run faster
  cbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);
  Serial.begin(9600); // set the baud rate

}

void loop() {
  int val, spikes, samps, sincehey;
  bool lastWasIntr = 0;

  while (1){
    val = analogRead(analogPin);  // New input
    samps++;
    if (val > 15){
      spikes++;
    }

    if (samps > 200) {
      if (spikes == 0) {
          if (!lastWasIntr) {
            Serial.println("Intr");
          }
          lastWasIntr = 1;
      }
      if (spikes > 5) {
          if (lastWasIntr) {
            Serial.println("Back on");
          }
          lastWasIntr = 0;
      }
      // Serial.println("samps:");
      // Serial.println(samps);
      // Serial.println("spikes:");
      // Serial.println(spikes);
      samps = 0;
      spikes = 0;
    }
    // Serial.println("sendtext");
  }
}
