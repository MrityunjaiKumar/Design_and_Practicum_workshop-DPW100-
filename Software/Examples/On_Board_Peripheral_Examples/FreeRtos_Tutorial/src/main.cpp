#include <Arduino.h>
#include "Common.h"
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(1000);
  // setup_executionCore();
  setup_QueuesBetweenTask();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // loop_getExecutionCore();
  void loop_QueuesBetweenTask();
}
