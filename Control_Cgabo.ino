/*
 Prueba inicial de control de cuarto personal utilizando el NODE Red y ESP8266
 por ahora solo se tiene un módulo de RELE de 2 channel, un sensor de PIR y un sensor de temperatura y humedad
 la idea es implementar un pulsador que encienda y apague las luces de forma permanente y que el NodeRed tambien haga esta función
 pero al tiempo activen una función que el PIR encienda la luz con el movimiento y la deje encendida 5 minutos sin que detecte movimientos

*/
/************************************
 ** Librerias **
 ***********************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h> // Including library for dht
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRutils.h>
/************************************
 ** constantes **
 ***********************************/
#define rele1 D7
#define rele2 D6
#define pinPIR D2
#define puertoMqtt 1883
#define DHTPIN 14     // Para conectar pin data del dth11 en pin D5
#define DHTTYPE DHT11 // se puede seleccionar el DHT22 y DHT21
/***********************************
 ** objetos y variables **
 ***********************************/
DHT dht(DHTPIN, DHTTYPE);              // se configura el pin y el tipo de DHT para la temperatura y humedad
WiFiClient clienteWifi;                // este cliente se encarga de la comunicacion con el wifi
PubSubClient clienteMQTT(clienteWifi); // este utiliza el cliente anterior para hacer poder crear la conexion mqtt
const char *ssid = "your_ssid";
const char *claveWifi = "your_password";
const char *brokerMqtt = "192.168.0.222"; // ip del broker sin http ni nada solo los numeros
uint32_t ultimoIntentoReconexion;
uint32_t timerEnvioDatos;
const int pinBoton = 16;
const uint16_t kRecvPin = 5; // GPIO pin para D1
const uint16_t kIrLed = 15;  // ESP8266 GPIO pin D8; D5 para DHT; D6 para Salida rele 2; D7 salida rele 1; PIR D2
bool estado = false;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

uint16_t rawDataOff[139] = {8958, 4512, 630, 1682, 626, 576, 626, 576, 628, 580,
                            628, 1678, 630, 1680, 626, 580, 628, 580, 624, 1678, 630, 1678, 628, 1684,
                            626, 578, 628, 580, 626, 576, 626, 578, 628, 582, 628, 580, 626, 576, 628, 580,
                            628, 578, 624, 1682, 626, 1680, 630, 580, 624, 576, 626, 578, 628, 582, 628, 576,
                            626, 576, 630, 1680, 626, 576, 624, 1678, 632, 580, 628, 576, 624, 1678, 630, 580,
                            630, 20006, 630, 578, 624, 576, 628, 580, 630, 578, 624, 576, 626, 580, 628, 578,
                            624, 578, 628, 580, 626, 578, 624, 580, 630, 578, 626, 576, 626, 1680, 626, 580,
                            626, 582, 628, 578, 624, 576, 626, 578, 630, 580, 630, 576, 626, 576, 626, 580,
                            630, 580, 626, 576, 624, 576, 628, 582, 628, 578, 626, 578, 624, 576, 628, 1678,
                            628, 580, 630};
uint16_t rawDataOn[139] = {8964, 4512, 628, 1674, 628, 580, 628, 580, 628, 1686, 630,
                           1684, 630, 1684, 630, 580, 630, 580, 630, 1684, 630, 1678, 626, 1674, 626, 576, 626,
                           574, 626, 576, 626, 574, 626, 576, 626, 576, 626, 576, 624, 578, 626, 578, 624, 1674,
                           628, 1684, 628, 1676, 626, 576, 626, 576, 626, 576, 626, 576, 626, 576, 624, 1674, 630,
                           580, 630, 1684, 630, 580, 630, 578, 630, 1684, 630, 580, 630, 20078, 630, 580, 630,
                           580, 630, 580, 630, 580, 630, 580, 630, 578, 626, 574, 626, 576, 626, 576, 626, 578,
                           630, 578, 630, 580, 630, 578, 626, 1674, 626, 576, 628, 576, 626, 576, 626, 576, 626,
                           576, 626, 576, 626, 576, 626, 576, 624, 576, 626, 576, 624, 576, 626, 576, 626, 576,
                           624, 576, 626, 578, 628, 582, 628, 1684, 628, 1674, 630};
IRsend irsend(kIrLed);
IRrecv irrecv(kRecvPin);
decode_results results; // Somewhere to store the results

void conectarAlWifi()
{

  WiFi.begin(ssid, claveWifi);
  Serial.print("conectando...");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi Conectado ");
  Serial.println("direccion IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *mensaje, unsigned int length)
{
  String topico = topic;
  Serial.print("Mensaje recibido del topico: ");
  Serial.println(topico);
  Serial.print("mensaje: ");
  Serial.println(int((char)mensaje[0]));
  if (topico == "/rele1")
  {
    if (int((char)mensaje[0]) == 53)
    {
      if (digitalRead(rele1) == LOW)
      {
        digitalWrite(rele1, HIGH);
        char msg1[10];
        snprintf(msg1, 5, "%ls", "off");
        clienteMQTT.publish("/ventilador", msg1);
        Serial.print("se envia estado de ventilador: ");
        Serial.println(msg1);
      }
      else
      {
        digitalWrite(rele1, LOW);
        char msg1[10];
        snprintf(msg1, 5, "%ls", "on");
        clienteMQTT.publish("/ventilador", msg1);
        Serial.print("se envia estado de ventilador: ");
        Serial.println(msg1);
      }
    }
  }
  else if (topico == "/rele2")
  {
    if (int((char)mensaje[0]) == 53)
    {
      if (digitalRead(rele2) == LOW)
      {
        digitalWrite(rele2, HIGH);
        char msg1[10];
        snprintf(msg1, 5, "%ls", "off");
        clienteMQTT.publish("/luz", msg1);
        Serial.print("se envia estado de luz: ");
        Serial.println(msg1);
      }
      else
      {
        digitalWrite(rele2, LOW);
        char msg1[10];
        snprintf(msg1, 5, "%ls", "on");
        clienteMQTT.publish("/luz", msg1);
        Serial.print("se envia estado de luz: ");
        Serial.println(msg1);
      }
    }
  }
  else if (topico == "/aire")
  {
    if (int((char)mensaje[0]) == 53)
    {
      if (estado == true)
      {
        estado = false;
        char msg1[10];
        snprintf(msg1, 5, "%ls", "off");
        clienteMQTT.publish("/aire", msg1);
        Serial.println("se envía rawoff ");
        irsend.sendRaw(rawDataOff, 139, 38); // Send a raw data capture at 38kHz.
      }
      else
      {
        estado = true;
        char msg1[10];
        snprintf(msg1, 5, "%ls", "on");
        clienteMQTT.publish("/aire", msg1);
        Serial.println("se envía rawon ");
        irsend.sendRaw(rawDataOn, 139, 38); // Send a raw data capture at 38kHz.
      }
    }
  }
  else
  {
    Serial.println("error de mensaje");
  }
}

boolean reconexion()
{
  Serial.print("Conectando a broker mqtt");
  // intentando conectar al broker
  if (clienteMQTT.connect("ESP8266Client"))
  {
    Serial.println("Conectado");
    // publicamos que estamos conectados
    clienteMQTT.publish("/conexion", "Conectado");
    // nos suscribimos a los topicos para controlar los ledes
    clienteMQTT.subscribe("/rele1");
    clienteMQTT.subscribe("/rele2");
    clienteMQTT.subscribe("/aire");
  }
  else
  {
    Serial.print("fall=");
    Serial.print(clienteMQTT.state());
  }
  return clienteMQTT.connected();
}

void setup()
{
  irsend.begin();
  irrecv.enableIRIn();
  Serial.begin(kBaudRate);
  while (!Serial) // Wait for the serial connection to be establised.
    delay(50);
  Serial.printf("\nIRrecvDumpV2 is now running and waiting for IR input on Pin "
                "%d\n",
                kRecvPin);

  Serial.println("iniciando programa Cuarto gabo");
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(pinPIR, INPUT);
  pinMode(pinBoton, INPUT);
  dht.begin();
  conectarAlWifi();
  clienteMQTT.setServer(brokerMqtt, puertoMqtt); // le decimos cual es el servidor y el puerto al que se debe conectar
  clienteMQTT.setCallback(callback);             // le decimos como se llama la funcion de callback
}

void loop()
{

  if (!clienteMQTT.connected())
  {
    if (millis() - ultimoIntentoReconexion > 5000)
    {
      ultimoIntentoReconexion = millis();
      // Attempt to reconnect
      if (reconexion())
      {
        ultimoIntentoReconexion = 0;
      }
    }
  }
  else
  {
    // cliente conectado
    bool PIR = digitalRead(pinPIR);
    delay(10);
    if (millis() - timerEnvioDatos > 5000)
    { // Se utiliza un timer de 5 segundos (5000 ms)
      timerEnvioDatos = millis();
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t))
      { // se verifica si leyó desde el sensor DHT
        Serial.println(F("Fallo la lectura desde el sensor DHT"));
        return;
      }
      char msg[5];
      snprintf(msg, 5, "%lf", t);
      clienteMQTT.publish("/temperatura", msg);
      snprintf(msg, 5, "%lf", h);
      clienteMQTT.publish("/humedad", msg);
    }

    if (digitalRead(pinPIR) != PIR)
    {
      delay(10);
      char msg[4];
      snprintf(msg, 2, "%ld", digitalRead(pinPIR));
      clienteMQTT.publish("/PIR", msg);
      Serial.print("se envía a NR el valor de PIR ");
      Serial.println(msg);
    }
    if (irrecv.decode(&results))
    {
      // print() & println() can't handle printing long longs. (uint64_t)
      serialPrintUint64(results.value, HEX);
      switch (results.value)
      {
        char msg1[10];
      case 16753245:
        Serial.println("se envía rawoff ");
        irsend.sendRaw(rawDataOff, 139, 38); // Send a raw data capture at 38kHz.
        // char msg1[10];
        snprintf(msg1, 5, "%ls", "off");
        clienteMQTT.publish("/aire", msg1);
        break;
      case 16736925:
        Serial.println("se envía rawon ");
        irsend.sendRaw(rawDataOn, 139, 38); // Send a raw data capture at 38kHz.
        snprintf(msg1, 5, "%ls", "on");
        clienteMQTT.publish("/aire", msg1);
        break;
      }
      irrecv.resume(); // Receive the next value
    }

    clienteMQTT.loop();
  }
}
