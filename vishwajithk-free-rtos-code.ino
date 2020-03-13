#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define THRESHOLD 512
#define MS_DELAY 500
#define Q_SIZE 5

//QueueHandle_t q1;
QueueHandle_t q2;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
 Serial.begin(9600);
 lcd.begin();
 pinMode(A0, INPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 //q1 =  xQueueCreate(Q_SIZE,sizeof(int32_t));
 q2 =  xQueueCreate(Q_SIZE,sizeof(int32_t));

 xTaskCreate(vSenderTask,"Sender Task",200,NULL,1,NULL);
 //xTaskCreate(vReceiverTask1,"Receiver Task 1", 200,NULL, 1, NULL);
 xTaskCreate(vReceiverTask2,"Receiver Task 2", 100,NULL, 1, NULL);
}

void vSenderTask(void *pvParameters)
{
  BaseType_t qStatus;
  
  while(1)
  {
    //int32_t valueToSend = random(1, 100);//getLDR();
    int32_t valueToSend = analogRead(A0);
    //qStatus = xQueueSend(q1,&valueToSend,0); 
    qStatus = xQueueSend(q2,&valueToSend,0);
    vTaskDelay(pdMS_TO_TICKS(MS_DELAY));
  }
}

/*void vReceiverTask1(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while(1)
  {
    xQueueReceive(q1,&valueReceived,xTicksToWait);
    Serial.print("Received value  : ");
    Serial.println(valueReceived);
    vTaskDelay(pdMS_TO_TICKS(MS_DELAY));
  }
}*/

void vReceiverTask2(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  lcd.setCursor(0, 0);
  while(1)
  {
    lcd.clear();
    xQueueReceive(q2,&valueReceived,xTicksToWait);
    lcd.print(valueReceived);

    if (valueReceived < THRESHOLD)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);

    vTaskDelay(pdMS_TO_TICKS(MS_DELAY));
  }
}

void loop(){}
