
// 
// Exemplo n.3 de utilização da biblioteca motbepled.h
// via Wi-Fi, com o servidor web do ESP-32 definido como access-point
// Pressupõe um motor de passo 28byj-48 (motor n.0)
// Rede Wi-Fi criada com o nome motbepled senha 00000000, acessível
// pelo IP 192.168.4.1 ou outro, informado pelo monitor serial
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

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <motbepled.h>

motbepled motor(2); // indica motor de passo no modo 2 

const char *ssid = "motbepled";
const char *password = "00000000";

bool ledState = 0;
const int ledPin = 2;
int velocidade, angulo, sentido, steps;
long passos;

AsyncWebServer server(80);
IPAddress local_IP(192, 168, 1, 123);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="pt-BR">
<head>
  <title>motbepled</title>
  <meta charset="utf-8"/>
  <meta name="viewport" conteudo="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: monospace;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #000000;
  }
  .cabec {
    overflow: hidden;
    background-color: #0F8B8D;
  }
  body {
    margin: 0;
  }
  .conteudo {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .cartao {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .botao {
    padding: 10px 40px;
    font-family: monospace;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /.botao:hover {background-color: #0f8b8d}/
   .botao:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<meta name="viewport" conteudo="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="cabec">
    <h1>Exemplo de uso da biblioteca motbepled.h</h1>
  </div>
  <div class="conteudo">
    <div class="cartao">
    <form action="">
      <h2>Parâmetros para movimentação do motor</h2>
        <label>Velocidade (RPM).....: </label>
        <input type="number" list="velocidade"  id="Velocidade">
        <datalist id="velocidade">
            <option value=1>
            <option value=2>
            <option value=5>
            <option value=6>
            <option value=10>
            <option value=12>
        </datalist><br>
        <label>Ângulo (°)...........: </label>
        <input type="number" list="angulo" id="Angulo">
        <datalist id="angulo">
            <option value=0>
            <option value=15>
            <option value=30>
            <option value=45>
            <option value=90>
            <option value=180>
            <option value=360>
        </datalist><br>
        <label>Passos (se ângulo=0).: </label>
        <input type="number" list="passos" id="Passos">
        <datalist id="passos">
            <option value=512>
            <option value=1024>
            <option value=2048>
            <option value=8192>
            <option value=20480>
            <option value=204800>
        </datalist><br>
        <label>Sentido..............: </label>
        <input type="text" list="sentido" id="Sentido">
        <datalist id="sentido">
            <option value="horário">
            <option value="anti-horário">
        </datalist>
    </form>
      <p id="mensagem"></p>
      <button class="botao" type="button" onclick="girar()" id="botao">Girar</button>
    </div>
  </div>
<script>
  function girar() {
    var xhttp = new XMLHttpRequest();
    var velocidade = document.getElementById('Velocidade').value;
    var angulo = document.getElementById('Angulo').value;
    var passos = document.getElementById('Passos').value;
    var giro = (document.getElementById('Sentido').value=="horário")?1:0;
 xhttp.onreadystatechange = function () {
 if (this.readyState == 4 && this.status == 200) {
  document.getElementById("mensagem").innerHTML = this.responseText;}};
 var msg="&v="+velocidade+"&a="+angulo+"&p="+passos+"&g="+giro;
 console.log("giro?" + msg);
 xhttp.open("GET", "girar?" + msg, true);
 xhttp.send();
  }
</script>
</body>
</html>
)rawliteral";

void setup()
{
  Serial.begin(115200);
  delay(500);
  motor.pinsStep0(0,1,2,3,-1,-1);        //informa os pinos usados pelo motor
  motor.pinBeep(10);motor.pinLed(8, 0);  //pino usado pelo Beep e pelo Led
  motor.begin();
  Serial.println("Programando ESP para o modo soft access point");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);
  server.begin();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {request->send(200, "text/html", html); 
    Serial.println("Enviou pagina!");});
  
  server.on("/girar", HTTP_GET, [](AsyncWebServerRequest *request) { // 
    AsyncWebParameter *p = request->getParam("v");
    velocidade = p->value().toInt();
    p = request->getParam("a");
    angulo = p->value().toInt();
    p = request->getParam("p");
    passos = p->value().toInt();
    p = request->getParam("g");
    sentido = p->value().toInt();

    Serial.print("Velocidade: ");
    Serial.print(velocidade);
    Serial.print(", angulo: ");
    Serial.print(angulo);
    Serial.print(", passos: ");
    Serial.print(passos);
    Serial.print(", sentido: ");
    if(sentido==1){Serial.println("anti-horario");}else{Serial.println("horario");}
    if(angulo==0) {steps=passos;}else{steps = map(angulo, 0, 360, 0, 2048);}

    while (motor.stepstogo(0)!=0){}
    motor.runStep(0, steps, velocidade, sentido);
    Serial.print("Girando ");
    Serial.print(steps);
    Serial.print(" passos, com velocidade ");
    Serial.print(velocidade);
    Serial.print(" e sentido ");
    Serial.println(sentido);
  });
}

void loop()
{
}
