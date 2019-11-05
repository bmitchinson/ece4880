void setup() {
  Serial.begin(9600); // set the baud rate
  Serial.println("ready"); // print "Ready" once
}

void loop() {
  delay(10000); // ms
  Serial.println("sendtext");
}
