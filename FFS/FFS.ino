#include "SPIFFS.h"
//Serial Peripheral Interface Flash Fail System

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open("/hello_spiffs_document.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
void loop (){}
