
#include "arduino_api.h"

// int LED_BUILTIN;

void setup() {
  println("I am Daniel 😎");

  // LED_BUILTIN = getPinLED();

  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);

  println("Greeting: ");
  char greeting[64];
  getGreeting(greeting, sizeof(greeting));
  print(greeting);
}

// the loop function runs over and over again forever
void loop() {
  for (double celsius = -1; celsius <= 20; celsius++) {
    // convert decimal into integer, later convert back
    double celsiusf = celsius * 100;
    double kelvin = celsiusf + 27315;
    println(" Celsius = ");
    printn(celsiusf);
    print(" Kelvin= ");
    printn(kelvin);
    println("---------");
  }
}
/*
 * Entry point
 */

WASM_EXPORT
void _start() {
  setup();
  while (1) {
    loop();
    break;
  }
}
