#include <Arduino.h>

#define NTC_PIN     32
#define RELAY_PIN   26
#define BUZZER_PIN  25
#define LED_PIN     27

String state = "IDLE";

// Accurate NTC temp reading
float readTemperatureC() {
  const float BETA = 3950;
  const float SERIES_RESISTOR = 10000;
  int adc = analogRead(NTC_PIN);
  float voltage = adc * 3.3 / 4095.0;
  float resistance = SERIES_RESISTOR * (voltage / (3.3 - voltage));
  float tempK = 1.0 / (1.0 / 298.15 + (1.0 / BETA) * log(resistance / 10000.0));
  return tempK - 273.15;
}

// Heater control logic
void HeaterTask(void *parameter) {
  while (true) {
    float temp = readTemperatureC();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C | ");

    if (temp < 30) {
      state = "IDLE";
      digitalWrite(RELAY_PIN, HIGH);
    } else if (temp >= 30 && temp < 40) {
      state = "HEATING";
      digitalWrite(RELAY_PIN, LOW);
    } else if (temp >= 40 && temp < 45) {
      state = "STABILIZING";
      digitalWrite(RELAY_PIN, LOW);
    } else if (temp >= 45 && temp < 50) {
      state = "TARGET REACHED";
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      state = "OVERHEAT";
      digitalWrite(RELAY_PIN, HIGH);
    }

    Serial.println("State: " + state);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// LED Indicator Task
void LEDTask(void *parameter) {
  while (true) {
    if (state == "HEATING" || state == "STABILIZING") {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// Buzzer alert on overheat
void BuzzerTask(void *parameter) {
  while (true) {
    if (state == "OVERHEAT") {
      digitalWrite(BUZZER_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(200));
      digitalWrite(BUZZER_PIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(200));
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

// Mock BLE Advertising (print to Serial)
void BLETask(void *parameter) {
  while (true) {
    Serial.println("[BLE] Advertising state: " + state);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  xTaskCreate(HeaterTask, "Heater", 4096, NULL, 1, NULL);
  xTaskCreate(LEDTask, "LED", 1024, NULL, 1, NULL);
  xTaskCreate(BuzzerTask, "Buzzer", 1024, NULL, 1, NULL);
  xTaskCreate(BLETask, "BLE", 2048, NULL, 1, NULL);
}

void loop() {
  // All handled by FreeRTOS
}
