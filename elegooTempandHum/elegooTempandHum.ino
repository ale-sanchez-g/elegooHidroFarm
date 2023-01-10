#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 8     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT sensor types DHT11, DHT22, DHT21 


// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// To reduce the noce the below Variables are used to control frequency the data points are published
float startTemp = 0;
float startHum = 0;

const int rs = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7, relayPIN = 10, waterLevel = A0, LCDdisplay = 11;
// Set number of seconds for flooding ( 900 = 15 min)
const long int waterFor = 900;
// set regressive timeer for 3 hours to water for 15 min
long int  waterTimer = 10800;
const long int  startTime = 10800;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {

  // Serial.begin(9600); // disable as it breaks the LCD display

  // this is to set up the display view on the LCD screen
  analogWrite(LCDdisplay,100); // Generate PWM signal at pin D11, value of 100 (out of 255)
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  lcd.print("    Welcome");
  lcd.setCursor(0, 2);
  lcd.print("Weather station");
  delay(5000);
  lcd.clear();

  pinMode(relayPIN, OUTPUT);
  dht.begin();

}

void loop() {  
  
  lcd.print("Gathering...");
     
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float newHum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float newTemp = dht.readTemperature();
  // Read water level
  int value = analogRead(waterLevel);


  // Check if any reads failed and exit early (to try again).
  if (isnan(newHum) || isnan(newTemp)) {
    lcd.print(F("Failed to read from DHT sensor!"));
    delay(5000);
    lcd.clear();
    
    Serial.println("turning off - no data");
    digitalWrite(relayPIN, LOW);
    
    delay(5000);

    return;
  };

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(newTemp, newHum, false);
  
  delay(2000);
  lcd.clear();

  // Print humidity and Temperature
  lcd.print("Humd: " + String(newHum) + "%");
  lcd.setCursor(0, 2);
  lcd.print("Temp: " + String(newTemp) + "C");

  // if the temperature is above 30 degrees, force flooding
  if (newTemp > 30 ) {
  	waterTimer = startTime;
  }

  // Check if flooding is required
  // TODO: improve logic to check temperature and humidity during flooding
  if (waterTimer == startTime) {
    digitalWrite(relayPIN, HIGH);
    delay(waterFor * 1000);
    digitalWrite(relayPIN, LOW);
    waterTimer = (waterTimer - waterFor);
  }
  
  // Display temperature for 5 seconds if not flooding
  delay(5000);
  lcd.clear();

  // Print heat index and water level
  lcd.print("HeatInd: " + String(hic) + "C"); 
  lcd.setCursor(0, 2);
  lcd.print("Water Level:" + String(value));
  delay(5000);
  lcd.clear();
  
  // reseting variables
  waterTimer = (waterTimer - 17);
  startTemp = newTemp;
  startHum = newHum;

  // display when next flooding is expected
  lcd.print("Next Flood in");
  lcd.setCursor(0, 2);
  lcd.print(String(waterTimer/60) + "min");
  delay(5000);
  lcd.clear();
}