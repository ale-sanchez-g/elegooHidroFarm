#include <LiquidCrystal.h>

const int rs = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const long int waterFor = 5;
// set regressive timeer for 3 hours to water for 15 min
long int  waterTimer = 10800;
const long int  startTime = 10800;

void setup() {
  // Serial.begin(9600); disable as it breaks the LCD display

  // this is to set up the display view on the LCD screen
  analogWrite(11,100); // Generate PWM signal at pin D11, value of 100 (out of 255)
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  lcd.print("    Welcome");
  lcd.setCursor(0, 2);
  lcd.print("Weather station");
  delay(5000);
  lcd.clear();
  
  Serial.println("turning off - start");
  Serial.println("Current timer = " + String(waterTimer));
  Serial.println("Current startTime = " + String(startTime));
  Serial.println("Current water for = " + String(waterFor));

}

void loop() {  

  lcd.print("Gathering...");
  delay(2000);

  // Read water level
  int value = analogRead(A0);

  // Read temp level
  float temp = analogRead(A1);
  float voltage  = temp * 5.019;
  voltage /= 1024.0;
  
  float newTemp = (voltage - 0.5) * 100;
  lcd.clear();

  lcd.print("Temp: " + String(newTemp) + "C");

  if (newTemp > 30 ) {
  	waterTimer = startTime;
  }
 
  if (waterTimer == startTime) {
    Serial.println("turning on for 15min");
    digitalWrite(10, HIGH);
    delay(waterFor * 1000);
    Serial.println("turning off ater 15min");
    digitalWrite(10, LOW);
    waterTimer -= waterFor;
  }
  
  delay(5000);
  lcd.clear();

  lcd.print("Water Level:" + String(value));
  delay(5000);
  lcd.clear();
  
  waterTimer -= 17;
  
  lcd.print("Next Flood in");
  lcd.setCursor(0, 2);
  lcd.print(String(waterTimer/60) + " min");
  delay(5000);
  lcd.clear();
}