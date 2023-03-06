#include <Adafruit_BusIO_Register.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <Wire.h>
#include <activity_classification_inferencing.h>
#include <m3_env.h>
#include <wasm3.h>

// Sensor Configuration
#define FREQUENCY_HZ 250
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))
Adafruit_MPU6050 mpu;

float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;

static unsigned long last_interval_ms = 0;

// Display Configuration
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

const char *ssid = "Don’t try connect it";
const char *password = "Ahxuanlagiwth2000";
// const char *ssid = "Daniel";
// const char *password = "000806abc";
/*
 * Configuration
 */

#define MODE_PIN 32 // Button to switch modes
int counter = 1;

#define BUTTON_PIN 18 // button to collect/upload data to AWS

// define LED PIN
#ifndef LED_PIN
#define LED_PIN 10
#endif

// Task Handles
TaskHandle_t inferencing_handle = NULL;
TaskHandle_t wasm_handle = NULL;

#define WASM_STACK_SLOTS 2048
#define NATIVE_STACK_SIZE (32 * 1024)

// For (most) devices that cannot allocate a 64KiB wasm page
//#define WASM_MEMORY_LIMIT   4096

/*
 * WebAssembly app
 */

#include "C:/Users/limjo/wasm3-arduino_OTA/Wasm_Advanced_OTA/wasm_apps/cpp/app.wasm.h"

/*
 * API bindings
 *
 * Note: each RawFunction should complete with one of these calls:
 *   m3ApiReturn(val)   - Returns a value
 *   m3ApiSuccess()     - Returns void (and no traps)
 *   m3ApiTrap(trap)    - Returns a trap
 */

m3ApiRawFunction(m3_arduino_millis) {
  m3ApiReturnType(uint32_t)

      m3ApiReturn(millis());
}

m3ApiRawFunction(m3_arduino_delay) {
  m3ApiGetArg(uint32_t, ms)

      // You can also trace API calls
      // Serial.print("api: delay ");
      Serial.println(ms);

  delay(ms);

  m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_printn) {
  m3ApiGetArg(int32_t, n)
      // You can also trace API calls
      float nn = ((float)n / 100);
  // Serial.print(n);
  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.println(nn);
  // display.display();
  Serial.println(nn);
  delay(200);
  m3ApiSuccess();
}

// This maps pin modes from arduino_wasm_api.h
// to actual platform-specific values
uint8_t mapPinMode(uint8_t mode) {
  switch (mode) {
  case 0:
    return INPUT;
  case 1:
    return OUTPUT;
  case 2:
    return INPUT_PULLUP;
  }
  return INPUT;
}

m3ApiRawFunction(m3_arduino_pinMode) {
  m3ApiGetArg(uint32_t, pin) m3ApiGetArg(uint32_t, mode)

#if !defined(PARTICLE)
      typedef uint8_t PinMode;
#endif
  pinMode(pin, (PinMode)mapPinMode(mode));

  m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_digitalWrite) {
  m3ApiGetArg(uint32_t, pin) m3ApiGetArg(uint32_t, value)

      digitalWrite(pin, value);

  m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getPinLED) {
  m3ApiReturnType(uint32_t)

      m3ApiReturn(LED_PIN);
}

m3ApiRawFunction(m3_arduino_print) {
  m3ApiGetArgMem(const uint8_t *, buf) m3ApiGetArg(uint32_t, len)

      // printf("api: print %p %d\n", buf, len);
      // display.print(buf,len);
      display.clearDisplay();
  Serial.write(buf, len);

  m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getGreeting) {
  m3ApiGetArgMem(uint8_t *, out) m3ApiGetArg(uint32_t, out_len)

      const char buff[] = "Hello WASM world! 😊";
  memcpy(out, buff, min(sizeof(buff), out_len));

  m3ApiSuccess();
}

M3Result LinkArduino(IM3Runtime runtime) {
  IM3Module module = runtime->modules;
  const char *arduino = "arduino";

  m3_LinkRawFunction(module, arduino, "millis", "i()", &m3_arduino_millis);
  m3_LinkRawFunction(module, arduino, "delay", "v(i)", &m3_arduino_delay);
  m3_LinkRawFunction(module, arduino, "printn", "v(i)", &m3_arduino_printn);
  m3_LinkRawFunction(module, arduino, "pinMode", "v(ii)", &m3_arduino_pinMode);
  m3_LinkRawFunction(module, arduino, "digitalWrite", "v(ii)",
                     &m3_arduino_digitalWrite);

  // Test functions
  m3_LinkRawFunction(module, arduino, "getPinLED", "i()",
                     &m3_arduino_getPinLED);
  m3_LinkRawFunction(module, arduino, "getGreeting", "v(*i)",
                     &m3_arduino_getGreeting);
  m3_LinkRawFunction(module, arduino, "print", "v(*i)", &m3_arduino_print);
  return m3Err_none;
}

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg)                                                       \
  {                                                                            \
    Serial.print("Fatal: " func " ");                                          \
    Serial.println(msg);                                                       \
    return;                                                                    \
  }

void wasm_task(void *) {
  Serial.println("\nWasm3 v" M3_VERSION " (" M3_ARCH "), build " __DATE__
                 " " __TIME__);
  for (;;) {
    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment();
    if (!env)
      FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime(env, WASM_STACK_SLOTS, NULL);
    if (!runtime)
      FATAL("NewRuntime", "failed");

#ifdef WASM_MEMORY_LIMIT
    runtime->memoryLimit = WASM_MEMORY_LIMIT;
#endif

    IM3Module module;
    result = m3_ParseModule(env, &module, app_wasm, app_wasm_len);
    if (result)
      FATAL("ParseModule", result);

    result = m3_LoadModule(runtime, module);
    if (result)
      FATAL("LoadModule", result);

    result = LinkArduino(runtime);
    if (result)
      FATAL("LinkArduino", result);

    IM3Function f;
    result = m3_FindFunction(&f, runtime, "_start");
    if (result)
      FATAL("FindFunction", result);

    Serial.println("Running WebAssembly...");
    result = m3_CallV(f);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.fillRoundRect(0, 0, 128, 32, 4, WHITE);
    display.setTextSize(2);
    display.setCursor(0, 12);
    display.setTextColor(BLACK);
    display.print("WASM MODE");
    display.display();
    // Should not arrive here

    if (result) {
      M3ErrorInfo info;
      m3_GetErrorInfo(runtime, &info);
      Serial.print("Error: ");
      Serial.print(result);
      Serial.print(" (");
      Serial.print(info.message);
      Serial.println(")");
      if (info.file && strlen(info.file) && info.line) {
        Serial.print("At ");
        Serial.print(info.file);
        Serial.print(":");
        Serial.println(info.line);
      }
    }
  }
}

void mode(void *) {
  for (;;) {
    // Handle input
    int switchVal = digitalRead(MODE_PIN);
    if (switchVal == HIGH) {
      counter++;
      // Reset count if over max mode number
      if (counter == 3) {
        counter = 1;
      }
    } else
      // Change mode
      switch (counter) {
      case 1:
        if (wasm_handle != NULL) {
          vTaskSuspend(wasm_handle);
          vTaskResume(inferencing_handle);
        }
        break;
      case 2:
        if (inferencing_handle != NULL) {
          vTaskSuspend(inferencing_handle);
          vTaskResume(wasm_handle);
        }
        break;
      case 3:
        Serial.println("button is 3");
        break;
      }
  }
}

void ei_printf(const char *format, ...) {
  static char print_buf[1024] = {0};
  va_list args;
  va_start(args, format);
  int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
  va_end(args);
  if (r > 0) {
    Serial.write(print_buf);
  }
}

void inferencing(void *) {
  Serial.print("Features: ");
  Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  Serial.print("Label count: ");
  Serial.println(EI_CLASSIFIER_LABEL_COUNT);
  for (;;) {
    sensors_event_t a, g, temp;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.fillRoundRect(0, 0, 80, 23, 4, WHITE);
    display.drawRoundRect(81, 0, 47, 23, 4, WHITE);
    display.setTextSize(1);
    display.setCursor(85, 3);
    display.setTextColor(WHITE);
    display.println(" WIFI:");
    display.setTextSize(1);
    display.setCursor(86, 12);
    display.setTextColor(BLACK, WHITE);
    display.print("  ON  ");

    if (digitalRead(BUTTON_PIN) == HIGH) {
      display.setTextSize(1);
      display.setCursor(0, 24);
      display.setTextColor(WHITE);
      display.println(" DATA: COLLECTING");
    }
    if (digitalRead(BUTTON_PIN) == LOW) {
      display.setTextSize(1);
      display.setCursor(0, 24);
      display.setTextColor(WHITE);
      display.println(" DATA: NOT COLLECTING");
    }
    if (millis() > last_interval_ms + INTERVAL_MS) {
      last_interval_ms = millis();

      mpu.getEvent(&a, &g, &temp);

      features[feature_ix++] = a.acceleration.x;
      features[feature_ix++] = a.acceleration.y;
      features[feature_ix++] = a.acceleration.z;

      if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        Serial.println("Running the inference...");
        signal_t signal;
        ei_impulse_result_t result;
        int err = numpy::signal_from_buffer(
            features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
        if (err != 0) {
          ei_printf("Failed to create signal from buffer (%d)\n", err);
          return;
        }

        EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);

        if (res != 0)
          return;
        size_t ix = 0;
        float MaxValue = max(max(max(result.classification[ix].value,
                                     result.classification[ix + 1].value),
                                 result.classification[ix + 2].value),
                             result.classification[ix + 3].value);
        if (result.classification[ix].value == MaxValue) {
          display.setTextSize(2);
          display.setCursor(15, 0);
          display.setTextColor(BLACK);
          display.println(result.classification[ix].label);
          // display.print(": ");
          display.setTextSize(1);
          display.setCursor(25, 15);
          display.print(result.classification[ix].value);
          display.display();
        }

        if (result.classification[ix + 1].value == MaxValue) {
          display.setTextSize(2);
          display.setCursor(12, 0);
          display.setTextColor(BLACK);
          display.println(result.classification[ix + 1].label);
          // display.print(": ");
          display.setTextSize(1);
          display.setCursor(25, 15);
          display.print(result.classification[ix + 1].value);
          display.display();
        }
        if (result.classification[ix + 2].value == MaxValue) {
          display.setTextSize(2);
          display.setCursor(12, 0);
          display.setTextColor(BLACK);
          display.println(result.classification[ix + 2].label);
          // display.print(": ");
          display.setTextSize(1);
          display.setCursor(25, 15);
          display.print(result.classification[ix + 2].value);
          display.display();
        }
        if (result.classification[ix + 3].value == MaxValue) {
          display.setTextSize(2);
          display.setCursor(12, 0);
          display.setTextColor(BLACK);
          display.println(result.classification[ix + 3].label);
          // display.print(": ");
          display.setTextSize(1);
          display.setCursor(25, 15);
          display.print(result.classification[ix + 3].value);
          display.display();
        }
        ei_printf("Predictions ");
        ei_printf("(DSP: %d ms., Classification: %d ms.)", result.timing.dsp,
                  result.timing.classification);
        ei_printf(": \n");

        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
          ei_printf("    %s: %.5f\n", result.classification[ix].label,
                    result.classification[ix].value);
        }
        feature_ix = 0;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  WiFi.mode(WIFI_STA); // Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  if (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.fillRoundRect(0, 0, 80, 23, 4, WHITE);
    display.drawRoundRect(81, 0, 47, 23, 4, WHITE);
    display.setTextSize(1);
    display.setCursor(85, 3);
    display.setTextColor(WHITE);
    display.println(" WIFI:");
    display.setTextSize(1);
    display.setCursor(86, 12);
    display.setTextColor(WHITE);
    display.print("  OFF ");
    display.setTextSize(2);
    display.setCursor(15, 0);
    display.setTextColor(BLACK);
    display.println("idle");
    display.setTextSize(1);
    display.setCursor(0, 24);
    display.setTextColor(WHITE);
    display.println(" DATA: NOT COLLECTING");
    display.display();
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  xTaskCreatePinnedToCore(mode,   // function name
                          "mode", // task name
                          4000,   // stack size
                          NULL,   // task parameter
                          1,      // task priority
                          NULL,   // task handle
                          CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(inferencing,         // function name
                          "inferencing",       // task name
                          4000,                // stack size
                          NULL,                // task parameter
                          2,                   // task priority
                          &inferencing_handle, // task handle
                          CONFIG_ARDUINO_RUNNING_CORE);

  xTaskCreate(&wasm_task, "wasm3", NATIVE_STACK_SIZE, NULL, 1, &wasm_handle);
}

void loop() {}
