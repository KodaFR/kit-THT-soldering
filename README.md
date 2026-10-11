# Kit THT Soldering

Projet de carte électronique à composants traversants (THT), avec un firmware
basé sur Arduino.

## Matériel

- Circuit imprimé à deux couches avec composants THT.
- Schéma, PCB et projet KiCad disponibles dans [`kitkodatht/`](kitkodatht/).

## Firmware

Le firmware se trouve dans [`firmware/main/`](firmware/main/). Il est conçu
pour être compilé et téléversé avec l’Arduino IDE.

1. Installez les bibliothèques listées dans [`requirements.txt`](requirements.txt)
   avec le gestionnaire de bibliothèques Arduino.
2. Ouvrez `firmware/main/main.ino` dans l’Arduino IDE.
3. Sélectionnez la carte et le port série correspondant à votre matériel, puis
   compilez et téléversez le firmware.

## Communication série

Configurez le moniteur série sur **9600 bauds**.
