
#include "arduino_api.h"

// int LED_BUILTIN;

void setup() {
  println("METRIC UNIT TO EMPERIAL CONVERSION running on WASM3!");

  // LED_BUILTIN = getPinLED();

  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);

  println("Status: ");
  char greeting[64];
  getGreeting(greeting, sizeof(greeting));
  print(greeting);
  println(" ");
}

// the loop function runs over and over again forever
void loop() {
  int acc_x = getx();
  int acc_xft = (acc_x * 3.2808399);
  int acc_y = gety();
  int acc_yft = (acc_y * 3.2808399);
  int acc_z = getz();
  int acc_zft = (acc_z * 3.2808399);
  int temp = gettemp();
  int tempf = ((temp * 1.8) + 32);
  println(" ");
  println(
      "----------------------------------------------------------------------"
      "---------------");
  println(" Acc_x = ");
  printn(acc_x);
  print("m/s^2 ");
  print(" Acc_y = ");
  printn(acc_y);
  print("m/s^2 ");
  print(" Acc_z = ");
  printn(acc_z);
  print("m/s^2 ");
  print(" Temperature = ");
  printn(temp);
  print("°C");
  println(" ");
  println("                     METRIC UNIT TO EMPERIAL CONVERSION");
  println(" ");
  println(" Acc_x = ");
  printn(acc_xft);
  print("ft/s^2 ");
  print(" Acc_y = ");
  printn(acc_yft);
  print("ft/s^2 ");
  print(" Acc_z = ");
  printn(acc_zft);
  print("ft/s^2 ");
  print(" Temperature = ");
  printn(tempf);
  print("°F");
}
/*
 * Entry point
 */

WASM_EXPORT
void _start() {
  setup();
  while (1) {
    loop();
  }
}
