// BLYNK CONNECTION
#define BLYNK_TEMPLATE_ID "TMPL6BwlLAcEF"
#define BLYNK_TEMPLATE_NAME "FinPro IoT Fish Feeder"
#define BLYNK_AUTH_TOKEN "_cu_rU2BO_eKINNFT72EzTwHZNX6TYih"
#define BLYNK_PRINT Serial

// LIBRARIES
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32Servo.h>

// WIFI CREDENTIALS
char ssid[] = "Giovano";
char pass[] = "mantap123";

// DHT DEFINITIONS
#define DHTPIN 4 //GPIO4
#define DHTTYPE DHT11

// VIRTUAL PINS DEFINITION
#define VPIN_SWITCH V0
#define VPIN_TEMP V1
#define VPIN_HUMID V2
#define VPIN_TIMER V3
#define VPIN_SET V4

// DIGITAL PINS DEFINITION
#define SERVOPIN 18 // GPIO18
#define LEDPIN 19 // GPIO19
#define BUZZERPIN 21 // GPIO21
#define INTPIN 22 // GPIO22

// OBJECT DEFINITION
DHT dht(DHTPIN, DHTTYPE); // SENSOR
Servo myservo;

// SHARED VARIABLES
double temperature = 0;
double humidity = 0;
bool switchState = false; // TURN ON OR OFF
bool setState = false; // SET TIMER INTERVAL
int timerValue = 0; // FEED INTERVAL
int tempTimerValue = 0;
volatile bool stopAllTasks = false;

// ISR
void IRAM_ATTR handleInterrupt(){
  stopAllTasks = true;
}

// SERVO VARIABLES
const int servoOpenTime = 2000;

// MUTEX
SemaphoreHandle_t xServoMutex;

// TASK HANDLERS
TaskHandle_t SensorTaskHandle = NULL;
TaskHandle_t BlynkTaskHandle = NULL;
TaskHandle_t ServoTaskHandle = NULL;

// FUNCTION PROTOTYPE
void WiFiConnect();
void BlynkConnect();
void MutexCreation();

void SensorTask(void *pvParameters);
void BlynkTask(void *pvParameters);
void ServoTask(void *pvParameters);

//MAIN FUNCTIONS
void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(INTPIN, INPUT_PULLUP);

  // ATTACH INTERRUPT TO INTERRUPT PIN
  attachInterrupt(digitalPinToInterrupt(INTPIN), handleInterrupt, FALLING);

  //SETUP
  int status = 0;

  WiFiConnect();
  BlynkConnect();
  MutexCreation();

  dht.begin();
  myservo.attach(SERVOPIN);
  myservo.write(0);

  status = 1;

  //TASK CREATION
  if(status){
    xTaskCreatePinnedToCore(SensorTask, "SensorTask", 4096, NULL, 1, &SensorTaskHandle, 1);
    xTaskCreatePinnedToCore(BlynkTask, "BlynkTask", 4096, NULL, 1, &BlynkTaskHandle, 1);
    xTaskCreatePinnedToCore(ServoTask, "ServoTask", 4096, NULL, 1, &ServoTaskHandle, 1);
  }
}

void loop() {
  if (stopAllTasks) {
    // All tasks are stopped; halt the loop
    Serial.println("System halted.");
    digitalWrite(LEDPIN, LOW);   // Turn off LED
    digitalWrite(BUZZERPIN, LOW); // Turn off buzzer
    myservo.write(0);

        // Delete all tasks
    if (SensorTaskHandle != NULL) {
      vTaskDelete(SensorTaskHandle);
      SensorTaskHandle = NULL; // Clear handle to indicate it's been deleted
      Serial.println("SensorTask deleted.");
    }

    if (BlynkTaskHandle != NULL) {
      vTaskDelete(BlynkTaskHandle);
      BlynkTaskHandle = NULL; // Clear handle to indicate it's been deleted
      Serial.println("BlynkTask deleted.");
    }

    if (ServoTaskHandle != NULL) {
      vTaskDelete(ServoTaskHandle);
      ServoTaskHandle = NULL; // Clear handle to indicate it's been deleted
      Serial.println("ServoTask deleted.");
    }

    while (true) {
      delay(1000); // Keep the system halted
    }
  }
}

// WIFI CONNECT FUNCTION
void WiFiConnect() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

// BLYNK CONNECT FUNCTION
void BlynkConnect() {
  Serial.println("Connecting to Blynk...");
  Blynk.config(BLYNK_AUTH_TOKEN);
  while (Blynk.connect() == false) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Blynk");
}

// CREATE MUTEX FUNCTION
void MutexCreation() {
  Serial.println("Creating Mutex...");
  xServoMutex = xSemaphoreCreateMutex();
  if (xServoMutex == NULL) {
    Serial.println("Failed to create mutex. Halting.");
    while (1); // HALT IF MUTEX FAILS
  }
  Serial.println("Mutex created successfully.");
}

// BLYNK_WRITE HANDLER FOR ON/OFF STATE
BLYNK_WRITE(VPIN_SWITCH) {
  switchState = param.asInt();
  Serial.print("Switch state updated: ");
  Serial.println(switchState ? "ON" : "OFF");
}

BLYNK_WRITE(VPIN_SET){
  if(VPIN_SET){
    timerValue = tempTimerValue;
    Serial.print("Timer value updated: ");
    Serial.print(timerValue);
    Serial.println(" seconds");
  }
}

// BLYNK_WRITE HANDLER FOR TIMER
BLYNK_WRITE(VPIN_TIMER) {
  tempTimerValue = param.asInt(); // READ TIMER VALUE FROM BLYNK
}

// BLYNK TASK FUNCTION
void BlynkTask(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    Blynk.run();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// SENSOR TASK FUNCTION
void SensorTask(void *pvParameters) {
  (void)pvParameters;
  const TickType_t xDelay = 2000 / portTICK_PERIOD_MS; // READ EVERY 2 SECONDS

  for (;;) {
    if (switchState) {
      // READ TEMPERATURE AND HUMIDITY
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" *C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        // SEND TO BLYNK
        Blynk.virtualWrite(VPIN_TEMP, temperature);
        Blynk.virtualWrite(VPIN_HUMID, humidity);
      }
    }

    vTaskDelay(xDelay);
  }
}

// Shared variable to track servo status
bool servoWorking = false;

void ServoTask(void *pvParameters) {
  (void)pvParameters;
  const int servoClosedPosition = 0;    // Servo closed position
  const int servoOpenPosition = 90;     // Servo open position

  for (;;) {
    if (switchState && timerValue > 0) { // Only operate if the switch is ON and timer is set
      if (xSemaphoreTake(xServoMutex, portMAX_DELAY)) { // Lock the servo for safe access

        digitalWrite(LEDPIN, HIGH);
        digitalWrite(BUZZERPIN, HIGH);

        Blynk.virtualWrite(V5, "RUNNING"); // Send status to Blynk

        // Open the servo
        myservo.write(servoOpenPosition);
        vTaskDelay(servoOpenTime / portTICK_PERIOD_MS); // Wait while the servo is open

        // Close the servo
        myservo.write(servoClosedPosition);

        digitalWrite(LEDPIN, LOW);
        digitalWrite(BUZZERPIN, LOW);


        xSemaphoreGive(xServoMutex); // Release the servo lock
      }

      // Wait for the next interval
      if(switchState){
        vTaskDelay(timerValue * 1000 / portTICK_PERIOD_MS);
      }
    } else {
      Blynk.virtualWrite(V5, "IDLE"); // Send status to Blynk
      vTaskDelay(100 / portTICK_PERIOD_MS); // Short delay to avoid busy looping
    }
  }
}

