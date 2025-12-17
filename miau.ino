#include "rgb_lcd.h"
//#include <Servo.h>
#include <EEPROM.h>

//pinos
#define PIN_SWITCH 23

//#define SERVO_PIN 5
#define IR_PIN_5v A0
#define IR_PIN_10 A1
#define IR_PIN_25v A3
#define IR_PIN_25 A4
#define IR_PIN_50 A6
#define IR_PIN_1 A5
#define IR_PIN_5 A8


//variaveis
double dinheiros = 0.00;
double ultimoValorLCD = -1;
int cinco = 0;
int dez = 0;
int vinteCinco = 0;
int cinquenta = 0;
int um = 0;
int irValue = 0;
boolean bocaFechada = false;
int contador = 0;

//debounce IR
bool IR5 = false;
bool IR10 = false;
bool IR25 = false;
bool IR50 = false;
bool IR1 = false;
bool IR25v = false;
bool IR5v = false;
int leitura5;
int leitura10;
int leitura25;
int leitura50;
int leitura1;
int leitura5v;
int leitura25v;

//lcd
rgb_lcd lcd;
int colorR = 255;
int colorG = 0;
int colorB = 0;

//Servo myservo;

void setup() {
  Serial.begin(9600);
  
  // Recupera valores guardados na memória
  //EEPROM.get(0, dinheiros);
  // EEPROM.get(8, cinco);
  // EEPROM.get(10, dez);
  // EEPROM.get(12, vinteCinco);
  // EEPROM.get(14, cinquenta);
  // EEPROM.get(16, um);

  // Seta os pinos
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  pinMode(IR_PIN_5, INPUT);
  pinMode(IR_PIN_10, INPUT);
  pinMode(IR_PIN_25, INPUT);
  pinMode(IR_PIN_50, INPUT);
  pinMode(IR_PIN_1, INPUT);
  pinMode(IR_PIN_5v, INPUT);
  pinMode(IR_PIN_25v, INPUT);
  
  // Servo
//  /myservo.attach(SERVO_PIN);
  
  // Iniciar LCD
  lcd.begin(16, 4);
  lcd.setRGB(colorR, colorG, colorB); //cor
  lcd.setCursor(0, 0); // col x lin
  lcd.print("Dinheiros: ");
  lcd.setCursor(0, 1); // col x lin
  lcd.print("R$ ");
  lcd.print(dinheiros);
}

void loop() {
  calcularDinheiros();
  
  if(digitalRead(PIN_SWITCH) == LOW){
//    for (int pos = 90; pos >= 0; pos--) {
//      calcularDinheiros();
//      myservo.write(pos);
//      delay(15);
//    }
//    bocaFechada = true;
      Serial.println("Botao pressionado!!");
  }

  if(bocaFechada){
    contador++;
    delay(15);
  }

  if(contador == 200){
    for (int pos = 0; pos <= 90; pos++) {
      calcularDinheiros();
      myservo.write(pos);
      delay(15); 
    }
    bocaFechada = false;
    contador = 0;
  }

}

void atualizarMemoria(){
  EEPROM.put(0, dinheiros);
  // EEPROM.put(8, cinco);
  // EEPROM.put(10, dez);
  // EEPROM.put(12, vinteCinco);
  // EEPROM.put(14, cinquenta);
  // EEPROM.put(16, um);
}

void resetarMemoria(){
  dinheiros = 0;
  cinco = 0;
  dez = 0;
  vinteCinco = 0;
  cinquenta = 0;
  um = 0;

  EEPROM.put(0, dinheiros);
  // EEPROM.put(8, cinco);
  // EEPROM.put(10, dez);
  // EEPROM.put(12, vinteCinco);
  // EEPROM.put(14, cinquenta);
  // EEPROM.put(16, um);
}

void atualizarLCD() {
  if (dinheiros != ultimoValorLCD) {
    lcd.setCursor(0, 0);
    lcd.print("Dinheiros:     ");

    lcd.setCursor(0, 1);
    lcd.print("R$ ");
    lcd.print(dinheiros , 2);
    lcd.print("     ");

    ultimoValorLCD = dinheiros;
  }
}



void calcularDinheiros(){
  leitura5 = analogRead(IR_PIN_5);
  leitura10 = analogRead(IR_PIN_10);
  leitura25 = analogRead(IR_PIN_25);
  leitura50 = analogRead(IR_PIN_50);
  leitura1 = analogRead(IR_PIN_1);
  leitura5v = analogRead(IR_PIN_5v);
  leitura25v = analogRead(IR_PIN_25v);


  
  // Conta moedas
  if(leitura5 > 1015 && !IR5){
    dinheiros += 0.05;
    cinco++;
    IR5 = true;
    //atualizarMemoria();
  }

  if(leitura10 > 1015 && !IR10){
    dinheiros += 0.10;
    dez++;
    IR10 = true;
  }
  if(leitura25 > 1015 && !IR25){
    dinheiros += 0.25;
    vinteCinco++;
    IR25 = true;
  }
  if(leitura50 > 120 && !IR50){
    dinheiros += 0.50;
    cinquenta++;
    IR50 = true;
  }
  if(leitura1 > 1015 && !IR1){
    dinheiros += 1.00;
    um++;
    IR1 = true;
  }
  if(leitura5v > 1015 && !IR5v){
    dinheiros += 0.05;
    cinco++;
    IR5v = true;
  }
  if(leitura25v > 1015 && !IR25v){
    dinheiros += 0.25;
    vinteCinco++;
    IR25v = true;
  }
 
  // Debounce
  if(leitura5 < 1006){
    IR5 = false;
  }
  if(leitura10 < 1006){
    IR10 = false;
  }
  if(leitura25 < 1006){
    IR25 = false;
  }
  if(leitura50 < 50){
    IR50 = false;
  }
  if(leitura1 < 1006){
    IR1 = false;
  }
  if(leitura5v < 1006){
    IR5v = false;
  }
  if(leitura25v < 1006){
    IR25v = false;
  }

  atualizarLCD();

  
  //Serial.print("Dinheiro = ");
//  Serial.println(leitura5);
  

}
