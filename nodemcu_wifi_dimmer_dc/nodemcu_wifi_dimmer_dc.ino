#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

const char *ssid = "nodemcu dimmer";
const char *password = "123456789";

WiFiServer SERVER(80);
WiFiClient  CLIENT;

int LED1 = D5;
boolean DEBUG_CONNECTED = false;

String response = "{0}";
bool begins = false;
bool finished = false;

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  DC Dimmer");

  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);

  WiFi.softAP(ssid, password);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
  SERVER.begin();

  lcd.setCursor(0, 1);
  lcd.print("Brightness ");
  lcd.setCursor(11, 1);
  lcd.print(String(response));
}

void loop() {
  if (SERVER.hasClient()) {
    if (!CLIENT || !CLIENT.connected()) {
      if (CLIENT) {
        CLIENT.stop();
      }
      CLIENT = SERVER.available();
    }
  }

  if (CLIENT && CLIENT.connected()) {
    if (!DEBUG_CONNECTED) {
      lcd.setCursor(14, 0);
      lcd.print("CC");
      Serial.println("connected !!");
      DEBUG_CONNECTED = true;
    }
    if (CLIENT.available() > 0) {
      paintLCD();
      while (CLIENT.available() > 0) {
        char DATA = CLIENT.read();
        //Serial.println(DATA);
        if (DATA == '{')
        {
          response = "";
          begins = true;
        }
        if (begins)
        {
          response += (DATA);
        }
        if (DATA == '}')
        {
          begins = false;
          // Serial.println(response);
        }
      }
    }
  } else {
    lcd.setCursor(14, 0);
    lcd.print("NC");
    DEBUG_CONNECTED = false;
  }


  if (response == "{0}") {
    analogWrite(LED1, 0);
  } else if (response == "{10}") {
    analogWrite(LED1, 25);
  } else if (response == "{20}") {
    analogWrite(LED1, 50);
  } else if (response == "{30}") {
    analogWrite(LED1, 75);
  } else if (response == "{40}") {
    analogWrite(LED1, 100);
  } else if (response == "{50}") {
    analogWrite(LED1, 128);
  } else if (response == "{60}") {
    analogWrite(LED1, 150);
  } else if (response == "{70}") {
    analogWrite(LED1, 178);
  } else if (response == "{80}") {
    analogWrite(LED1, 204);
  } else if (response == "{90}") {
    analogWrite(LED1, 230);
  } else if (response == "{100}") {
    analogWrite(LED1, 255);
  }
}

void paintLCD() {
  lcd.setCursor(0, 1);
  lcd.print("Brightness ");

  lcd.setCursor(12, 1);
  lcd.print(" ");
  lcd.setCursor(13, 1);
  lcd.print(" ");
  lcd.setCursor(14, 1);
  lcd.print(" ");
  lcd.setCursor(15, 1);
  lcd.print(" ");

  lcd.setCursor(11, 1);
  lcd.print(String(response));
}
