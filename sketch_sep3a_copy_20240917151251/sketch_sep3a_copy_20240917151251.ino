//Grupo 7 Schatzyki, Dubovsky, guilman, rivadineira
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// Pines
#define BOT1 35
#define BOT2 34
#define SENSOR_TEMP 23
#define LED 25

// Variables

float temperatura;
unsigned long Time = 0;
int tiempocalculo;
int tiempomuestro;

int maquina;

DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

#define MODO1 0
#define MODO2 1
#define INTERVALO1 2
#define INTERVALO2 3
#define INTERVALO3 4
#define INTERVALO4 5



// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "sacho"
#define WIFI_PASSWORD "123456789"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAOp9NLfppQOqJG8q1CpFkm2D2L2eGLdEI"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "matischatzyki@gmail.com"
#define USER_PASSWORD "Sacho101"

// Insert RTDB URL
#define DATABASE_URL "https://tp-firebase-5a936-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// Variable to save USER UID
String uid;
// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/temperature";
// Parent Node (to be updated in every loop)
String parentPath;
int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

unsigned long lastTimeBotRan = 0;



// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }
  time(&now);
  return now;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();

  configTime(0, 0, ntpServer);

  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the user sign-in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long-running token generation task
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // maquinaialize the library with the Firebase authentication and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";

  // maquinaial setup
  tiempocalculo = 30000;
  pinMode(BOT1, INPUT_PULLUP);
  pinMode(BOT2, INPUT_PULLUP);
  u8g2.begin();
  dht.begin();
  maquina = MODO1;
}

void loop() {
  Serial.println(millis() - lastTimeBotRan);
  tiempomuestro = tiempocalculo / 1000;

  if (Firebase.ready() && (millis() - lastTimeBotRan > tiempocalculo || lastTimeBotRan == 0)) {
    lastTimeBotRan = millis();

    // Get current timestamp
    timestamp = getTime();
    Serial.print("time: ");
    Serial.println(timestamp);

    parentPath = databasePath + "/" + String(timestamp);


    json.set(tempPath.c_str(), dht.readTemperature());
    Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
  }

  switch (maquina) {
    case MODO1:
      Serial.println("MODO1");
      displayTempUmbral();
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW)
        maquina = INTERVALO1;
      break;
    case INTERVALO1:
      Serial.println("1");
      displayTempUmbral();
      if (digitalRead(BOT1) == HIGH && digitalRead(BOT2) == HIGH) {
        maquina = MODO2;
      }
      break;
    case MODO2:
      Serial.println("MODO2");
      displayUmbral();
      if (digitalRead(BOT1) == LOW) {
        maquina = INTERVALO2;
      }
      if (digitalRead(BOT2) == LOW) {
        maquina = INTERVALO3;
      }
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        maquina = INTERVALO4;
      }

      break;
    case INTERVALO2:
      Serial.println("2");
      displayUmbral();
      if (digitalRead(BOT1) == HIGH) {
        tiempocalculo = tiempocalculo + 30000;
        maquina = MODO2;
      }
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        maquina = INTERVALO4;
      }
      break;
    case INTERVALO3:
      Serial.println("3");
      displayUmbral();
      if (digitalRead(BOT2) == HIGH) {
        maquina = MODO2;
        if (tiempocalculo > 30000) {
          tiempocalculo = tiempocalculo - 30000;
        }
      }

      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        maquina = INTERVALO4;
      }
      break;
    case INTERVALO4:
      Serial.println("4");
      displayUmbral();
      if (digitalRead(BOT1) == HIGH && digitalRead(BOT2) == HIGH) {
        maquina = MODO1;
      }
      break;
  }
}

void displayUmbral() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(18, 30);
  u8g2.print(tiempomuestro);
  u8g2.drawStr(55, 30, "Time");
  u8g2.drawStr(15, 50, "-");
  u8g2.drawStr(100, 50, "+");

  u8g2.sendBuffer();
}

void displayTempUmbral() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 15, "TP FIREBASE");

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 30, "Temperatura: ");
  u8g2.setCursor(80, 30);
  temperatura = dht.readTemperature();  // Actualiza la temperatura
  u8g2.print(temperatura);
  u8g2.drawStr(110, 30, "°C");

  u8g2.sendBuffer();
}