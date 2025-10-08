
//
// Exemplo n.7 de utilização da biblioteca motbepled.h
// move o motor de passo em ambos os sentidos com velocidades
// variadas a partir de um sensor IR conectado ao pino 9 do
// microprocessador. Os comandos provenitentes do controle
// remoto IR são os seguintes:
//
// ok - anda/para o motor alternadamente
// ^  - aumenta a velocidade
// v  - diminui a velocidade
// <  - gira a esquerda
// >  - gira a direita
//
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

#define IR_SMALLD_NEC
#include <IRsmallDecoder.h>
#include <Arduino.h>
#include <motbepled.h>
motbepled x(2);

bool sent=true;
uint8_t vel=8;
uint32_t anda=true;

void setup() {
  
  x.pinsStep0(0,1,2,3,-1,-1);
  x.pinBeep(10);
  x.pinLed(8, 0);
  x.begin();
  x.beep(3, 100, 2000, 100);
  x.led(20, 50, 25);
  x.runStep(0, 2048000000L, vel, sent); 
}

void loop() {
  
  static IRsmallDecoder irDecoder(9);
  irSmallD_t irData;
  
  if (irDecoder.dataAvailable(irData)){
    x.beep(1, 4, 2000, 0);
    x.led(1, 50, 25);
    if (irData.cmd==70){if (vel<15){vel=vel+2;}}
    if (irData.cmd==21){if (vel>3){vel=vel-2;}}
    if (irData.cmd==68){sent=false;}
    if (irData.cmd==67){sent=true;}
    if (irData.cmd==64){anda=!anda;}
    if (anda){x.runStep(0, 2048000000L, vel, sent);}else{x.stopStep(0);}  
  }     
}

// irData.cmd=codigo da tecla pressionada
// irData.keyHeld=0 se for o primeiro pressionamento, 1=tecla mantida pressionada

// codigos retornados:
// 70="^"  74="#"  24="5"
// 21="V"  82="0"  94="6"
// 68="<"  22="1"   8="7"
// 67=">"  25="2"  28="8"
// 64="K"  13="3"  90="9".
// 66="*"  12="4"

