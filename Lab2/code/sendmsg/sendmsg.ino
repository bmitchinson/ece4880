void setup() {
  Serial.begin(9600); // set the baud rate
  Serial.println("text!"); // print "Ready" once
}

void loop() {
  delay(5000); // ms
  Serial.println("sendtext");
}
