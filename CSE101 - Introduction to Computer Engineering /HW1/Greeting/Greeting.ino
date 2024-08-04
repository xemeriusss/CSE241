
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

    Serial.println("Hi my name is Sudenur Meydan.");
    delay(500);


}

void loop() {
}
