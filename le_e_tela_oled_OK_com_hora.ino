#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C  // Endereço padrão para muitas telas OLED SSD1306

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Replace with your network credentials
const char* ssid = "florenca";
const char* password = "beneecuca";

const char* serverName = "https://davidpezzin.com.br/dados_arduino.php";

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing..."));
  display.display();
  display.println(F("Conectando a florenca"));
  display.display();
    display.println(F("status:Desconectado"));
  display.display();
      display.println(F("ip´:192.168.4.1"));
  display.display();
        display.println(F("Timeout:5segundos"));
  display.display();


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { // Check Wi-Fi connection
    Serial.println(" -1- ");
     display.println(F("Conectado, Aguardando leitura"));
  display.display();
  delay(1000);
    //WiFiClient client;
      //client.setInsecure();
   //HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;
    
    https.begin(client, serverName); // Use client with URL
    int httpCode = https.GET(); // Make GET request

    if (httpCode > 0) { // Check for successful response
      String payload = https.getString();
      Serial.println("Payload: " + payload);

      Serial.println(" -3- ");

      // Parse the data
      String values[7];
      int index = 0;
      int startIndex = 0;

      for (int i = 0; i < payload.length(); i++) {
        if (payload[i] == ',' || i == payload.length() - 1) {
          if (i == payload.length() - 1) i++;
          values[index] = payload.substring(startIndex, i);
          startIndex = i + 1;
          index++;
        }
      }

      // Display data
      display.clearDisplay();
      display.setCursor(0, 0); 
      display.println(values[7]);
      display.print("TempAgua: ");
      display.println(values[0]);
      display.print("UmidadeCano: ");
      display.println(values[2]);
      display.print("TempInterna: ");
      display.println(values[1]);
      display.print("UmidInterna: ");
      display.println(values[3]);
      display.print("TempExterna: ");
      display.println(values[5]);
      display.print("UmiExteterna: ");
      display.println(values[6]);
      display.display();
      display.print("Luminosidade: ");
      display.println(values[4]);
     
      display.display();
    } else {
      Serial.println("Error on HTTP request");
    }

    https.end(); // End HTTP request
  }
  Serial.println(" -2- ");
  delay(30000); // Wait for 60 seconds before next request
}
