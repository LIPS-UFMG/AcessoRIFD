// CONTROLE DE ACESSO RFID COM TRAVA ELÉTRICA
// USINAINFO < https://www.usinainfo.com.br >
// FACEBOOK  < https://www.facebook.com/usinainfo.arduino >
// YOUTUBE   < https://www.youtube.com/usinainfo >
// INSTAGRAM < @usinainfo >

#include <SPI.h>            // Inclui a biblioteca SPI.h
#include <RFID.h>           // Inclui a biblioteca RFID.h
#include <Wire.h>           // Inclui a biblioteca Wire.h
#include <LiquidCrystal.h>  // Inclui a biblioteca LiquidCrystal.h

#define SS_PIN 10  // Define os pinos do RFID
#define RST_PIN 9  // Define os pinos do RFID

RFID rfid(SS_PIN, RST_PIN);  //Iniciliza as configurações da biblioteca RFID

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);  // Declara os pinos do display

const int solenoide = 8;  // Declara pino da fechadura elétrica
int serNum[5];            // Variável de leitura da tag
bool access = false;      // Define valor padrão de acesso negado
int tries = 0;            // Quantidade de tentativas falhas seguidas
int maxTries = 3;         // Quantidade máxima de tentativas para bloqueio
int bloq = 10;            // Tempo de bloqueio

int cards[][5] = {
  // Declara os códigos liberados para acesso
  { 2, 219, 100, 34, 159 },   //0 Eduardo
  { 59, 196, 213, 0, 42 },    //1 Davi
  { 115, 176, 118, 8, 189 },  //2 Sebastiao

  { 147, 212, 41, 25, 119 },   //3 Daniel
  { 218, 108, 92, 161, 75 },   //4 Lucas
  { 231, 159, 219, 43, 136 },  //5 Vinicius
  { 236, 20, 101, 205, 80 },   //6 Bruno
  { 166, 37, 108, 33, 206 },   //7 Giulia
  { 92, 187, 124, 204, 87 },   //8 Samuel

  { 158, 229, 235, 159, 15 },  //9 Lucas
  { 211, 9, 30, 25, 221 },     //10 Gabriela
  { 137, 77, 78, 89, 211 },    //11 Leonardo
  { 131, 213, 11, 25, 68 },    //12 Jessica
  { 121, 39, 138, 90, 142 }    //13 Joao

};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  rfid.init();  //configurações iniciais

  pinMode(solenoide, OUTPUT);  //solenoide é a saída para fechadura
  digitalWrite(solenoide, HIGH);

  lcd.setCursor(0, 0);  //seta cursor para primeira linha, primeira coluna
  lcd.print(F(" Controle RFID"));
  lcd.setCursor(0, 1);           //seta cursor para segunda linha, primeira coluna
  lcd.print(F("   de Acesso"));  //imprime mensagem inicial
  delay(2000);
  lcd.clear();
}



void loop() {

  lcd.setCursor(0, 0);
  lcd.print(F("Acesso ao LIPS!"));
  lcd.setCursor(0, 1);
  lcd.print(F("Aprox. o cartao"));
  delay(1500);

  if (rfid.isCard()) {  //loop principal, lê se for cartão

    if (rfid.readCardSerial()) {  // salva valor lido
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println("");                                // le serial number
      for (int i = 0; i < sizeof(cards); i++) {          //Loop para procurar no banco se cartão tem acesso
        for (int j = 0; j < sizeof(rfid.serNum); j++) {  //Confere grupo de caracteres a cada vez
          if (rfid.serNum[j] != cards[i][j]) {
            access = false;
            break;
          } else {
            access = true;
          }
        }
        if (access) break;
      }

      if (access) {  //se cartão tiver acesso
        digitalWrite(solenoide, LOW);
        delay(70);
        digitalWrite(solenoide, HIGH);

        lcd.setCursor(0, 0);
        lcd.print(F("  Bem-Vindo(a) "));
        for (int i = 0; i < sizeof(rfid.serNum); i++) {  //loop para imprimir nome
          if (rfid.serNum[i] == cards[0][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    Eduardo     ");
            break;
          } else if (rfid.serNum[i] == cards[1][i]) {
            lcd.setCursor(0, 1);
            lcd.print("      Davi      ");
            break;
          } else if (rfid.serNum[i] == cards[2][i]) {
            lcd.setCursor(0, 1);
            lcd.print("   Sebastião   ");
            break;
          } else if (rfid.serNum[i] == cards[3][i]) {
            lcd.setCursor(0, 1);
            lcd.print("     Daniel     ");
            break;
          } else if (rfid.serNum[i] == cards[4][i]) {
            lcd.setCursor(0, 1);
            lcd.print("     Lucas      ");
            break;
          } else if (rfid.serNum[i] == cards[5][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    Vinicius     ");
            break;
          } else if (rfid.serNum[i] == cards[6][i]) {
            lcd.setCursor(0, 1);
            lcd.print("     Bruno     ");
            break;
          } else if (rfid.serNum[i] == cards[7][i]) {
            lcd.setCursor(0, 1);
            lcd.print("     Giulia     ");
            break;
          } else if (rfid.serNum[i] == cards[8][i]) {
            lcd.setCursor(0, 1);
            lcd.print("     Samuel     ");
            break;
          } else if (rfid.serNum[i] == cards[9][i]) {
            lcd.setCursor(0, 1);
            lcd.print("      Lucas    ");
            break;
          } else if (rfid.serNum[i] == cards[10][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    Gabriela   ");
            break;
          } else if (rfid.serNum[i] == cards[11][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    Leonardo   ");
            break;
          } else if (rfid.serNum[i] == cards[12][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    Jessica   ");
            break;
          } else if (rfid.serNum[i] == cards[13][i]) {
            lcd.setCursor(0, 1);
            lcd.print("    João   ");
            break;
          } else {
            lcd.setCursor(0, 1);
            lcd.print("    Mistério   ");
            break;
          }
        }
        delay(3000);
        lcd.clear();
        tries = 0;  //zera quantidade de tentativas falhas

      } else {  //caso acesso negado
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("  Acesso Negado  "));
        tries += 1;                    //incrementa quantidade de tentativas falhas
        for (int i = 3; i > 0; i--) {  //printa serial rejeitado e contagem regressiva no canto inferior direito
          lcd.setCursor(0, 0);
          lcd.print(rfid.serNum[0]);
          lcd.print(F(" "));
          lcd.print(rfid.serNum[1]);
          lcd.print(F(" "));
          lcd.print(rfid.serNum[2]);
          lcd.print(F(" "));
          lcd.setCursor(0, 1);
          lcd.print(rfid.serNum[3]);
          lcd.print(F(" "));
          lcd.print(rfid.serNum[4]);
          lcd.print(F(" "));
          //lcd.setCursor(7,1);
          lcd.setCursor(14, 1);
          lcd.print(i);
          delay(1000);
        }
        lcd.clear();
        rfid.halt();
      }
    }
  } else if (tries >= maxTries) {  //se qtd de tentativas ultrapassar limite, bloquea tentativas por tempo definido
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Lim. Tentativas"));
    lcd.setCursor(0, 1);
    lcd.print(F("Aguarde"));
    lcd.setCursor(14, 1);

    for (int i = bloq; i >= 0; i--) {  // loop de bloqueio
      lcd.print(F("i"));
      delay(1000);
    }
    tries = 0;
    lcd.clear();
  }
}
