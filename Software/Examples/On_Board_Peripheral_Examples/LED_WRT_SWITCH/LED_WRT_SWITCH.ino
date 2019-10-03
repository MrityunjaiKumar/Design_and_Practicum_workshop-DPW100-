#define LED_BUILTIN 2
#define SWITCH_BUITLIN 4

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_BUITLIN, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(SWITCH_BUITLIN));
}
