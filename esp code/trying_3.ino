#include <ESP8266WiFi.h>
#define capacidade  6//quatidade de cadeiras
//estado das cadeiras {0=livre, 1= ocupada}
int stbt1 = 0;
int stbt2 = 0;
int stbt3 = 0;
int stbt4 = 0;
int stbt5 = 0;
int stbt6 = 0;
//sensores
int sensor1 = 1;//sensor 1
int sensor2 = 1;//sensor 2
int sts1;//estado atual do sensor 1
int sts2;//estado atual do sensor 2
int sts1a,sts2a;//estado anterior
//contadores
int cont;//contador geral
int lugares;//contador de lugares ocupados

const char* ssid = "Crazzy Host";//Rede a qual o modulo estará conectado
const char* password = "#limite110";//Senha da Rede

WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  delay(10);

  // Connectar a rede wifi
  Serial.println();
  Serial.println();
  Serial.print("Connectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");

  // Inicialização do servidor 
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.println(WiFi.localIP());

}
String ler_st(int stbt)
  {
    if (stbt == 1){
      return "Ocupada";
    }else{
      return "Livre";
    }
  }

String ler_sensores(){
  String st_sensores;
  sts1a=sts1;
  sensor1 = digitalRead(D0);  
  if (sensor1 == 0)
  {  
    st_sensores += "Sensor 1 :: Movimento - <a id=\"s1\"> Detectado </a><br>";
    digitalWrite(D3, HIGH);
    sts1=1;
  }  
  else{  
    st_sensores += "Sensor 1 :: Movimento - <a id=\"s1\"> Ausente </a><br>";
    digitalWrite(D3, LOW);
    sts1=0;
  }  

  st_sensores +="<br>";
   sts2a=sts2;
   sensor2 = digitalRead(D7);
   if (sensor2 == 0)
  {  
   st_sensores += "Sensor 2 :: Movimento - <a id=\"s2\"> Detectado </a><br>";
    digitalWrite(D3, HIGH);
    sts2=1;
  }  
  else{  
    st_sensores += "Sensor 2 :: Movimento - <a id=\"s2\"> Ausente </a><br>";
    digitalWrite(D3, LOW);
    sts2=0;
  }
  
  return st_sensores;  
}
String movimento_capacidade(){

  String mov_cap;
  mov_cap += "<h4> Status de Movimento</h4>";
  mov_cap += " Status :: ";
  if(sts1a==1 && sts1==0 && sts2==1 ){
     mov_cap += "<a id=\"status\"> Entrada</a><br>";
    cont++;
  }
  if (sts2a==1 && sts2==0 && sts1==1){
     mov_cap += "<a id=\"status\"> Saida</a><br>";
    cont--;
  }
  if (sts2==1 && sts1==1){
    mov_cap += "<a id=\"status\"> possivel problema nos sensores detectado</a><br>";
  }
  if (sts2==0 && sts1==0){
    mov_cap += "<a id=\"status\"> Inativo</a><br>";
  }
  if(cont<0){
    cont=0;
  }
  
  mov_cap += "<h4> Status de Capacidade</h4>";
  
  if(cont >= capacidade){
    mov_cap += "<a id=\"capacidade\">Superlotação</a><br>";
    mov_cap += "Lugares disponiveis :: ";
    mov_cap += "<a id=\"lugdisponiveis\">0</a><br>";
  } else {
    mov_cap += "Lugares disponiveis :: ";
    lugares = capacidade - cont;
    mov_cap += "<a id=\"lugdisponiveis\">";
    mov_cap +=lugares;
    mov_cap +="</a><br>";
  }
  if(lugares <0){
    lugares =0;
  }

  mov_cap += "<br>";
  
  mov_cap += "Contador :: ";
  mov_cap += "<a id=\"contador\">";
  mov_cap += cont;
  mov_cap += "</a><br>";

  return mov_cap;
}
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Novo cliente");
  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  
  String pag = "";

  pag += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  pag += "<html lang=\"en\"><head><meta http-equiv='refresh' content='3' name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  pag += "<title>ESP8266 Web Server</title>";
  pag += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;}</style>";
  pag += "</head>";
  pag += "<h3>ESP8266 Web Server</h3>";
  pag += "<h3>Sistema de controle de fluxo</h3>";

  pag += "<h4>Status de Cadeiras: </h4>";
  
  pag += "Cadeira 1 :: ";
  stbt1 = digitalRead(D1);
  pag += "<a id=\"c1\">"+ler_st(stbt1)+"</a><br>";
  
  pag += "<br> Cadeira 2 :: ";
  stbt2 = digitalRead(D2);
  pag += "<a id=\"c2\">"+ler_st(stbt2)+"</a><br>";
  
  pag +="<br> Cadeira 3 :: ";
  stbt3 = digitalRead(D4);
  pag += "<a id=\"c3\">"+ler_st(stbt3)+"</a><br>";

  pag += "<br> Cadeira 4 :: ";
  stbt4 = digitalRead(D5);
  pag += "<a id=\"c4\">"+ler_st(stbt4)+"</a><br>";

  pag += "<br> Cadeira 5 :: ";
  stbt5 = digitalRead(D6);
  pag += "<a id=\"c5\">"+ler_st(stbt5)+"</a><br>";
  
  pag += "<br> Cadeira 6 :: ";
  stbt6 = digitalRead(D8);
  pag += "<a id=\"c6\">"+ler_st(stbt6)+"</a><br>";

  pag += "<h4> Status dos sensores</h4>";
  
  pag += ler_sensores();

  pag += movimento_capacidade();
    
  pag += "</html>\n";
  client.print(pag);
  client.flush();
  
}
