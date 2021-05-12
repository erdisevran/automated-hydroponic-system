
//const int RELAY_PIN = A5;  

const int pompa_pin = 12;
int calis_dakika = 10;
int dur_dakika = 20;

void setup() { 
  pinMode(pompa_pin, OUTPUT);
}

void loop() {
  digitalWrite(pompa_pin, LOW);  // calis
  delay(calis_dakika*60000);
  digitalWrite(pompa_pin, HIGH); //dur
  delay(dur_dakika*60000);
}
