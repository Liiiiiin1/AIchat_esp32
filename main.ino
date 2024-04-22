#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "TTS.h"

int flag=1;
int buttonPin =2;
int dataPin =1;
int dinPin = 3;
int startPin= 4;
// 1. Replace with your network credentials
TTSModule ttsModule;
const char* ssid = "2101-2";
const char* password = "dsj-2101";



String inputText = "你好";
// 2. Replace with your access_token
String apiUrl = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=24.fc7eaac1e44b29b1e71a366a2c88271a.2592000.1713419726.282335-57109160";

String answer;
String getGPTAnswer(String inputText) {
  HTTPClient http;
  http.begin(apiUrl);
  http.setTimeout(30000);     //超过时间，回答超时（ms）
  http.addHeader("Content-Type", "application/json");
  // http.addHeader("access_token", String(access_token));
  String payload = "{\"messages\":[{\"role\": \"user\",\"content\": \"" + inputText + "\"}],\"disable_search\": false,\"enable_citation\": false}";
  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) {
    String response = http.getString();
    http.end();
    Serial.println(response);
   
    // Parse JSON response
    DynamicJsonDocument jsonDoc(2048);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["result"];
    return outputText;
    // Serial.println(outputText);
  } else {
    http.end();
    Serial.printf("Error %i \n", httpResponseCode);
    return "<error>";
  }
}


void do2(){
  delay(1000);
  ttsModule.speak("还有5分钟该下班了");
  return ;
}


void do3(){
  delay(1000);
  ttsModule.speak("收到来访信息，已发往邮箱");
  return ;
}


void setup() {
  
  // Initialize Serial
  Serial.begin(115200);
  pinMode(buttonPin,INPUT);
  pinMode(dataPin,INPUT);
  pinMode(dinPin,INPUT);
  pinMode(startPin,INPUT);
  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  answer = getGPTAnswer(inputText);
  Serial.println("Answer: " + answer);
  Serial.println("Enter a prompt:");
  ttsModule.init();
}

void loop() {
        int buttonState = 0;
      int dataState = 0;
      int dinState = 1;
      int faceState = 0;// do nothing
  
  if(digitalRead(startPin)==HIGH){

      buttonState = digitalRead(buttonPin);
      dataState = digitalRead(dataPin);
      dinState = digitalRead(dinPin);
      
      if(buttonState == HIGH){
        answer = getGPTAnswer("北京今日天气");
        String subString = answer.substring(0, 140);
        delay(1000);
      ttsModule.speak(subString);
      }   
    if(dataState == HIGH){
      do2();
      }
    if(dinState == LOW){
      do3();
     }   
  }
}
