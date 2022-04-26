const int ANALOG_PIN = 9;

void setup() {
    pinMode(ANALOG_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    int res = analogRead(ANALOG_PIN);
    Serial.print(res);
}
