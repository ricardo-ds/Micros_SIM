/* 
* SensorBMP180.ino
* Julio C. Sandria Reynoso - 23-Mayo-2017
* Este programa toma la Temperatura y Presión de un sensor
* BMP180 conectado a Arduino UNO
* Esta basado en el Tutorial sensor de presion barometrica
* http://www.naylampmechatronics.com/blog/43_Tutorial-sensor-de-presi%C3%B3n-barom%C3%A9trica-BMP180.html
*/
 
#include <SFE_BMP180.h>
#include <Wire.h>
 
SFE_BMP180 bmp180;
char statusBmp180;
double P,T,A,PNivelMar=1013.25; //presion sobre el nivel del mar en mbar
 
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando programa...");
  delay(1000);
 
  if (bmp180.begin()) {
    Serial.println("Sensor BMP180 iniciado correctamente");
    Serial.println();
    Serial.println("  P mbar = Presion barometrica en milibares");
    Serial.println("  T *C = Temperatura en grados Celcius");
    Serial.println("  A msnm = Altura en metros sobre el nivel del mar");
    Serial.println();
  } else {
    Serial.println("Error al iniciar sensor BMP180");
  }
  Serial.println("P mbar\tT *C\tA msnm");
}
 
void loop() {
  delay(1000);
  if( leerBmp180() ) {
    Serial.print(P,2);
    Serial.print("\t");
    Serial.print(T,2);
    Serial.print("\t");
    Serial.println(A,2);
  } else
    Serial.println("pp.pp\ttt.tt\taa.aa?");
}
 
boolean leerBmp180() {
  //Inicio de lectura de temperatura
  statusBmp180 = bmp180.startTemperature(); 
  if (statusBmp180 != 0) {
    //Pausa para que finalice la lectura
    delay(statusBmp180); 
    //Obtener la temperatura
    statusBmp180 = bmp180.getTemperature(T); 
    if (statusBmp180 != 0) {
      // Inicio lectura de presión
      statusBmp180 = bmp180.startPressure(3); 
      if (statusBmp180 != 0) {
        //Pausa para que finalice la lectura
        delay(statusBmp180); 
        //Obtenemos la presión
        statusBmp180 = bmp180.getPressure(P,T); 
        if (statusBmp180 != 0) {
          A = bmp180.altitude(P,PNivelMar);
          return true; // Se tomo lectura de Temperatura, Presion y Altitud
        }      
      }      
    }   
  }
  return false; // No se tomo lectura de Temperatura, Presion y Altitud
} // leerBmp180
