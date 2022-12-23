/*
BUZÓN_INTELIGENTE

Este sistema basado en un Wemos D1 mini (ESP8266), tiene el objetivo de mandar un mensaje de Telegram 
a un usuario/grupo cuando una carta llegue al buzón, o se abra la puerta del mismo.

Durante la mayoria del tiempo se encuentra en reposo para ahorrar batería.

Autor: Daniel Rosell Moreno
GitHub: drosell271
*/

/*
VARIABLES
*/
//Librerias necesarias
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

//Configuración del modo ahorro
#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
extern "C" 
{
  #include "user_interface.h"
}

//Configuración Wi-Fi
const char* ssid = "SSID";
const char* password = "PASSWD";

//Bot de Telegram
#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "YYYYYYYYYYYYYYYYYYY"
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Definimos los distintos sensores
#define CARTA 14  //sensor de cartas en GPIO14 (D5)
#define PUERTA 12 //sensor de puerta abierta GPIO12 (D6)
int puerta_abierta = 0;
ADC_MODE (ADC_VCC); //Hacemos que el pin A0 lea directamente el voltaje de alimentación

/*
FUNCIONES
*/
//Conexión Wi-Fi
void conect_wifi()
{
  Serial.print("Conectandose a: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("¡¡Conectado!!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

//Callback
void callback() 
{
  Serial1.println("Callback");
  Serial.flush();
}

//Suspensión del sistema
void sleep()
{
  Serial.println("Entrando en modo ahorro");
  gpio_pin_wakeup_enable(GPIO_ID_PIN(CARTA), GPIO_PIN_INTR_HILEVEL);
  gpio_pin_wakeup_enable(GPIO_ID_PIN(PUERTA), GPIO_PIN_INTR_HILEVEL);
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_set_wakeup_cb(callback);
  wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);
  delay(1000);
}

//Decide y envia el mensaje
void send_message()
{
  if (puerta_abierta == 1)
  {
    Serial.println("Puerta abierta");
    bot.sendMessage(CHAT_ID, "Correo recogido \xF0\x9F\x93\xAA", "");
    delay(120000);
    puerta_abierta = 0;
  }
  else
  {
    Serial.println("Carta detectada");
    bot.sendMessage(CHAT_ID, "Ha llegado una carta \xF0\x9F\x93\xAB", "");
  }
}

//Obtiene el valor de batería
void battery_check()
{
  Serial.println("Comprobando batería");
  float voltaje;
  int counter;

  voltaje = 0;
  counter = 0;
  while (counter <= 10)
  {
    voltaje = voltaje + (ESP.getVcc() / 1000);
    delay(100);
    Serial.printf("Check: %d \n", counter);
    counter ++;
  }
  voltaje = voltaje / 10;
  Serial.printf("Voltaje: %f \n", voltaje);
  if (voltaje <= 2.75)
  {
    Serial.println("Batería Baja");
    bot.sendMessage(CHAT_ID, "Batería baja \xF0\x9F\x94\x8B", "");
  }
  else
  {
    Serial.println("Batería OK");
  }
}

/*
PROGRAMA PRINCIPAL
*/
void setup() 
{
  //Inciamos la comunicacion serie en 115200
  Serial.begin(115200);

  //Nos conectamos a la red Wi-Fi
  conect_wifi();

  //Iniciamos callback
  callback();

  //Añade el certificado de api.telegram.org
  client.setTrustAnchors(&cert);

  //Configuramos los sensores
  pinMode(CARTA, INPUT);
  pinMode(PUERTA, INPUT);

  //Obtener hora local
  configTime(0, 0, "pool.ntp.org");

  //Mensaje de Inicio
  bot.sendMessage(CHAT_ID, "Sistema Iniciado correctamente \xE2\x9C\x85", "");
}

void loop()
{
  //Iniciamos el modo ahorro
  sleep();

  //Al activarse un sensor se sale del modo ahorro
  Serial.println("Saliendo del modo ahorro");

  //Comprobamos si el sensor que ha reactivado el sistema es el de la puerta
  puerta_abierta = digitalRead(PUERTA);

  //Nos conectamos a la red Wi-Fi
  conect_wifi();

  //Modem completamente encendido
  wifi_set_sleep_type(NONE_SLEEP_T);

  //Enviamos el mensaje
  send_message();

  //comprobamos la batería
  battery_check();
  
  delay(1000);
}
