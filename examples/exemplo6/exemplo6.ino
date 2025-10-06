
//
// Exemplo n.6 de utilização da biblioteca motbepled.h
// utilização de um display oled SSD1306 no conector I2C
// o display mostra a posição do motor de passo, em tempo real
// emite beeps e piscadas de led, e movimentos repetitivos
// Pressupõe um motor de passo 28byj-48 (motor n.0)
// IMPORTANTE: Se estiver sendo utilizado um hardware com
// o ESP32 C3, o led da placa não pode ser utilizado, pois
// é usado como sinal SDA da interface I2C do ESP32 C3
// -----------------------------------------------------------
// Antes de executar qualquer um dos exemplos de uso da biblioteca
// motbepled.h, deve-se conferir com especial cuidado os comandos:
// -----------------------------------------------------------
// motbepled construtor que define o tipo de motor(es) conectados
// pinsStep0 informa os pinos usados pelo motor de passo n.0
// pinsStep1 informa os pinos usados pelo motor de passo n.1
// pinsDC0   informa os pinos usados pelo motor DC n.0
// pinsDC1   informa os pinos usados pelo motor DC n.1
// pinsDC2   informa os pinos usados pelo motor DC n.2
// pinsDC3   informa os pinos usados pelo motor DC n.3
// pinBeep   informa o pino associado ao Beep
// pinLed    informa o pino associado ao led e o nível lógico do led aceso
// -----------------------------------------------------------
//


#include <Arduino.h>
#include <motbepled.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <tabsc>

Adafruit_SSD1306 o(128, 64, &Wire, -1);
motbepled x(2);

bool sent=true;
uint8_t vel;
uint32_t steps, p, pa=0;

void setup() {
  Serial.begin(115200);
  if(!o.begin(2, 0x3C)) {
    Serial.println(F("Não encontrou o display SSD1306"));
    for(;;);
  }

  x.pinsStep0(0,1,2,3,-1,-1); //informa os pinos usados pelo motor
  x.pinBeep(10);              //pino usado pelo Beep
  x.pinLed(-1, 0);            //Led DESABILITADO. pino 8 usado pela I2C)
  x.begin();
  randomSeed(analogRead(4));

  o.clearDisplay();
  for (int k=0; k<64; k++){o.drawLine(0, k, 127, k, SSD1306_WHITE);}
  o.fillCircle(63, 31, 30, SSD1306_BLACK);
  o.display();
}

void loop() {
  
  if (x.stepstogo(0)==0){     //se o motor de passo estiver pararado
    x.beep(2, 200, 2000, 100);//emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
    vel=random(1, 17);        //gera velocidade entre 1 e 16RPM
    steps=2048*random(1,5);   //gera número de voltas entre 1 e 4
    sent=!sent;               //inverte o sentido e ativa o motor para andar n voltas, 1 a 8RPM
    x.runStep(0, steps, vel, sent);
  }else{
    p=int(float(x.stepstogo(0)%2048)/2048.0*360.0);
    if (sent){p=360-p;}
    if (p!=pa){
      raio(pa, SSD1306_BLACK);
      raio(p, SSD1306_WHITE);
      pa=p;
      o.display();
    }
  }  
}

void raio(int ang, int cor)
{
  o.drawLine(63, 31, 63+(tabsc[ang][1])*30, 31+(tabsc[ang][0])*30, cor);
}
