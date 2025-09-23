
//
// Exemplo n.2 de utilização da biblioteca motbepled.h
// emite beeps e piscadas de led, e movimentos repetitivos
// Pressupõe um motor de passo 28byj-48 (motor n.0)
// e dois motores DC n. 3 e 4 
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


#include <motbepled.h>

motbepled x(2, 0);            //indica motor de passo n.0 no modo 2 motores DC n.3 e n.4

bool sent;
uint32_t voltas;

void setup() {
  
  x.pinsStep0(16,17,18,19,15,14);  //4 pinos step n.0 mais 2 pinos en1 e en2
  x.pinsDC2(13,27,25);             //pinos motor DC n.2 (13,27) e o seu pwm (25)
  x.pinsDC3(33,23,26);             //pinos motor DC n.3 (33,23) e o seu pwm (26)
  x.pinBeep(5);x.pinLed(4, 1);     //pino beep (5) e pino Led (4), Led acesso com nivel alto (1)
  
  x.begin();
  Serial.begin(115200);
}

void loop() {
  
  if (x.timetogo(2)==0){      //se o motor DC n.2 estiver parado,      
    x.runDC(2, 4000, 100, 1); //ativa o motor DC n.2 por 4 segundos, PWM 100%, setido horário
  }
  if (x.timetogo(3)==0){      //se o motor DC n.3 estiver parado,
    x.runDC(3, 12000, 45, 0); //ativa o motor DC n.3 por 12 segundos, PWM 66%, setido antihorário
  }

  if (x.stepstogo(0)==0){     //se o motor de passo n.0 estiver pararado
    x.beep(2, 200, 2000, 100);//emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
    x.led(20, 50, 25);        //pisca o LED 20 vezes com 50ms aceso e 25ms apagado
    voltas=2048*random(1,21); //gera número de voltas entre 1 e 20
    sent=!sent;               //inverte o sentido e ativa o motor para andar 2048 passos, 1 a 10RPM
    x.runStep(0, voltas, random(1, 11), sent);
  }  
     
  x.setms(1000);
  while(x.getms()>0){}        //espera 100ms (apenas como exemplo) e repete tudo 
}
