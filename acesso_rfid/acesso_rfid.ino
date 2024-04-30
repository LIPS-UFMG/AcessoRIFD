// CONTROLE DE ACESSO RFID COM TRAVA ELÉTRICA
// USINAINFO < https://www.usinainfo.com.br >
// FACEBOOK  < https://www.facebook.com/usinainfo.arduino >
// YOUTUBE   < https://www.youtube.com/usinainfo >
// INSTAGRAM < @usinainfo >

#include <SPI.h>                 // Inclui a biblioteca SPI.h
 #include <RFID.h>                // Inclui a biblioteca RFID.h
#include <Wire.h>                // Inclui a biblioteca Wire.h
#include <LiquidCrystal.h>       // Inclui a biblioteca LiquidCrystal.h

#define SS_PIN 10                       // Define os pinos do RFID
#define RST_PIN 9                       // Define os pinos do RFID

RFID rfid(SS_PIN, RST_PIN);             //Iniciliza as configurações da biblioteca RFID

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);    // Declara os pinos do display

const int solenoide = 8;                // Declara pino da fechadura elétrica
int serNum[5];                          // Variável de leitura da tag

int cards[][5] = {                      // Declara os códigos liberados para acesso
  {147, 212, 41, 25, 119},            // Tag de Acesso 1
  {240, 193, 247, 54, 240},             // Tag de Acesso 2
  {131, 95, 107, 154, 45},
  {236, 20, 101, 205, 80},
  {59, 196, 213, 0, 42}              // Tag de Acesso Bruno
};

bool access = false;

int alarm = 0;
uint8_t alarmStat = 0;
uint8_t maxError = 5;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  rfid.init();

  pinMode(solenoide, OUTPUT);
  digitalWrite(solenoide, LOW);

 
  lcd.setCursor (0, 0);
  lcd.print(F(" Controle RFID"));
  lcd.setCursor (0, 1);
  lcd.print(F("   de Acesso"));
  delay (1000);
  lcd.clear();
  
}

void loop() {
  if (alarm >= maxError) {
    alarmStat = 1;
  }

  if (alarmStat == 0) {
    lcd.setCursor (0, 0);
    lcd.print(F(" Acesso ao LIPS"));
    lcd.setCursor (0, 1);
    lcd.print(F("Aprox. o cartao"));

    if (rfid.isCard()) {

      if (rfid.readCardSerial()) {
        Serial.print(rfid.serNum[0]);
        Serial.print(" ");
        Serial.print(rfid.serNum[1]);
        Serial.print(" ");
        Serial.print(rfid.serNum[2]);
        Serial.print(" ");
        Serial.print(rfid.serNum[3]);
        Serial.print(" ");
        Serial.print(rfid.serNum[4]);
        Serial.println("");

        for (int x = 0; x < sizeof(cards); x++) {             //Loop para procurar no banco se cartão inserido é válido
          for (int i = 0; i < sizeof(rfid.serNum); i++ ) {    
            if (rfid.serNum[i] != cards[x][i]) {
              access = false;
              break;
            } else {
              access = true;
            }
          }
          if (access) break;
        }
      }

      if (access) {
        Serial.println("Bem-Vindo(a)!"); 
        lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
        lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
        lcd.print(rfid.serNum[4]);
        digitalWrite(solenoide, HIGH);
        lcd.setCursor (0, 0);
        lcd.print(F("  Bem-Vindo(a) "));
        lcd.setCursor (0, 1);
        lcd.print(F("                "));
        for (int i = 3; i > 0; i--) {
          lcd.setCursor (0, 1);
          lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
          lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
          lcd.print(rfid.serNum[4]);
          lcd.setCursor (14, 1); 
          lcd.print(i);
          delay (1000);
        }
        digitalWrite(solenoide, LOW);
        lcd.clear();
      } else {
        alarm = alarm + 1;
        Serial.println("Acesso Negado!");
        lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
        lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
        lcd.print(rfid.serNum[4]);
        lcd.clear();
      }
    }
    rfid.halt();
  }
  else {
    lcd.setCursor (0, 0);
    lcd.print(F("  Acesso Negado "));
    lcd.setCursor (0, 1);
    lcd.print(F("                "));
    for (int i = 3; i > 0; i--) {
      lcd.setCursor (0, 1);
      lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
      lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
      lcd.print(rfid.serNum[4]);
      lcd.setCursor (14, 1); lcd.print((i));
      lcd.print(F("  ")); delay (1000);
    }
    alarmStat = 0;
    alarm = 0;
  }
}
