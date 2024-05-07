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
int tries;                // Quantidade de tentativas falhas seguidas
int maxTries;             // Quantidade máxima de tentativas para bloqueio
int bloq;                 // Tempo de bloqueio

int cards[][5] = {
  // Declara os códigos liberados para acesso
  { 147, 212, 41, 25, 119 },   //0 Daniel
  { 59, 196, 213, 0, 42 },     //1 Davi
  { 218, 108, 92, 161, 75 },   //2 Lucas
  { 231, 159, 219, 43, 136 },  //3 Vinicius
  { 2, 219, 100, 34, 159 },    //4 Eduardo
  { 236, 20, 101, 205, 80 },   //5 Bruno
  { 166, 37, 108, 33, 206 },   //6 Giulia
  { 92, 187, 124, 204, 87 },   //7 Samuel
  { 158, 229, 235, 159, 15 },  //8 Lucas
  { 211, 9, 30, 25, 221 }      //9 Gabriela
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

  access = false;  //define valor padrão de acesso negado
  tries = 0;       //quantidade de tentativas falhas seguidas
  maxTries = 3;    //quantidade máxima de tentativas para bloqueio
  bloq = 5;        //tempo de bloqueio

  if (tries >= maxTries) {  //se qtd de tentativas ultrapassar limite, bloquea tentativas por tempo definido
    lcd.setCursor(0, 0);
    lcd.print(F("Max. Tentativas"));
    lcd.setCursor(0, 1);
    lcd.print(F("Aguarde"));
    lcd.setCursor(14, 1);


    for (int i = bloq; i >= 0; i--) {
      lcd.print(F("i"));
      delay(1000);
    }
    tries = 0;
    lcd.clear();
  }

  else {  //Modo de operação normal

    lcd.setCursor(0, 0);
    lcd.print(F(" Acesso ao LIPS"));
    lcd.setCursor(0, 1);
    lcd.print(F("Aprox. o cartao"));

    if (rfid.isCard()) {  //confere se valor lido é um cartão

      if (rfid.readCardSerial()) {  // salva valor lido se for compativel
        Serial.print(rfid.serNum[0]);
        Serial.print(" ");
        Serial.print(rfid.serNum[1]);
        Serial.print(" ");
        Serial.print(rfid.serNum[2]);
        Serial.print(" ");
        Serial.print(rfid.serNum[3]);
        Serial.print(" ");
        Serial.print(rfid.serNum[4]);
        Serial.println("");  // le serial number

        for (int x = 0; x < sizeof(cards); x++) {          //Loop para procurar no banco se cartão inserido é válido
          for (int i = 0; i < sizeof(rfid.serNum); i++) {  //Confere grupo de caracteres a cada vez
            if (rfid.serNum[i] != cards[x][i]) {
              access = false;
              break;
            } else {
              access = true;
            }
          }
          if (access) break;
        }


        if (access) {  //se cartão for válido

          digitalWrite(solenoide, LOW);
          delay(70);
          digitalWrite(solenoide, HIGH);

          lcd.setCursor(0, 0);
          lcd.print(F("  Bem-Vindo(a) "));
          /*for (int i = 3; i > 0; i--) {         //loop para imprimir numero do cartão, util para cadastrar novo cartão
          lcd.setCursor (0, 1);
          lcd.print(rfid.serNum[0]); lcd.print(rfid.serNum[1]);
          lcd.print(rfid.serNum[2]); lcd.print(rfid.serNum[3]);
          lcd.print(rfid.serNum[4]);
          lcd.setCursor (14, 1); 
          lcd.print(i);
        }*/
          for (int i = 0; i < sizeof(rfid.serNum); i++) {  //loop para imprimir nome
            if (rfid.serNum[i] == cards[0][i]) {
              lcd.setCursor(0, 1);
              lcd.print("     Daniel     ");
              break;
            } else if (rfid.serNum[i] == cards[1][i]) {
              lcd.setCursor(0, 1);
              lcd.print("      Davi      ");
              break;
            } else if (rfid.serNum[i] == cards[2][i]) {
              lcd.setCursor(0, 1);
              lcd.print("     Lucas      ");
              break;
            } else if (rfid.serNum[i] == cards[3][i]) {
              lcd.setCursor(0, 1);
              lcd.print("    Vinicius     ");
              break;
            } else if (rfid.serNum[i] == cards[4][i]) {
              lcd.setCursor(0, 1);
              lcd.print("    Eduardo     ");
              break;
            } else if (rfid.serNum[i] == cards[5][i]) {
              lcd.setCursor(0, 1);
              lcd.print("     Bruno     ");
              break;
            } else if (rfid.serNum[i] == cards[6][i]) {
              lcd.setCursor(0, 1);
              lcd.print("     Giulia     ");
              break;
            } else if (rfid.serNum[i] == cards[7][i]) {
              lcd.setCursor(0, 1);
              lcd.print("     Samuel     ");
              break;
            } else if (rfid.serNum[i] == cards[8][i]) {
              lcd.setCursor(0, 1);
              lcd.print("      Lucas    ");
              break;
            } else if (rfid.serNum[i] == cards[9][i]) {
              lcd.setCursor(0, 1);
              lcd.print("    Gabriela   ");
              break;
              // } else if (rfid.serNum[i] == cards[10][i]) {
              //   lcd.setCursor(0, 1);
              //   lcd.print("    Leonardo   ");
              //   break;
            } else {
              lcd.setCursor(0, 1);
              lcd.print("Usuário desconhecido");
              break;
            }
          }

          delay(3000);
          lcd.clear();
          tries = 0;  //zera quantidade de tentativas falhas
        } else {      //caso acesso negado
          lcd.setCursor(0, 0);
          lcd.print(F("  RFID Negado "));
          for (int i = 3; i > 0; i--) {  //printa serial rejeitado e contagem regressiva no canto inferior direito
            lcd.setCursor(14, 1);
            lcd.print((i));
            delay(1000);
            tries += 1;  //incrementa quantidade de tentativas falhas
          }
          lcd.clear();
          rfid.halt();
        }
      }
    }
  }
}
