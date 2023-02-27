# Repositorio proyecto ruido ambiente
Se incluye el código en Arduino para prototipo de medición de nivel de sonido y los archivos de docker para desplegar en un servidor local Linux el stack de contenedores para la solución construida sobre los servicios de Mosquitto, InfluxDB, NodeRED, Grafana.


## Demo funcionando
![demo](img/demo.jpg)

## Código en Arduino
Código en Arduino para prototipo basado en TinyZero y módulo DFRobot Analog Sound Meter para medición de nivel de sonido en dB y transmisión de datos mediante MQTT a servidor local Linux para visualización en tiempo real y envío de datos Ubidots.

## Hardware
![hardware](img/hardware.png)

## Setup servidor Linux

### Docker

