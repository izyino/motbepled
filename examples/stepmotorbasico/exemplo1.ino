
//
// Exemplo n.1 de utilização da biblioteca motbepled.h
// emite beeps e piscadas de led, e movimentos repetitivos
// Pressupõe um motor de passo 28byj-48 (motor n.0)
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
motbepled x(2);               //indica motor de passo no modo 2 em CN1 

bool sent=true;
uint16_t vel;
uint32_t voltas;

void setup() {
  
  x.pinsStep0(0,1,2,3,-1,-1); //informa os pinos usados pelo motor
  x.pinBeep(10);              //pino usado pelo Beep
  x.pinLed(8, 0);             //pino usado pelo Led
  x.begin();
  randomSeed(analogRead(4));
}

void loop() {
  
  if (x.stepstogo(0)==0){     //se o motor de passo estiver pararado
    x.beep(2, 200, 2000, 100);//emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
    x.led(20, 50, 25);        //pisca o LED 20 vezes com 50ms aceso e 25ms apagado
    vel=random(1, 17);        //gera velocidade entre 1 e 16RPM
    voltas=2048*random(1,21); //gera número de voltas entre 1 e 20
    sent=!sent;               //inverte o sentido e ativa o motor para andar n voltas, 1 a 8RPM
    x.runStep(0, voltas, vel, sent);
  }  
     
  x.setms(100);
  while(x.getms()>0){}        //espera 100ms (apenas como exemplo) e repete tudo 
}

