
//
// Exemplo n.1 de utilização da biblioteca motbepled.h
// emite beeps e piscadas de led, e movimentos repetitivos
// -----------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor n.0)
// -----------------------------------------------------------
//


#include <motbepled.h>
motbepled x(2);                      //indica motor de passo no modo 2 em CN1 

bool sent;
uint32_t voltas;

void setup() {
  
  x.pinsStep0(0,1,2,3,-1,-1);        //informa os pinos usados pelo motor
  x.pinBeep(10);x.pinLed(8, 0);      //pino usado pelo Beep e pelo Led
  x.begin();
  Serial.begin(115200);
}

void loop() {
  
  if (x.stepstogo(0)==0){     //se o motor de passo estiver pararado
    x.beep(2, 200, 2000, 100);//emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
    x.led(20, 50, 25);        //pisca o LED 20 vezes com 50ms aceso e 25ms apagado
    voltas=2048*random(1,21); //gera número de voltas entre 1 e 20
    sent=!sent;               //inverte o sentido e ativa o motor para andar 2048 passos, 1 a 10RPM
    x.runStep(0, voltas, random(1, 11), sent);
  }  
     
  x.setms(1000);
  while(x.getms()>0){}        //espera 100ms (apenas como exemplo) e repete tudo 
}
