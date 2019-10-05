#include <Arduino.h>

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {4, 0, false};

void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}



void setup() {
    Serial.begin(115200);
    pinMode(button1.PIN, INPUT);
    attachInterruptArg(button1.PIN, isr, &button1, FALLING);
}

void loop() {
    if (button1.pressed) {
        Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;
    }
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 10000) {
      lastMillis = millis();
      detachInterrupt(button1.PIN);
    }
}
