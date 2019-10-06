#include "Common.h"
#include <Arduino.h>
void genericTask(void *parameter)
{
	Serial.print("Created task: Executing on core ");
	Serial.println(xPortGetCoreID());
	vTaskDelete(NULL);
}

void setup_executionCore()
{
	Serial.print("Setup: Executing on core ");
	Serial.println(xPortGetCoreID());

	xTaskCreate(
		genericTask,   /* Task function. */
		"genericTask", /* String with name of task. */
		10000,		   /* Stack size in words. */
		NULL,		   /* Parameter passed as input of the task */
		2,			   /* Priority of the task. */
		NULL);		   /* Task handle. */
	delay(2000);
}

void  loop_getExecutionCore(){
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}