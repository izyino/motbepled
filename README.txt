Para facilitar o desenvolvimento de aplicações a serem hospedadas em placas baseadas nos microcontroladores ESP32, foi criada uma biblioteca de nome motbepled.h, disponível em https://github.com/izyino/motbepled a qual dispõe das seguintes funções:
--------------------------------------------------------------------------------------------------------

#include <motbepled.h>

para incluir a biblioteca ao programa. Dependendo de onde a biblioteca estiver gravada, pode-se usar alternativamente o formato #include “motbepled.h”


--------------------------------------------------------------------------------------------------------
motbepled x(t0);
motbepled x(t0, t1);

comando construtor (usar um ou outro) que deve ser informado logo após o include, sendo t0, t1 variáveis do tipo int8_t que definem o tipo de motor de passo eventualmente conectado ao microcontrolador, sendo possível os seguintes valores:

1 – Para motor 28byj-48, 2048 passos por volta, baixo torque, baixo consumo
2 – Para motor 28byj-48, 2048 passos por volta, alto torque, alto consumo
3 – Para motor 28byj-48, 4096 passos por volta, médio torque, médio consumo
4 - Para motor Nema17, 200 passos por volta, modo único

--------------------------------------------------------------------------------------------------------
x.pinsStep0(p1, p2, p3, p4, en1, en2);

comando que informa os pinos do microcontrolador ESP associados ao motor de passo n. 0, sendo p1, p2, p3 e p4 das bobinas principais e en1 e en2 dos sinais de enable. Pode-se informar o valor -1 para os sinais en1 e/ou en2 caso não existam ou não se aplicam. As variáveis p1,p2,p3,p4,en1,en2 são todas do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinsStep1(p1, p2, p3, p4, en1, en2);

comando que informa os pinos do microcontrolador ESP associados ao motor de passo n. 1, sendo p1, p2, p3 e p4 das bobinas principais e en1 e en2 dos sinais de enable. Pode-se informar o valor -1 para os sinais en1 e/ou en2 caso não existam ou não se aplicam. As variáveis p1,p2,p3,p4,en1,en2 são todas do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinsDC0(p1, p2, pwm);
       
comando que informa os pinos do microcontrolador ESP associados ao motor DC n.0, sendo p1,p2 os pinos do motor e pwm o pino da velocidade PWM. O valor -1 significa que os sinais correspondentes não se aplicam no presente caso. As variáveis p1,p2,pwm são do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinsDC1(p1, p2, pwm);
       
comando que informa os pinos do microcontrolador ESP associados ao motor DC n.1, sendo p1,p2 os pinos do motor e pwm o pino da velocidade PWM. O valor -1 significa que os sinais correspondentes não se aplicam no presente caso. As variáveis p1,p2,pwm são do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinsDC2(p1, p2, pwm);
       
comando que informa os pinos do microcontrolador ESP associados ao motor DC n.2, sendo p1,p2 os pinos do motor e pwm o pino da velocidade PWM. O valor -1 significa que os sinais correspondentes não se aplicam no presente caso. As variáveis p1,p2,pwm são do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinsDC3(p1, p2, pwm);
       
comando que informa os pinos do microcontrolador ESP associados ao motor DC n.3, sendo p1,p2 os pinos do motor e pwm o pino da velocidade PWM. O valor -1 significa que os sinais correspondentes não se aplicam no presente caso. As variáveis p1,p2,pwm são do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinBeep(pb);
       
comando que informa o pino do microcontrolador ESP associado ao beep. Pode-se informar o valor -1 caso não exista beep no hardware em questão. A variáveis pb é do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.pinLed(pl, niv);
       
comando que informa o pino do microcontrolador ESP associado ao led (pl) e o nível lógico (0 ou 1)para que o led acenda (niv). Pode-se informar o valor -1 caso não exista led no hardware em questão. As variáveis pb e niv são do tipo int8_t. Este comando deve ser informado na sessão de setup de todos os programas, sempre antes do x.begin()

--------------------------------------------------------------------------------------------------------
x.begin();

inicializa as diversas funções da biblioteca. Deve ser colocado na sessão de setup de todos os programas que se utilizem da biblioteca 

--------------------------------------------------------------------------------------------------------
x.runStep(n, steps, velstep, cwstep);

comando que ativa o motor de passo, de forma automática e assíncrona, conforme as seguintes variáveis:

n – variável uint8_t contendo o número do motor que será movimentado (0 ou 1)

steps – variável uint32_t contendo o número de passos a movimentar

velstep – variável uint16_t que define a velocidade da movimentação, em RPM (Rotações Por Minuto)

cwstep – variável booleana que define o sentido da movimentação, sendo “true” para sentido horário e “false” para sentido anti-horário 

--------------------------------------------------------------------------------------------------------
x.stepstogo(n);

esta função retorna no formato uint32_t o número de passos ainda restantes para que o motor n (n=0 ou 1) chegue ao seu destino. Zero significa que o motor n já chegou ao seu último destino e já encontra-se parado. Antes de comandar qualquer movimentação deve-se consultar esta função para ter certeza que o motor n encontra-se parado. A variável n é do tipo uint8_t

--------------------------------------------------------------------------------------------------------
x.runDC(n, time, veldc, cwdc);

comando que ativa o motor DC, de forma automática e assíncrona, conforme as seguintes variáveis:

n – variável uint8_t com número do motor DC que será movimentado (0, 1, 2 ou 3):
 
time – variável uint32_t contendo o tempo em milisegundos que o motor DC ficará ativado

velDC – variável unint8_t que define a velocidade da movimentação, em termos de  porcentagem entre 0 e 100. Sendo 0=0% motor parado, 100=100% motor com velocidade máxima.

cwDC – variável booleana que define o sentido da movimentação, sendo “true” para sentido horário e “false” para sentido anti-horário 

--------------------------------------------------------------------------------------------------------
x.timetogo(n);

esta função retorna no formato uint32_t, em milisegundos, o tempo ainda restante para que o motor DC n complete o último comando runDC. Se retornar zero significa que o motor DC n já está parado. Antes de comandar qualquer movimentação do motor DC n deve-se consultar esta função para ter certeza que o mesmo se encontra parado. A variável n é do tipo uint8_t

--------------------------------------------------------------------------------------------------------
x.beep(bnum, bdur, bfreq, binter);

comando que ativa a emissão de beeps sonoros, de forma automática e assíncrona, conforme as seguintes variáveis:

bnum – variável inteira que especifica o número de beeps a serem emitidos

bdur – variável inteira que especifica a duração de cada beep, em milisegundos 

bfreq – variável inteira que especifica a freqüência dos beeps, em Hertz (Hz)

binter – variável inteira que especifica a duração da pausa entre os beeps, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.led(lnum, ldur, linter);

comando que ativa piscadas do Led, de forma automática e assíncrona, conforme as seguintes variáveis:

lnum – variável inteira que especifica o número de piscadas a serem emitidas

ldur – variável inteira que especifica a duração do Led acesso em cada piscada, em milisegundos 

linter – variável inteira que especifica a duração do Led apagado em cada piscada, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.setms(yms);

comando para inicializar o contador de milisegundos com o valor informado pela variável yms do tipo uint32_t. Imediatamente após inicializado, o contador começa ser subtraído de 1 a cada milisegundo

--------------------------------------------------------------------------------------------------------
x.getms();

esta função retorna no formato uint32_t o estado antes do contador de milisegundos previamente inicializado pelo comando x.setms. Serve como alternativa para a função delay(), de forma assíncrona

--------------------------------------------------------------------------------------------------------
x.stopStep(n);

esta função interrompe o movimento do motor de passo n (n=0 ou 1)

--------------------------------------------------------------------------------------------------------
x.stopDC(n);

esta função interrompe o movimento do motor DC n (n=0, 1, 2 ou 3)

--------------------------------------------------------------------------------------------------------
x.stopBeep();

esta função interrompe a emissão de beeps sonoros

--------------------------------------------------------------------------------------------------------
x.stopLed();

esta função interrompe as piscadas do Led


############################################################################################
Exemplos de utilização da biblioteca

No início do programa:
#include <motbepled.h>
motbepled x(2);

na sessão do setup:
x.pinsStep0(16,17,18,19,15,14);
x.pinsDC2(13,27,25);
x.pinsDC3(33,23,26);
x.pinBeep(5);
x.pinLed(4, 1);
x.begin();

--------------------------------------------------------------------------------------------------------
//movimenta o motor de passo n.0, tipo 28BYJ-48,
//velocidade 3, sentido horário, 2048 passos:

//função principal:
x.runStep(0, 2048, 3, true);
//o motor começa a se movimentar imediatamente após a função runStep ser executada

//para saber se o motor de passo n.0 já chegou ao destino, fazer
if (x.stepstogo(0)>0) {ainda não chegou ao destino. Está em movimento...};

//a qualquer momento o movimento do motor de passo n.0 pode ser interrompido
x.stopStep(0);


--------------------------------------------------------------------------------------------------------
//movimenta o motor DC n.3,
//velocidade 75%, sentido anti-horário, durante 15segundos:

//função principal:
x.runDC(3, 15000, 75, false);
//o motor começa a se movimentar imediatamente após a função runDC ser executada

//para saber se o motor DC nº3 ainda está girando ou já esta parado, fazer
if (x.timetogo(3)>0) {ainda não terminou o último comando runDC. Está em movimento...};

//a qualquer momento o movimento do motor DC n.3 pode ser interrompido
x.stopDC(3);


--------------------------------------------------------------------------------------------------------
//emite 10 beeps de 2KHz de 0,5s com pausa interbeeps de 0,25s:

//função principal:
x.beep(10, 500, 2000, 250);
//os beeps começam a ser emitidos imediatamente após a função beep ser executada

//a qualquer momento a emissão dos beeps sonoros pode ser interrompida
x.stopBeep();


--------------------------------------------------------------------------------------------------------
//pisca o Led 50 vezes com 0,25s aceso seguido de 0,10s apagado:

//função principal:
x.led(50, 250, 100);
//o Led começa a piscar imediatamente após a função led ser executada

//a qualquer momento as piscadas do Led podem ser interrompidas
x.stopLed();


--------------------------------------------------------------------------------------------------------
//contagem de 4 segundos, de forma assíncrona:

//função principal:
x.setms(4000);
while (x.getms()>0){enquanto espera 4s, pode fazer coisas…}
//a variável x.xms começa a ser decrementada a cada um milisegundo imediatamente após ter sido inicializada pela função setms

############################################################################################





