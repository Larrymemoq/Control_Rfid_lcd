#include <MFRC522.h>
#include<LiquidCrystal.h>
#include <SPI.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); //inicializacion pines de comunicacion RC522

MFRC522::MIFARE_Key key;

//tarjetas dadas de atra en hexadecimal 
byte tag1[5] = {0x13, 0x57, 0x22, 0xAA}; 
byte tag2[5] = {0xF3, 0x54, 0x01, 0xAA};
byte tag3[5] = {0x03, 0x8C, 0xA1, 0xA9};

byte tagActual[5];

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.print("Iniciando control_calidad_2");
  lcd.print("xxxxxxxxxxxx");//mientras carga lo pirmero en mostrar 
  SPI.begin(); // inicia la comunicacion spi
  rfid.PCD_Init(); // Inicia la comunicacion con rc522
  pinMode(8, OUTPUT);//led rojo
  pinMode(A0, OUTPUT);//relay 
  pinMode(A1, OUTPUT);//buzzer




}

void loop() {
  lcd.print("XXXXXXXXXXXXXXX");//PRIMER LINEA MENSAJE ESTATICO 
  lcd.setCursor(0, 1);
  lcd.print("xxxxxx");//SEGUNDA LINEA MENSAJE ESTATICO 
  if ( ! rfid.PICC_IsNewCardPresent())//lee si hay tarjeta presente 
    return;
  if( ! rfid.PICC_ReadCardSerial())       // leer la tarjeta 
        return;
        lcd.clear();
        Serial.print(F("Card UID:"));
        dump_byte_array(rfid.uid.uidByte, rfid.uid.size);   // obtiene los valores leidos de la tarjeta 

}

void dump_byte_array(byte *buffer, byte bufferSize) {      // realiza la lectura del codigo de la tarjeta y comprueba 
    for (byte i = 0; i < bufferSize; i++) {         // extrae valores del codigo 
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
      tagActual[i] = buffer[i];
    }
         if(compararArray(tagActual,tag1)){              // comprobamos el acceso para el primer usuario 
           Serial.println("\nI know this card!");
           Serial.println("XXXXXXXX");//PUERTO SERIAL MENSAJE APROBADO 
           digitalWrite(A1,HIGH);
           delay(100);
           digitalWrite(A1,LOW);
           lcd.clear();
           lcd.print("XXXXXXXXXX");//PRIMERA LINEA MENSAJE APROBADO 
           printUnlockMessage();
         }
         else if(compararArray(tagActual,tag2)){          // comprobamos el acceso para el segundo usuario 
           Serial.println("\nRECONOZCO ESTA TARJETA!");// MEMSAJE SERIAL DE RECONOCIMIENTO DE LA TARJETA 
           Serial.println("XXXXXXXXXXXXXXX"); //PUERTO SERIAL MENSAJE APROBADO 
           digitalWrite(A1,HIGH);
           delay(100);
           digitalWrite(A1,LOW);
           lcd.clear();
           lcd.print("XXXXXXXXXXXXXX");//PRIMERA LINEA MENSAJE APROBAD
           printUnlockMessage();
         }
         else if(compararArray(tagActual,tag3)){    // comprobamos el acceso para el TERCER usuario 
           Serial.println("\nRECONOZCO ESTA TARJETA!");
           Serial.println("XXXXXXXXXXXXX");  //PUERTO SERIAL MENSAJE APROBADO 
           digitalWrite(A1,HIGH);
           delay(100);
           digitalWrite(A1,LOW);
           lcd.clear();
           lcd.print("XXXXXXXXXXX");//PRIMERA LINEA MENSAJE APROBAD
           printUnlockMessage();
         }
          else {
            lcd.clear();
            lcd.print("TARJETA");//PRIMERA LINEA MENSAJE RECHAZADO 
            lcd.setCursor(0, 1);
            //AREGLO PARA SONAR EL BUZZER
            digitalWrite(A1,HIGH);
            delay(100);
            digitalWrite(A1,LOW);
            delay(100);
            digitalWrite(A1,HIGH);
            delay(100);
            digitalWrite(A1,LOW);
            //TERMINA EL BUZZER
            digitalWrite(8, HIGH);//ENCIENDE EL LED ROJO 
            lcd.print("DESCONOCIDA");//SEGUNDA LINEA MENSAJE RECHAZADO
            delay(2000);
            digitalWrite(8, LOW);//APAGA EL LED ROJO 
            lcd.clear();
            Serial.println("\nTARJETA DESCONOCIDA ");//PUERTO SERIAL MENSAJE RECHAZADO
            }       
}
// Funcion que manda el buen mensaje 
void printUnlockMessage()
{
  lcd.setCursor(0, 1);
  lcd.print("DESBLOQUEADO");//SEGUNDA LINEA MENSAJE APROBAD
  digitalWrite(A0, HIGH);// ACTIVA EL RELAY PARA MANDAR EL PULSO 
  delay(5000);
  lcd.clear();
  lcd.print("xxxxxxxxxxx");//SEGUNDA LINEA VUELVE A SU ESTADO NORMAL 
  digitalWrite(A0, LOW);//TERMINA EL PULSO DEL RELAY 
}
//COMARACION DE ARREGLOS HEXADECIMALES PARA LAS TARJETAS 
boolean compararArray(byte array1[], byte array2[]){    // comprobara codigo leido con el registrado previamente 

  if(array1[0] != array2[0]) return(false);
  if(array1[1] != array2[1]) return(false);
  if(array1[2] != array2[2]) return(false);
  if(array1[3] != array2[3]) return(false);
  if(array1[4] != array2[4]) return(false);
  
  return(true);
}
