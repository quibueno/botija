/*
  Program: Data Sending Program for InfluxDB in the Cloud
  Author: Luiz Quirino
  Email: lugumar@gmail.com
  License: GPL v3
*/
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "HX711.h"
#include <esp_sleep.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define CELULA_DT  17
#define CELULA_SCK  18
#define LED_BUILTIN 2

const char* ssid = "<SSID_NAME>"; # wifi_home
const char* password = "<PASSWORD>"; # password
const char* influxDB_server = "<Influx address>"; # https://us-east-1-1.aws.cloud2.influxdata.com
const uint16_t influxDB_port = 8086;
const char* influxDB_token = "<Influx Token>"; # tko2tFoOlrw90ruw90LJcoXuBDoaAdKJO_d7vczUYdokGRhi1eFNTloXlPQSKX1asdasowe90ruwekuQ==
const char* influxDB_org = "<Influx ORG>"; # 3de9112373ww9c23
const char* influxDB_bucket = "botija"; # name of bucket

HX711 escala;
float fator_calib = -16511.45;
float tara = 0.0;

WiFiClient wifiClient;
WebServer server(80);
unsigned long lastMeasurementTime = 0;
unsigned long lastWiFiCheckTime = 0;
const unsigned long measurementInterval = 1 * 60 * 1000; // 1 minute
const unsigned long wifiCheckInterval = 10 * 60 * 1000; // 10 minutes

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600); // UTC-3

void blinkLED() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void connectToWiFi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
}

void handleRoot() {
  float medida = escala.get_units(10) - tara;
  Serial.print("Leitura: ");
  Serial.print(medida, 2);
  Serial.print(" kg");
  Serial.println();

  HTTPClient http;
  http.begin(wifiClient, String(influxDB_server) + "/api/v2/write?org=" + String(influxDB_org) + "&bucket=" + String(influxDB_bucket) + "&precision=s");
  http.addHeader("Content-Type", "text/plain");
  http.addHeader("Authorization", "Token " + String(influxDB_token));

  String data = "weight,location=botija value=" + String(medida);
  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Erro na solicitação HTTP");
  }

  http.end();

  blinkLED();
}

void handleConfig() {
  if (server.args() > 0) {
    for (uint8_t i = 0; i < server.args(); i++) {
      String argName = server.argName(i);
      String argValue = server.arg(i);

      if (argName.equals("wifi_ssid")) {
        ssid = argValue.c_str();
        Serial.print("SSID configurado: ");
        Serial.println(ssid);
      } else if (argName.equals("wifi_password")) {
        password = argValue.c_str();
        Serial.println("Senha Wi-Fi configurada");
      } else if (argName.equals("tara")) {
        tara = argValue.toFloat();
        Serial.print("Tara configurada: ");
        Serial.println(tara);
        escala.set_offset(tara * fator_calib);
      }
    }

    server.send(200, "text/html", "Configuração aplicada. O ESP32 será reiniciado.");
    delay(2000);
    ESP.restart();
  } else {
    String html = "<html><body>";
    html += "<h1>Configurações do ESP32</h1>";
    html += "<form action='/config' method='POST'>";
    html += "SSID Wi-Fi: <input type='text' name='wifi_ssid'><br>";
    html += "Senha Wi-Fi: <input type='password' name='wifi_password'><br>";
    html += "Tara: <input type='text' name='tara'><br>";
    html += "<input type='submit' value='Salvar'>";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Rotina de medida com a célula de carga.");
  pinMode(LED_BUILTIN, OUTPUT);
  escala.begin(CELULA_DT, CELULA_SCK);
  connectToWiFi();
  escala.set_scale(fator_calib);
  escala.tare();
  escala.set_offset(tara * fator_calib);

  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.begin();

  timeClient.begin();
  timeClient.update();

  // Obter o horário atual
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter a hora atual");
  } else {
    int currentHour = timeinfo.tm_hour;
    // Configurar o sono profundo se estiver entre as 21h e as 9h
    if (currentHour >= 21 || currentHour < 9) {
      // Configurar o tempo de sono profundo para a próxima ocorrência de 9h
      time_t next9AM = (timeinfo.tm_hour < 9) ? mktime(&timeinfo) : mktime(&timeinfo) + 24 * 3600;
      time_t sleepTime = next9AM - time(NULL);
      esp_sleep_enable_timer_wakeup(sleepTime * 1000000); // Converter para microssegundos
      Serial.println("Entrando no modo de sono profundo até as 9h");
      esp_deep_sleep_start();
    }
  }
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();

  // Verificar a conexão Wi-Fi a cada wifiCheckInterval
  if (currentMillis - lastWiFiCheckTime >= wifiCheckInterval) {
    lastWiFiCheckTime = currentMillis;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wi-Fi desconectado! Reiniciando...");
      ESP.restart();
    }
  }

  // Atualizar o horário a cada hora
  if (currentMillis - lastMeasurementTime >= 60 * 60 * 1000) {
    lastMeasurementTime = currentMillis;
    timeClient.forceUpdate();
  }

  // Enviar os dados a cada measurementInterval
  if (currentMillis - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentMillis;
    handleRoot();
  }
}
