#include <Arduino_FreeRTOS.h>
#include "semphr.h"

#define  RED      6
#define  YELLOW   7
#define  BLUE     8

SemaphoreHandle_t xCountingSemaphore;

void setup()
{
  Serial.begin(9600);
  
  pinMode(RED,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(BLUE,OUTPUT);
  
  xCountingSemaphore = xSemaphoreCreateCounting(1,0); # 1 = 1 serial monitor

  xTaskCreate(redLEDControllerTask, "Red Task",100,NULL,1,NULL);
  xTaskCreate(blueLEDControllerTask, "Blue Task", 100,NULL,1,NULL);
  xTaskCreate(yellowLEDControllerTask,"Yellow Task", 100,NULL,1,NULL);
 
 #xSemaphoreTake(xCountingSemaphore);  # if it is commented their is deadlock as task initiall dont have semaphore
 
}

void redLEDControllerTask(void *pvParameters)
{
 
  while(1)
  {
	xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    digitalWrite(RED, digitalRead(RED)^1);
    Serial.println("RED Task ");
    xSemaphoreGive(xCountingSemaphore);
	vTaskDelay(1);
   
  }
}
void blueLEDControllerTask(void *pvParameters)
{
  
  while(1)
  {
	xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    digitalWrite(BLUE, digitalRead(BLUE)^1);
	Serial.println("Blue Task: ");
	xSemaphoreGive(xCountingSemaphore);
	vTaskDelay(1);
  }
}
void yellowLEDControllerTask(void *pvParameters)
{
  pinMode(YELLOW,OUTPUT);
  while(1)
  {
	xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    digitalWrite(YELLOW, digitalRead(YELLOW)^1);
	Serial.println("Yellow Task: ")
	xSemaphoreGive(xCountingSemaphore);
	vTaskDelay(1);
  }
}

void loop(){}

