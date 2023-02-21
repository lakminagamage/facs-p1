//LIBRARY ADDITION HERE
#include <SPI.h>
#include <rdm6300.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>



//PIN DEFINITIONS HERE
#define RC522_RST_PIN        22
#define RC522_SS_PIN          21 
#define RDM6300_RX_PIN        5

//VARIABLES AND CONSTANTS DEFINITION HERE
unsigned long uid = 0;
const char* ssid     = "Among_Us";
const char* password = "lakmina2055176";
const char* serverName = "http://192.168.8.142:80/post-esp-data.php";
String apiKeyValue = "testapikey";


//PIN DEFINITIONS
MFRC522 mfrc522(RC522_SS_PIN, RC522_RST_PIN);
Rdm6300 rdm6300;




void setup() {

  //LIBRARY INITIALTIONS
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4); //added this delay cus mc is slower than we expected. It takes some time to start the initiation
  rdm6300.begin(RDM6300_RX_PIN);

  //wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected. IP Address: ");
  Serial.println(WiFi.localIP());


  boolean multipleRead = false;




}


unsigned long getID522(){
  if ( ! mfrc522.PICC_ReadCardSerial()) { 
    return 0;
  }
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA();
  return hex_num;
  }

void loop() {
  
  if (!rdm6300.get_new_tag_id()){
      delay(1000);
      uid = rdm6300.get_tag_id();
      delay(1000);
    }
  delay(30);

  if(mfrc522.PICC_IsNewCardPresent()) {
      uid = getID522();
  
  }
  delay(30);
  

  if(uid != 0){
      Serial.println(uid);
      if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "api_key=" + apiKeyValue + "&id=" + uid + "&name=" + uid;
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    uid = 0;
    delay(30);
  }
  
      
















}
