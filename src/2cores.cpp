/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
 
TaskHandle_t Task1;
 
void setup() {
  Serial.begin(115200);
 
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500);
}
 
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
 
  for(;;){
    Serial.print("loop() running on core ");
    Serial.println(xPortGetCoreID());
    delay(500);
  }
}
 
void loop() {
  Serial.print("loop() running on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}