/*********
 * 
  Jose Antonio Baños  EA7JCL
  Proyecto control remoto de antenas via Bot Telegram, con portal de configuracion
    
*********/

// Load Wi-Fi library
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <Preferences.h>

//defino el objeto EPROM
Preferences EPROM;

//Credenciales del AP cautivo
const char* ssid     = "S.ANTENAS-AP";
const char* password = "123456789";
const char* ssidwifi = "";
const char* passwordwifi = "";

//datos de conexion telegram
String bottoken;
String CHAT_ID = "";

String antena1,antena2,antena3,antena4,antena5,antena6,antena7,antena8 ="";
int primerarranque = 1;

// Incializa la wifi
WiFiClientSecure espClient;
//Objeto telegram
UniversalTelegramBot static bot(bottoken, espClient);    

// Chequea si hay mensajes nuevos cada 1 segundos. Subir este valor si da flood. En testeo.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Define los pines usados en arduino para el control de los reles
const int rele1 = 32;
const int rele2 = 33;
const int rele3 = 25;
const int rele4 = 26;
//#############################
const int rele5 = 27;
const int rele6 = 14;
//#############################
const int rele7 = 23;
const int rele8 = 22;

//rele activo
int activa = 0;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  
  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    
    // Escribe el mensaje recibido
    String text = bot.messages[i].text;
    //Serial.println(text);

    String from_name = bot.messages[i].from_name;
//Si el esp32 acaba de arrancar, en el primer mensaje recibido, da igual lo que sea, envia la info
if (primerarranque == 1){
      
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "El control remoto de antenas se ha iniciado correctamente \n\n";
      welcome += "desde la IP = " + WiFi.localIP().toString() + " \n";
      welcome += "\n\n";
      welcome += "Usa los siguientes comandos para activar las antenas.\n\n";
      welcome += "0  para desactivar todas las antenas \n";
      welcome += "1  para activar " + antena1 + " \n";
      welcome += "2  para activar " + antena2 + " \n";
      welcome += "3  para activar " + antena3 + " \n";
      welcome += "4  para activar " + antena4 + " \n";
      welcome += "5  para activar " + antena5 + " \n";
      welcome += "6  para activar " + antena6 + " \n";
      welcome += "7  para activar " + antena7 + " \n";
      welcome += "8  para activar " + antena8 + " \n";
      welcome += "E para ver el estado de la antena activa \n";
      welcome += "A para mostrar este mensaje \n";
      bot.sendMessage(chat_id, welcome, "");
      primerarranque = 0;
}
    else if (text == "A") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Usa los siguientes comandos para activar las antenas.\n\n";
      welcome += "0  para desactivar todas las antenas \n";
      welcome += "1  para activar " + antena1 + " \n";
      welcome += "2  para activar " + antena2 + " \n";
      welcome += "3  para activar " + antena3 + " \n";
      welcome += "4  para activar " + antena4 + " \n";
      welcome += "5  para activar " + antena5 + " \n";
      welcome += "6  para activar " + antena6 + " \n";
      welcome += "7  para activar " + antena7 + " \n";
      welcome += "8  para activar " + antena8 + " \n";
      welcome += "E para ver el estado de la antena activa \n";
      welcome += "A para mostrar este mensaje \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    else if (text == "0") {
        bot.sendMessage(chat_id, "Se ha desactivado todas las antenas","");
        digitalWrite(activa, HIGH);
        activa=0;
    }
    else if (text == "1") {
      bot.sendMessage(chat_id, antena1, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele1, LOW);
      activa=rele1;
    }
    else if (text == "2") {
      bot.sendMessage(chat_id, antena2, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele2, LOW);
      activa=rele2;
    }
    else if (text == "3") {
      bot.sendMessage(chat_id, antena3, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele3, LOW);
      activa=rele3;
    }
    else if (text == "4") {
      bot.sendMessage(chat_id, antena4, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele4, LOW);
      activa=rele4;
    }
    else if (text == "5") {
      bot.sendMessage(chat_id, antena5, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele5, LOW);
      activa=rele5;
    }
    else if (text == "6") {
      bot.sendMessage(chat_id, antena6, "");
      digitalWrite(activa, HIGH);
      digitalWrite(rele6, LOW);
      activa=rele6;
    }
      else if (text == "7") {
        bot.sendMessage(chat_id, antena7, "");
        digitalWrite(activa, HIGH);
        digitalWrite(rele7, LOW);
        activa=rele7;
      }
      else if (text == "8") {
        bot.sendMessage(chat_id, antena8, "");
        digitalWrite(activa, HIGH);
        digitalWrite(rele8, LOW);
        activa=rele8;
      }
      else if (text == "E") {
      if (activa == 0){
        bot.sendMessage(chat_id, "Todas las antenas estan desactivadas","");
      }
      else if (activa == rele1){
         bot.sendMessage(chat_id, antena1, "");
      }
      else if (activa == rele2){
         bot.sendMessage(chat_id, antena2, "");
      }
      else if (activa == rele3){
         bot.sendMessage(chat_id, antena3, "");
      }
      else if (activa == rele4){
         bot.sendMessage(chat_id, antena4, "");
      }
      else if (activa == rele5){
         bot.sendMessage(chat_id, antena5, "");
      }
      else if (activa == rele6){
         bot.sendMessage(chat_id, antena6, "");
      }
      else if (activa == rele7){
         bot.sendMessage(chat_id, antena7, "");
      }
      else if (activa == rele8){
         bot.sendMessage(chat_id, antena8, "");
      }
    }
    else {
      bot.sendMessage(chat_id, "Comando no reconocido. Escribe A para ver las opciones","");
    }
  }
}
//######################################################

void makeRandomMac(uint8_t mac[6])
{
  for (size_t i = 0; i < 6; ++i) {
    mac[i] = random(256);
  }
  mac[0] = mac[0] & ~0x01;
}

//#######################################################

// Activa el servidor web en el puerto 80
AsyncWebServer server(80);

// Variable que almacena la respuesta http
String header;
const char* PARAM_INPUT_1 = "inssid";
const char* PARAM_INPUT_2 = "passwifi";
const char* PARAM_INPUT_3 = "bottoken";
const char* PARAM_INPUT_4 = "chatid";
const char* PARAM_INPUT_5 = "ant1";
const char* PARAM_INPUT_6 = "ant2";
const char* PARAM_INPUT_7 = "ant3";
const char* PARAM_INPUT_8 = "ant4";
const char* PARAM_INPUT_9 = "ant5";
const char* PARAM_INPUT_10 = "ant6";
const char* PARAM_INPUT_11 = "ant7";
const char* PARAM_INPUT_12 = "ant8";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Formulario configuracion selector remoto de antenas</title>
  <meta charset="utf-8"/>
  <meta name="description" content=""/>

  <style>
*{box-sizing:border-box;}

form{
  width:500px;
  padding:16px;
  border-radius:10px;
  margin:auto;
  background-color:#ccc;
}

form label{
  width:150px;
  font-weight:bold;
  display:inline-block;
}

form input[type="text"],
form input[type="email"]{
  width:460px;
  padding:3px 10px;
  border:1px solid #f6f6f6;
  border-radius:3px;
  background-color:#f6f6f6;
  margin:8px 0;
  display:inline-block;
}

form input[type="submit"]{
  width:100%;
  padding:8px 16px;
  margin-top:32px;
  border:1px solid #000;
  border-radius:5px;
  display:block;
  color:#fff;
  background-color:#000;
} 

form input[type="submit"]:hover{
  cursor:pointer;
}
textarea{
  width:100%;
  height:100px;
  border:1px solid #f6f6f6;
  border-radius:3px;
  background-color:#f6f6f6;     
  margin:8px 0;
  /*resize: vertical | horizontal | none | both*/
  resize:none;
  display:block;
}
  </style>

</head>
<body>

<form action="/get" target="" method="get" name="configuraciones">
        
  <label for="ssid">Nombre WiFi</label>
  inssid:<input type="text" name="inssid" id="ssid" placeholder="Escribe el nombre de tu red WiFi"/>

  <label, for="passwifi">Contraseña red WiFi</label>
  passwifi:<input type="text" name="passwifi" id="passwifi" placeholder="Escribe la contraseña de la WiFi"/>

  <label for="bottoken" />Bot token</label>
  bottoken:<input type="text" name="bottoken" id="bottoken" placeholder="Escribe el token del bot que has creado" required />

  <label for="chatid">Chat ID</label>
  chatid:<input type ="text" name="chatid" id="chatid" placeholder="Escribe tu chat id"/>
  
        <label for="ant1">Nombre antena 1</label>
  antena1:<input type="text" value="" name="ant1" id="ant1" placeholder="Escribe que antena tienes conectada en posicion 1"/>
       
        <label for="ant2">Nombre antena 2</label>
  antena2:<input type="text" value="" name="ant2" id="ant2" placeholder="Escribe que antena tienes conectada en posicion 2"/>
 
        <label for="ant3">Nombre antena 3</label>
  antena3:<input type="text" value="" name="ant3" id="ant3" placeholder="Escribe que antena tienes conectada en posicion 3"/>

        <label for="ant4">Nombre antena 4</label>
  antena4:<input type="text" value="" name="ant4" id="ant4" placeholder="Escribe que antena tienes conectada en posicion 4"/>

        <label for="ant5">Nombre antena 5</label>
  antena5:<input type="text" value="" name="ant5" id="ant5" placeholder="Escribe que antena tienes conectada en posicion 5"/>

        <label for="ant6">Nombre antena 6</label>
  antena6:<input type="text" value="" name="ant6" id="ant6" placeholder="Escribe que antena tienes conectada en posicion 6"/>
 
        <label for="ant7">Nombre antena 7</label>
  antena7:<input type="text" value="" name="ant7" id="ant7" placeholder="Escribe que antena tienes conectada en posicion 7"/>
 
        <label for="ant8">Nombre antena 8</label>
  antena8:<input type="text" value="" name="ant8" id="ant8" placeholder="Escribe que antena tienes conectada en posicion 8"/>
  
<input type="submit" name="guardar" value="Guardar datos"/>
</form>

</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

//funcion para conectar a la wifi
void setup_wifi(){
  Serial.println();
  Serial.println("Conectando a...");
  Serial.println(ssidwifi);
  WiFi.begin(ssidwifi, passwordwifi);
  delay(5000);
  if (WiFi.status() != WL_CONNECTED){
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
  String inputMessage;
  String inputParam;
  // GET valor ssid 
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      EPROM.putString("Essidwifi", inputMessage);
      Serial.println(inputMessage);
    }
  // GET valor passwifi 
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      EPROM.putString("Epasswordwifi", inputMessage);
   }
  // GET valor bot token
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      EPROM.putString("Ebottoken", inputMessage);
    }
   if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      EPROM.putString("Echatid", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_5)) {
      inputMessage = request->getParam(PARAM_INPUT_5)->value();
      inputParam = PARAM_INPUT_5;
      EPROM.putString("Eantena1", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_6)) {
      inputMessage = request->getParam(PARAM_INPUT_6)->value();
      inputParam = PARAM_INPUT_6;
      EPROM.putString("Eantena2", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_7)) {
      inputMessage = request->getParam(PARAM_INPUT_7)->value();
      inputParam = PARAM_INPUT_7;
      EPROM.putString("Eantena3", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_8)) {
      inputMessage = request->getParam(PARAM_INPUT_8)->value();
      inputParam = PARAM_INPUT_8;
      EPROM.putString("Eantena4", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_9)) {
      inputMessage = request->getParam(PARAM_INPUT_9)->value();
      inputParam = PARAM_INPUT_9;
      EPROM.putString("Eantena5", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_10)) {
      inputMessage = request->getParam(PARAM_INPUT_10)->value();
      inputParam = PARAM_INPUT_10;
      EPROM.putString("Eantena6", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_11)) {
      inputMessage = request->getParam(PARAM_INPUT_11)->value();
      inputParam = PARAM_INPUT_11;
      EPROM.putString("Eantena7", inputMessage);
    }
    if (request->hasParam(PARAM_INPUT_12)) {
      inputMessage = request->getParam(PARAM_INPUT_12)->value();
      inputParam = PARAM_INPUT_12;
      EPROM.putString("Eantena8", inputMessage);
    }
    else {
      inputMessage = "Ningun mensaje enviado";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "Solicitud HTTP GET enviada al ESP. Reinicielo "
                                     "<br><a href=\"/\">Volver al portal de configuracion</a>");
  });
  server.onNotFound(notFound);
  server.begin();
  }
  Serial.println(WiFi.localIP());  
}

void setup() {

Serial.begin(115200);
#ifdef ESP8266
  client.setInsecure();
#endif

pinMode(rele1, OUTPUT);
pinMode(rele2, OUTPUT);
pinMode(rele3, OUTPUT);
pinMode(rele4, OUTPUT);
pinMode(rele5, OUTPUT);
pinMode(rele6, OUTPUT);
pinMode(rele7, OUTPUT);
pinMode(rele8, OUTPUT);
digitalWrite(rele1, HIGH);
digitalWrite(rele2, HIGH);
digitalWrite(rele3, HIGH);
digitalWrite(rele4, HIGH);
digitalWrite(rele5, HIGH);
digitalWrite(rele6, HIGH);
digitalWrite(rele7, HIGH);
digitalWrite(rele8, HIGH);


EPROM.begin("configuraciones",false);
String vssidwifi,vpasswordwifi ="";
  
  vssidwifi=EPROM.getString("Essidwifi").c_str();
  ssidwifi=vssidwifi.c_str();  
  vpasswordwifi=EPROM.getString("Epasswordwifi").c_str();
  passwordwifi=vpasswordwifi.c_str();
  bottoken=EPROM.getString("Ebottoken");
  CHAT_ID=EPROM.getString("Echatid");
  antena1=EPROM.getString("Eantena1");
  antena2=EPROM.getString("Eantena2");
  antena3=EPROM.getString("Eantena3");
  antena4=EPROM.getString("Eantena4");
  antena5=EPROM.getString("Eantena5");
  antena6=EPROM.getString("Eantena6");
  antena7=EPROM.getString("Eantena7");
  antena8=EPROM.getString("Eantena8");
  

//Cargar con la linea de abajo descomentada solo una vez para forzar en el esp32 arrancar la web cautiva
// en caso de querer reconfigurar el esp32 sin tener que apagar el router
//volver a comentar con las dos barras y volver a cargar para un funcionamiento normal del sketch   
//EPROM.putString("Essidwifi", "SINWIFI");

setup_wifi();
bot.updateToken(bottoken);

}

void loop(){
  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

}
