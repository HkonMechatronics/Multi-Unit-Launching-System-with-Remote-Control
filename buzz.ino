void setup_buzz() {
pinMode(buzzerPin, OUTPUT);
}

void annonserSeier() {
tone(buzzerPin, 1046);
delay(100);
tone(buzzerPin, 1318);
delay(100);
tone(buzzerPin, 1567);
delay(100);
tone(buzzerPin, 2093);
delay(400);
noTone(buzzerPin);
}