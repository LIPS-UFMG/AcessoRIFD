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

int cards[][5] = {                    // Declara os códigos liberados para acesso
  {147, 212, 41, 25, 119},            //Daniel
  {59, 196, 213, 0, 42},              //Davi
  {218, 108, 92, 161, 75},            //Lucas
  {231, 159, 219, 43, 136},           //Vinicius
  {2, 219, 100, 34, 159},             //Eduardo
  {236, 20, 101, 205, 80},            //Bruno
  {166, 37, 108, 33, 206},            //Giulia
  {92, 187, 124, 204, 87},            //Samuel
  {158, 229, 235, 159, 15},           //Lucas
  {211, 9, 30, 25, 221}               //Gabriela
};

bool access = false;

int alarm = 0;
uint8_t alarmStat = 0;
uint8_t maxError = 5;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  rfid.init();                      //configurações iniciais

  pinMode(solenoide, OUTPUT);       //solenoide é a saída para fechadura
  digitalWrite(solenoide, HIGH);

 
  lcd.setCursor (0, 0);             //seta cursor para primeira linha, primeira coluna
  lcd.print(F(" Controle RFID"));
  lcd.setCursor (0, 1);             //seta cursor para segunda linha, primeira coluna
  lcd.print(F("   de Acesso"));     //imprime mensagem inicial
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

    if (rfid.isCard()) {                    //confere se valor lido é um cartão

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
        Serial.println("");                 // le serial number

        for (int x = 0; x < sizeof(cards); x++) {             //Loop para procurar no banco se cartão inserido é válido
          for (int i = 0; i < sizeof(rfid.serNum); i++ ) {    //Confere grupo de caracteres a cada vez
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
 
      if (access) {                            //se cartão for válido

        digitalWrite(solenoide, LOW);
        delay(100);
        digitalWrite(solenoide, HIGH);
        
        lcd.setCursor (0, 0);
        lcd.print(F("  Bem-Vindo(a) "));
        /*for (int i = 3; i > 0; i--) {         //loop para imprimir numero do cartão, util para cadastrar novo cartão
          lcd.setCursor (0, 1);
          lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
          lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
          lcd.print(rfid.serNum[4]);
          lcd.setCursor (14, 1); 
          lcd.print(i);
        }*/            
          for (int i = 0; i < sizeof(rfid.serNum); i++ ) {    //loop para imprimir nome
            if (rfid.serNum[i] == cards[0][i]) {
              lcd.setCursor (0, 1);
              lcd.print("     Daniel     ");
              delay (3000);
              break;
            } else if(rfid.serNum[i] == cards[1][i]) {
              lcd.setCursor (0, 1);
              lcd.print("      Davi      ");
              delay (3000);
              break;
            }
              else if(rfid.serNum[i] == cards[2][i]) {
              lcd.setCursor (0, 1);
              lcd.print("     Lucas      ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[3][i]) {
              lcd.setCursor (0, 1);
              lcd.print("    Vinicius     ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[4][i]) {
              lcd.setCursor (0, 1);
              lcd.print("    Eduardo     ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[5][i]) {
              lcd.setCursor (0, 1);
              lcd.print("     Bruno     ");
              delay (3000); 
              break;
            }
            else if(rfid.serNum[i] == cards[6][i]) {
              lcd.setCursor (0, 1);
              lcd.print("     Giulia     ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[7][i]) {
              lcd.setCursor (0, 1);
              lcd.print("     Samuel     ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[8][i]) {
              lcd.setCursor (0, 1);
              lcd.print("      Lucas    ");
              delay (3000);
              break;
            }
            else if(rfid.serNum[i] == cards[9][i]) {
              lcd.setCursor (0, 1);
              lcd.print("    Gabriela   ");
              delay (3000);
              break;
            }
            else{
              lcd.setCursor (0, 1);
              lcd.print("Usuário desconhecido");
              delay (3000);
              break;
            }
          }
        
        lcd.clear();
      } else {              //caso acesso negado
        alarm = alarm + 1;
        Serial.println("Acesso Negado!");
        lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);       //printa numero do cartão
        lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
        lcd.print(rfid.serNum[4]);
        delay(3000)
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
