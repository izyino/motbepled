
//
// Exemplo n.5 de utilização da biblioteca motbepled.h
// com WiFi no modo station, para manter acesso a internet
// Pressupõe que um motor de passo 28byj-48 n.0 esteja conectado
// usado para despejar uma dose de ração a cada 45graus girados
// Acessar via browser o IP informado no monitor serial. Fazer então a
// programação da alimentação, de até 4 vezes ao dia, informando para
// cada refeição: hora, minuto e qtde de doses (uma dose=45graus)
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
#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <TimeLib.h>  
#include <WiFiAP.h>
#include <motbepled.h>

#include "html_unico.h"

WebServer server(80);

motbepled x(2);

String ssid,pass;

String hora;
byte   versao[4]={1,1,7,24};            //versão 1.1, julho, 2024
byte   serviu[4]={0,0,0,0};
bool   chntp=false,chwifi=false;
byte   h,m,s,parx[128];
int    n,i,k;

const char* ntpServer = "pool.ntp.org"; //servidor NTP
const long  gmtOffset_sec = -10800;     //gmt-3 (em segundos
const int   daylightOffset_sec = 0;     //horário de verão
  
void setup() {
  Serial.begin(115200);
  EEPROM.begin(128);
  x.pinsStep0(0,1,2,3,-1,-1);   //informa os pinos usados pelo motor
  x.pinBeep(10);x.pinLed(8, 0); //pino usado pelo Beep e pelo Led
  x.begin();
  x.beep(2, 200, 2000, 100);    //emite 2 beeps de 200ms, 2000Hz
  x.led(10, 50, 25);            //pisca o LED 10 vezes com 50ms aceso e 25ms apagado
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  x.beep(1, 200, 2000, 0);  //emite 1 beep de 200ms cada, 2000Hz
  x.led(10, 50, 25);        //pisca o LED 10 vezes com 50ms aceso e 25ms apagado

  EEPROM.write(92, versao[0]);EEPROM.write(93, versao[1]);
  EEPROM.write(94, versao[2]);EEPROM.write(95, versao[3]);
  EEPROM.commit();

  while (!Serial);
  delay(200);
  Serial.println("");
  Serial.println("Alimentador de animais - Programa teste n.5");
  Serial.println("-------------------------------------------");
  Serial.println("");

  dumpflash();

  initudo();
  pegahora();
  Serial.print("Hora de início: ");Serial.println(hora);
}

void loop() {
  server.handleClient();
  if ((millis()%1000)==0){           //a cada segundo verifica se deve servir...
    pegahora();
    for (k=0; k<4; k++){
      if ((h==parx[80+k*3])&&(m==parx[81+k*3])&&(serviu[k]==0)&&(x.xsteps[0]==0)){
        x.beep(3, 1000, 2000, 1000); //beep e pisca
        x.led(10, 50, 25);           //para avisar que vai servir 
        uint32_t nsteps=map(parx[82+k*3]*45,0,360,0,2048);  
        x.runStep(0, nsteps, 5, 1);  //roda motor 45 graus * doses a 5RPM
        while (x.stepstogo(0)!=0){}      //espera rodar 
        serviu[k]=1;
        if (k>0){serviu[k-1]=0;}else{serviu[3]=0;}
        Serial.print("Serviu: ");Serial.print(parx[82+k*3]);
        Serial.print(" doses, as ");Serial.println(hora);
      }
    }
  }
}

void initudo(){
  ssid="";pass="";
  for (k=0; k<EEPROM.read(124); k++){ssid=ssid+char(EEPROM.read(k));}
  for (k=0; k<EEPROM.read(125); k++){pass=pass+char(EEPROM.read(k+32));}
  for (k=0; k<128; k++){parx[k]=EEPROM.read(k);}
  Serial.println("Memoria EEPROM ok. Parametros recuperados com sucesso...");
  conectawifi();
  server.begin();
  server.on("/", raiz);
  server.on("/lepar", Lepar);
  server.on("/gravapar", Gravaparm);
}

void raiz(){
  String enviapg = uni_page;
  server.send(200, "text/html", enviapg);
}

void Lepar() {
  String parmValues="";
  for (int k=0;  k<128; k++){parmValues += String(parx[k]) + "&";}
  server.send(200, "text/plane", parmValues);
};


void Gravaparm() {
  x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
  x.ldur=50;x.linter=25;x.lnum=10;
  for (k=76; k<92; k++){
    parx[k] = server.arg(String(k-76)).toInt();
    EEPROM.write(k, parx[k]);
  }
  EEPROM.commit();
//  dumpflash();
  server.send(200, "text/plane", "ok");
}


void conectawifi()
{
  Serial.print("Tentando se conectar na rede ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  chwifi=false;
  uint32_t milisatu=millis();
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");delay(250);
    if ((milisatu+5000)<millis()){Serial.println("");Serial.println("Rede WiFi NÃO conectada");
      WiFi.disconnect();
      chwifi=false;break;
    }
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.println("");Serial.print("Rede WiFi conectada. IP: ");
    Serial.println(WiFi.localIP());
    chwifi=true;
  }
}


void pegahora(){
  struct tm timeinfo;
  bool chntp=false;
  k=0;
  uint32_t milisatu=millis();
  while (!getLocalTime(&timeinfo)){
    if ((k>10)||(milisatu+1000)<millis()){
      chntp=false;break;
    }  
  }

  if (getLocalTime(&timeinfo)){chntp=true;}

  hora="??:??:??";
  
  if (chntp){
    h=timeinfo.tm_hour;
    m=timeinfo.tm_min;
    s=timeinfo.tm_sec;
    hora="";

    if (h<10){hora="0"+String(h);}else{hora=hora+String(h);}
    hora=hora+":";
    if (m<10){hora=hora+"0"+String(m);}else{hora=hora+String(m);}
    hora=hora+":";
    if (s<10){hora=hora+"0"+String(s);}else{hora=hora+String(s);}
  }
} 


void dumpflash()
{
  uint8_t xbyte;
  uint16_t addr, k; char hexVal[5];
  Serial.println("");
  for (addr=0; addr<127; addr=addr+16){
    sprintf(hexVal, "%04d", addr);
    Serial.print(hexVal);Serial.print("  ");  
    sprintf(hexVal, "%04X", addr);
    Serial.print(hexVal);Serial.print("  ");  
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);
      sprintf(hexVal, "%02X", (xbyte));Serial.print(hexVal);Serial.print(" ");
    }
    Serial.print(" ");
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);         
      Serial.write(xbyte>31 && xbyte != 127 ? char (xbyte) : '.');
    }
    Serial.println("");
  }
  Serial.println("");
}  


//////////////////////////////////// fim ///////////////////////////////////////////////
