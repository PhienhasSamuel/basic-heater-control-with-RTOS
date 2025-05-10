#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#define HEATER_PIN 2
#define BUZZER_PIN 4

enum State {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

State currentState = IDLE;
float currentTemp = 0.0;

// Thresholds
const float startHeatingTemp = 25.0;
const float targetTemp = 35.0;
const float overheatTemp = 45.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(HEATER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // FreeRTOS Tasks
  xTaskCreatePinnedToCore(tempReadTask, "Temp Read Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(stateControlTask, "State Control Task", 2048, NULL, 1, NULL, 1);
}

void tempReadTask(void *parameter) {
  while (true) {
    float temp = dht.readTemperature();
    if (!isnan(temp)) {
      currentTemp = temp;
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // every 1 second
  }
}

void stateControlTask(void *parameter) {
  while (true) {
    State newState = currentState;

    if (currentTemp < startHeatingTemp) {
      newState = HEATING;
    } else if (currentTemp >= startHeatingTemp && currentTemp < targetTemp) {
      newState = STABILIZING;
    } else if (currentTemp >= targetTemp && currentTemp <= overheatTemp) {
      newState = TARGET_REACHED;
    } else if (currentTemp > overheatTemp) {
      newState = OVERHEAT;
    } else {
      newState = IDLE;
    }

    // Control outputs
    switch (newState) {
      case HEATING:
      case STABILIZING:
        digitalWrite(HEATER_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
        break;

      case TARGET_REACHED:
        digitalWrite(HEATER_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        break;

      case OVERHEAT:
        digitalWrite(HEATER_PIN, LOW);
        digitalWrite(BUZZER_PIN, HIGH);
        break;

      case IDLE:
      default:
        digitalWrite(HEATER_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        break;
    }

    currentState = newState;

    // Log
    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.print("°C | State: ");
    switch (currentState) {
      case IDLE: Serial.println("Idle"); break;
      case HEATING: Serial.println("Heating"); break;
      case STABILIZING: Serial.println("Stabilizing"); break;
      case TARGET_REACHED: Serial.println("Target Reached"); break;
      case OVERHEAT: Serial.println("Overheat"); break;
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS); // every 1 second
  }
}
void loop() {

}