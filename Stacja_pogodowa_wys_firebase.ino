#include <Firebase_Arduino_WiFiNINA.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
hd44780_I2Cexp lcd(0x3F);
// dołączam bibliotekę od obsługi czujnika DHTxx 
#define DHTPIN 2 // definicja preprocesora
#define DHTTYPE DHT22 // definicja preprocesora
#define PIR 3
#define LED 25
#define COMMON_ANODE
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

DHT dht(DHTPIN, DHTTYPE);
char ssid[] = "";
char pass[] = "";
FirebaseData firebaseData;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup() {
  Firebase.begin("", "", ssid, pass);
  Firebase.reconnectWiFi(true);

  dht.begin();
  Serial.begin(9600);
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status)
  {
    hd44780::fatalError(status);
  }

   pinMode(PIR, INPUT);
   pinMode(LED, OUTPUT);
   pinMode(redPin, OUTPUT);
   pinMode(greenPin, OUTPUT);
   pinMode(bluePin, OUTPUT);

}
void loop() {
  delay(4000); // czekam 2s
  float temp = dht.readTemperature();
  float wilg = dht.readHumidity();
  String json = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(wilg) + "}";
   
   if((digitalRead(PIR))==HIGH){
    digitalWrite(LED, HIGH);
    setColor(255, 0, 0);
   Serial.print("pir 1");
 
  } else {
    digitalWrite(LED, LOW);
    Serial.print("pir 0");
    setColor(0, 50, 84);
  }
  
    Serial.print("T: ");
    Serial.print(temp);
    Serial.print(" *C");
    Serial.print("  ");
    Serial.print("W: ");
    Serial.print(wilg);
    Serial.print("  %");
    Serial.print("\n");
    if (Firebase.updateNode(firebaseData, "/weather-station", json))
    {
      Serial.println("Timestamp: " + firebaseData.payload());
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
      Serial.println();
    }
    else
    {
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println();
    }
    lcd.setCursor(0,0);
    lcd.print("T: ");
    lcd.print(temp);
    lcd.print(" *C");
    lcd.setCursor(0,1);
    lcd.print("W: ");
    lcd.print(wilg);
    lcd.print(" %");
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
