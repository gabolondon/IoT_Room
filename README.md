# Control Home Room

Using an ESP8266 I have created a device that can be connected via serial Wi-Fi to my home Wi-Fi network and control the light in the room, the fan in my room, the TV (on/off), the Air Conditioning (on/off and Temperature selection) and an LED strip, all this making use of connection technologies such as **MQTT, WiFi, RF, Node-Red**, among other protocols that work as the basis of these technologies.

## Description

Through Node-Red, a local server based on Node.js is established, installed on a RaspberryPi, which facilitates the implementation of other communication protocols such as MQTT, HTTP, among others, opening a graphical and code programming interface. in the local address of our choice, in my case 127.0.0.1:1880, note that the default port is 1880.
In this server we graphically program the reception of messages through an MQTT server set to its high communication speed, in addition to programming a graphical interface that can be accessed by web browser from the local network to manipulate the states of the devices with buttons and sliders, as well as view information from the sensors connected to the ESP8266.
In order to use MQTT communication, the Mosquitto server must be previously installed on the raspberry, which establishes an MQTT server locally and will handle the information sent from the devices and NodeRed.
With NodeRed running and Mosquitto working correctly, an ESP8266 is programmed with a code that contains the information of the RF commands used by the remote controls of the TV, the AC and the LED strip, as well as the logic of operation for when it receives a power-on order perform the required action, always keeping in mind that each device has a different flow.

# Files

## Node-Red

In the file # flows **"NodeRed my room17_08_2021.json"** there is the updated flow with graphic programming by blocks and programming by code that will allow a graphical and functional interface with the NodeRed server, in addition to each block that points to MQTT carries the configuration of the parameters to send.
Additionally, there is a file named **"flows NodeRed my room.json"** which is a backup of the initial version that has previous local configurations.

## ESP8266

This device was programmed in ArduinoIDE, so the file with its programming is made in arduino C++, with its IDE you can upload the code in the file **"Control_Cgabo.ino"** to the board and it is ready to connect to the local network (You must change the access data to the network and the IP of the MQTT server"

## Otros

The file **"Mapping controls.xlsx"** and the file **"mapping control univ.txt"** are the results of the tests to get the RF commands from the device controls in the room.

# Hardware

A Raspberry Pi was used for the project, an ESP8266 on a WemosD1 board, connected to 2 buttons, an RF emitter LED, a temperature and humidity sensor, a PIR presence sensor, a double relay module (2 relays) and a 5V DC power source.
The modules used only require cable connection from their output pins to the input pins of the ESP8266, the connection pins are as follows:
|Pin | GPIO |
|--|--|
|Rele 1| D7|
Rele 2 |D6
Pin PIR |D2
DHTPIN |D5
Pin Boton1 |D0
Pin LedIR | D8

---

# Español

# Control Home Room

Utilizando Un ESP8266 he creado un dispositivo que se puede conectar mediante wifi serial a la red de wifi de mi hogar y controlar la luz del cuarto, el ventilador de mi cuarto, el TV (on/off), El Aire Acondicionado (on/off y selección de Temperatura) y una LED strip, todo esto haciendo uso de tecnologías de conección como **MQTT, WiFi, RF, Node-Red**, entre otros protocolos que trabajan como base de estas tecnologías.

## Description

Por medio de Node-Red se establece un servidor local con base en Node.js, instalando en una RaspberryPi, el cual facilita la implementación de otros protocolos de comunicación como MQTT, HTTP, entre otros, abriendo una interfaz de programación gráfica y de código en la dirección local de nuestra preferencia, en mi caso la 127.0.0.1:1880, notese que el puerto por defecto es el 1880.
En este servidor programamos gráficamente la recepción de mensajees por medio de un servidor MQTT puesto a su alta velocidad de comunicación, además que programamos una interfaz gráfica a la que se puede acceder por explorador web desde la red local para manipular los estados de los dispositivos con botones y deslizadores, además de ver la información de los sensores conectados al ESP8266.
Para poder utilizar la comunicación MQTT se debe instalar en la raspberry previamente el servidor Mosquitto, el cuál establece de forma local un servidor MQTT y va a hacer el handling de la información enviada desde los dispositivos y NodeRed.
Con NodeRed corriendo y con Mosquitto funcionando correctamente, se programa un ESP8266 con un código que contiene la información de los comandos RF utilizados por los controles remotos del TV, del AC y de la LED strip, además de la lógica de funcionamiento para cuando reciba una orden de encendido realice la acción requerida, siempre teniendo en cuenta que para cada dispositivo se tiene un flujo diferente.

# Files

## Node-Red

En el archivo # flows **"NodeRed mi cuarto17_08_2021.json"** se encuentra el flujo actualizado con programación gráfica por bloques y programación por código que va a permitir tener una interfaz gráfica y funcional con el servidor NodeRed, además que cada bloque que apunta a MQTT lleva la configuración de los parámetros a enviar.
Adicionalmente se encuentra un archivo con nombre **"flows NodeRed mi cuarto.json"** que es un respaldo de la versión inicial que tiene configuraciones locales previas.

## ESP8266

Este dispositivo se programó en ArduinoIDE, por lo que el archivo con su programación está hecho en C++ de arduino, con su IDE se podrá subir el código en el archivo **"Control_Cgabo.ino"** a la placa y queda lista para conectarse a la red local (Se deben cambiar los datos de acceso a la red y la IP del server MQTT"

## Otros

El archivo **"Mapeo controlles.xlsx"** y el archivo **"mapeo control univ.txt"** son los resultados de las pruebas de obtener los comandos RF de los controles de dispositivos en el cuarto.

# Hardware

Se utilizó para el proyecto un Raspberry Pi, un ESP8266 en una placa WemosD1, conectado a 2 botones, un LED emisor de RF, un sensor de temperatura y humedad, un sensor de presencia PIR, un módulo de relé doble(2 reles) y una fuente de poder 5V DC.
Los módulos utilizados sólo requieren coneción por cable de sus pines de salida hasta los pines de entrada del ESP8266, los pines de conexión son los siguientes:
|Pin | GPIO |
|--|--|
|Rele 1| D7|
Rele 2 |D6
Pin PIR |D2
DHTPIN |D5
Pin Boton1 |D0
Pin LedIR | D8
