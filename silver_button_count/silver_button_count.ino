#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "WIFI login";
const char* password = "WIFI pass";
String host = "https://www.googleapis.com/youtube/v3/channels?part=statistics&key=(API KEY)&id=(CHANNEL ID)";
String httpsFingerprint = "20:47:22:DB:D3:C9:1E:8F:6F:C1:C2:B2:3C:8C:8A:0F:C8:88:3E:45";
String payload;
int delta;
int R, G, B;
float delta_f = 0.0;
float K = 0.97, k;
int i = 0, j = 0, httpCode, new_subscribers;
long subscribers;
unsigned long last_check, last_color, last_Submins;
byte min_step = 3;
HTTPClient http;  //Declare an object of class HTTPClient

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ledR 13
#define ledG 14
#define ledB 12
boolean start_flag = true, error = false;

void setup () {
  Serial.begin(57600);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  Wire.begin(4, 5);
  lcd.init(); // initializing the LCD
  lcd.display(); //Enable or turn on the text
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi...");
  analogWrite(ledR, 500);
  analogWrite(ledG, 500);
  analogWrite(ledB, 500);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  lcd.clear();
  lcd.print("WiFi connected");
  analogWrite(ledR, 0);
  analogWrite(ledG, 0);
  analogWrite(ledB, 0);
}

void get_color() {
  int color = delta_f * 10;
  if (color <= 80) {
    k = map(color, 0, 80, 0, 1023);
    B = 1023 - k;
    G = k;
    R = 0;
  }
  if (color > 80 && delta <= 160) {
    k = map(color, 80, 160, 0, 1023);
    B = 0;
    G = 1023 - k;
    R = k;
  }
  if (color > 160) {
    k = map(color, 160, 250, 0, 1023);
    B = k;
    G = 0;
    R = 1023 - k;
  }
}

void getSubs() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    i = 0;
    httpCode = 0;
    payload = "";
    http.begin(host, httpsFingerprint);  //Specify request destination
    delay(1000);
    httpCode = http.GET();
    if (httpCode > 0) { //Check the returning code
      payload = http.getString();   //Get the request response payload
      error = false;
    } else {
      error = true;
    }
    http.end();   //Close connection

    if (payload.length() > 0) {
      i = 0;
      j = 0;
      const char compare[] = "erCount";

      while (i < payload.length()) {
        if (payload[i] == compare[j]) {
          j++;
        } else j = 0;
        if (j == 6) {
          int pos = 0;
          new_subscribers = 0;
          for (int ii = (i + 6); ii < (i + 12); ii++) {
            new_subscribers += (payload[ii] - '0') * pow(10, 5 - pos);
            pos++;
          }
          break;
        }
        i++;
      }
    }
  }
}

void loop() {
  if (start_flag) {
    getSubs();
    subscribers = new_subscribers;
    start_flag = false;
  }
  if (millis() - last_check > 10000) {
    getSubs();
    last_check = millis();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Subscribers:");
    lcd.setCursor(5, 1);
    lcd.print(new_subscribers);
    if (error) {
      lcd.setCursor(14, 1);
      lcd.print("er");
    }
  }
  if (millis() - last_Submins > (long)60000 * min_step) {
    delta = (long)new_subscribers - subscribers;
    if (delta < 0) delta = 0;
    subscribers = new_subscribers;
    last_Submins = millis();
  }
  if (millis() - last_color > 3000) {
    delta_f = delta_f * K + delta * (1 - K);
    get_color();
    analogWrite(ledR, R);
    analogWrite(ledG, G);
    analogWrite(ledB, B);

    Serial.print("delta: "); Serial.print(delta); Serial.print(", ");
    Serial.print("delta_f: "); Serial.print(delta_f); Serial.print(", ");
    Serial.print("R: "); Serial.print(R); Serial.print(", ");
    Serial.print("G: "); Serial.print(G); Serial.print(", ");
    Serial.print("B: "); Serial.println(B);
    last_color = millis();
  }
}
