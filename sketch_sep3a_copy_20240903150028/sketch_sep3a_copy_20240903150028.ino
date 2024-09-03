//Grupo 1 Eitan Cherniak, Eitan Grodniensky, Martin Feierman Y Alan Tejblum
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
int umb;
float temp;
unsigned long Time = 0;
int Intervalo;
int Intervalito;

int Maquina;

DHT dht(SENSOR_TEMP, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#define  Modo1 1
#define Intervalo1 2
#define Modo2 3
#define Subir 4
#define Bajar 5
#define Intervalo2 6


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "ORT-IoT"
#define WIFI_PASSWORD "OrtIOT24"

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

int Tiemposubida;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

unsigned long PrevMillis = 0;
unsigned long timerDelay = 180000;

void Modo1WiFi() { //cambiar
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long ObtenerTiempo() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("error con el tiempo");
    return 0;
  }
  time(&now);
  return now;
}

void setup() {
  Serial.begin(115200);
  Modo1WiFi();
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

  // Modo1ialize the library with the Firebase authentication and config
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

  // Modo1ial setup
  Intervalo = 30000;
  pinMode(BOT1, INPUT_PULLUP);
  pinMode(BOT2, INPUT_PULLUP);
  u8g2.begin();
  dht.begin();
  Maquina = Modo1;
}

void loop() {
  Serial.println(millis() - PrevMillis);
  Intervalito = Intervalo / 1000;

  if (Firebase.ready() && (millis() - PrevMillis > Intervalo || PrevMillis == 0)) {
    PrevMillis = millis();

    // Get current Tiemposubida
    Tiemposubida = ObtenerTiempo();
    Serial.print("time: ");
    Serial.println(Tiemposubida);

    parentPath = databasePath + "/" + String(Tiemposubida);


    json.set(tempPath.c_str(), dht.readTemperature());
    Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
  }

  switch (Maquina) {
    case Modo1:
      Pantalla_Modo1();
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW)
        Maquina = Intervalo1;
      break;
    case Intervalo1:
      Pantalla_Modo1();
      if (digitalRead(BOT1) == HIGH && digitalRead(BOT2) == HIGH) {
        Maquina = Modo2;
      }
      break;
    case Modo2:
      Pantalla_Tiempo();
      if (digitalRead(BOT1) == LOW) {
        Maquina = Subir;
      }
      if (digitalRead(BOT2) == LOW) {
        Maquina = Bajar;
      }
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        Maquina = Intervalo2;
      }

      break;
    case Subir:
      Pantalla_Tiempo();
      if (digitalRead(BOT1) == HIGH) {
        Intervalo = Intervalo + 30000;
        Maquina = Modo2;
      }
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        Maquina = Intervalo2;
      }
      break;
    case Bajar:
      Pantalla_Tiempo();
      if (digitalRead(BOT2) == HIGH) {
        Maquina = Modo2;
        if (Intervalo   > 30000) {
          Intervalo = Intervalo - 30000;
        }
      }

      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == LOW) {
        Maquina = Intervalo2;
      }
      break;
    case Intervalo2:
      Pantalla_Tiempo();
      if (digitalRead(BOT1) == HIGH && digitalRead(BOT2) == HIGH) {
        Maquina = Modo1;
      }
      break;
  }
}

void Pantalla_Tiempo() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(18, 30);
  u8g2.print(Intervalito);
  u8g2.drawStr(55, 30, "Time");
  u8g2.drawStr(15, 50, "-");
  u8g2.drawStr(100, 50, "+");

  u8g2.sendBuffer();
}

void Pantalla_Modo1() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(0, 15, "TP FIREBASE");

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 30, "Temperatura: ");
  u8g2.setCursor(80, 30);
  temp = dht.readTemperature();  // Actualiza la temperatura
  u8g2.print(temp);
  u8g2.drawStr(110, 30, "°C");

  u8g2.sendBuffer();
}