const int downOptoPin = 2;
const int upOptoPin = 3;

String deskState = "DOWN";

void move(int pin);

void setup() {
  pinMode(downOptoPin, OUTPUT);
  pinMode(upOptoPin, OUTPUT);
}

void loop() {
  if (deskState == "DOWN") {
    move(upOptoPin);
    deskState = "UP";
  } else if (deskState == "UP") {
    move(downOptoPin);
    deskState = "DOWN";
  }

  delay(10000);
}

void move(int pin) {
  digitalWrite(pin, HIGH);
  delay(2000);
  digitalWrite(pin, LOW);
}
