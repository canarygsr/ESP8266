
//String code;
//String call;
//const int Function_11_pin = Function_1_pin;
//const int Function_2_pin = 1;

void Function_1() {
    digitalWrite(config.function1_gpio, 0);
  delay(1000);
  digitalWrite(config.function1_gpio, 1);
  delay(1000);
}

void Function_2() {
    digitalWrite(config.function2_gpio, 0);
  delay(1000);
  digitalWrite(config.function2_gpio, 1);
  delay(1000);
}



void GPIO_toggle(int gpio, bool high_low){
    digitalWrite(gpio, high_low);
  delay(1000);
  digitalWrite(gpio, !high_low);
}

 void Function_3() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
 
  digitalWrite(1, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(3, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(4, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(1, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(3, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(4, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
 }

void setupFunctions () {
 pinMode(config.function1_gpio, OUTPUT); //note need to link to Function_1_pin
 pinMode(config.function2_gpio, OUTPUT);
 //  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
 //  pinMode(1, OUTPUT);
 //   pinMode(2, OUTPUT);
 //    pinMode(3, OUTPUT);
 //     pinMode(4, OUTPUT);
}
