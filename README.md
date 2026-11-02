# Kit THT Soldering

<p align="center">
  <img src="https://img.shields.io/badge/ESP32--S3-microcontrôleur-00599C?logo=espressif&logoColor=white" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/Arduino-IDE-00878F?logo=arduino&logoColor=white" alt="Arduino IDE">
  <img src="https://img.shields.io/badge/KiCad-PCB-314CB0?logo=kicad&logoColor=white" alt="PCB KiCad">
  <img src="https://img.shields.io/badge/Projet-en%20français-blue" alt="Projet en français">
</p>

<p align="center">
  Un kit électronique à assembler et à programmer autour d’un <strong>ESP32-S3</strong>.<br>
  🔧 Assemblez la carte · 💻 Téléversez le firmware · 🎮 Explorez son interface
</p>

<p align="center">
  <img src="visuels/pcb.png" alt="Aperçu 3D du circuit imprimé du kit" width="720">
</p>

## 🧭 Parcours

```mermaid
flowchart LR
    A["🧰 Préparer le matériel"] --> B["🔥 Souder selon la BOM<br/>et la sérigraphie"]
    B --> C["🔌 Brancher en USB-C"]
    C --> D["⚙️ Configurer Arduino IDE"]
    D --> E["⬆️ Téléverser le firmware"]
    E --> F["🎮 Utiliser les boutons et l'écran"]
```

<details>
<summary>Afficher le sommaire</summary>

- [🧰 Avant de commencer](#avant-de-commencer)
- [🔥 1. Assembler la carte](#assembler-la-carte)
- [⚙️ 2. Installer Arduino IDE](#installer-arduino-ide)
- [⬆️ 3. Téléverser le firmware](#televerser-le-firmware)
- [🎮 4. Utiliser la carte](#utiliser-la-carte)
- [🛠️ Dépannage](#depannage)
- [📂 Fichiers du projet](#fichiers-du-projet)

</details>

<a id="avant-de-commencer"></a>
## 🧰 Avant de commencer

### 🧪 Matériel

- Le circuit imprimé et les composants indiqués dans la [BOM](kitkodatht.csv).
- Un fer à souder et de l’étain à souder.
- Un câble **USB-C capable de transférer des données** (certains câbles ne servent qu’à la charge).
- Un ordinateur avec Arduino IDE.

| Indispensable | Pratique, mais facultatif |
| --- | --- |
| 🔥 Fer à souder | 🧵 Tresse à dessouder |
| 🧴 Étain à souder | ✨ Flux de soudure |
| 🔌 Câble USB-C avec données | |
| 💻 Ordinateur avec Arduino IDE | |

La tresse à dessouder et le flux peuvent être utiles pour corriger une soudure
ou faciliter le travail, mais ils ne sont pas nécessaires pour assembler le kit.

### 🔎 Repérer les composants

Vérifiez la [BOM à la racine du dépôt](kitkodatht.csv) avant de commencer.
La sérigraphie imprimée sur le PCB indique où placer les composants et leurs
repères (`R1`, `SW1`, etc.). Respectez les valeurs et le sens des composants
polarisés indiqués par la BOM et la sérigraphie.

<a id="assembler-la-carte"></a>
## 🔥 1. Assembler la carte

> ⚠️ **Sécurité :** ne branchez pas la carte à l’USB pendant la soudure.

1. **📋 Triez** les composants et comparez-les à la [BOM](kitkodatht.csv).
2. **🔎 Repérez** les emplacements grâce aux références sur le PCB. Vérifiez la
   valeur des résistances et l’orientation des composants polarisés.
3. **🔥 Soudez** les composants traversants selon leur contour sur la
   sérigraphie. Pour les composants à plusieurs broches, alignez-les avant de
   souder toutes les broches.
4. **✂️ Nettoyez et inspectez** : coupez les pattes qui dépassent et vérifiez
   qu’il n’y a ni pont d’étain ni broche dessoudée.
5. **✅ Contrôlez** l’absence de débris ou d’excès d’étain pouvant provoquer un
   court-circuit.

> 💡 Le **module ESP32-S3 DevKitC** se connecte au PCB et se programme par son
> port USB-C. Une fois la soudure terminée et inspectée, branchez-le en USB-C
> pour configurer Arduino IDE et téléverser le firmware.

<a id="installer-arduino-ide"></a>
## ⚙️ 2. Installer Arduino IDE

1. Téléchargez et installez [Arduino IDE](https://www.arduino.cc/en/software).
2. **Ajoutez le support de l’ESP32** :
   - Ouvrez **Fichier → Préférences** (Windows/Linux) ou
     **Arduino IDE → Settings/Preferences** (macOS).
   - Dans **URL de gestionnaire de cartes supplémentaires**, ajoutez :
     `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Ouvrez **Outils → Type de carte → Gestionnaire de cartes**, recherchez
     **esp32** et installez **esp32 by Espressif Systems**.
3. **Installez les bibliothèques** depuis **Outils → Gérer les bibliothèques**.
   Recherchez puis installez chacune des bibliothèques indiquées dans
   [`requirements.txt`](requirements.txt) :
   - **Adafruit GFX Library**
   - **Adafruit SH110X**
   - **Adafruit BusIO**

<a id="televerser-le-firmware"></a>
## ⬆️ 3. Téléverser le firmware

1. 🔌 Branchez le module ESP32-S3 au PC avec un câble USB-C de données. Si le
   module est amovible, installez-le sur le PCB avant de le connecter.
2. 📄 Dans Arduino IDE, ouvrez le sketch
   [`firmware/main/main.ino`](firmware/main/main.ino).
3. 🎛️ Dans **Outils → Type de carte**, sélectionnez **ESP32S3 Dev Module**.
   Selon la version du paquet ESP32, **ESP32-S3-DevKitC-1** peut aussi être
   proposé.
4. 🔗 Dans **Outils → Port**, choisissez le port série apparu lors du
   branchement de la carte.
5. ✅ Cliquez sur **Vérifier** pour compiler, puis sur **Téléverser** pour
   envoyer le programme.
   - Si le téléversement reste bloqué sur la connexion, maintenez **BOOT** sur
     le module, lancez le téléversement, puis relâchez BOOT quand la connexion
     commence.
6. 🖥️ Ouvrez le **Moniteur série** et sélectionnez **9600 bauds** pour lire
   les messages de démarrage.

<a id="utiliser-la-carte"></a>
## 🎮 4. Utiliser la carte

Au démarrage, l’écran affiche l’accueil et le buzzer joue une mélodie. Utilisez
les boutons de la carte :

| Bouton | Action |
| --- | --- |
| ⏺️ **Central** | Quitter l’accueil, puis faire progresser l’éclosion de l’œuf. |
| ◀️ **Gauche** | Revenir à l’écran de menu précédent. |
| ▶️ **Droit** | Passer à l’écran de menu suivant. |

🥚 **Pour commencer :** appuyez une fois au centre pour quitter l’accueil, puis
appuyez encore quatre fois pour faire éclore l’œuf. Dans le menu, gauche et
droite font défiler les écrans : santé, compétences, statistiques, argent,
animal, nourrir, apprendre, jouer et expérimenter.

> 🚧 **État du firmware :** les écrans sont affichés et peuvent être parcourus.
> Les actions détaillées des menus ne sont pas encore reliées à une sélection.

<a id="depannage"></a>
## 🛠️ Dépannage

- 🔌 **La carte n’apparaît pas dans la liste des ports :** essayez un autre câble
  USB-C (avec données), un autre port USB ou installez le pilote USB adapté à
  votre module.
- ⬆️ **Le téléversement ne démarre pas :** vérifiez la carte et le port
  sélectionnés, puis essayez la procédure avec le bouton BOOT décrite plus haut.
- 📚 **La compilation échoue sur une bibliothèque :** installez ou mettez à jour
  les trois bibliothèques Adafruit indiquées dans
  [`requirements.txt`](requirements.txt).
- 🖥️ **L’écran reste vide ou un composant ne répond pas :** débranchez l’USB,
  vérifiez la BOM, l’orientation et les soudures, puis rebranchez la carte.
- 📟 **Le moniteur série affiche des caractères illisibles :** sélectionnez
  **9600 bauds**.

<a id="fichiers-du-projet"></a>
## 📂 Fichiers du projet

- [`kitkodatht/`](kitkodatht/) — schéma, PCB et projet KiCad.
- [`kitkodatht.csv`](kitkodatht.csv) — BOM de référence du kit.
- [`firmware/main/`](firmware/main/) — firmware Arduino.
- [`visuels/pcb.png`](visuels/pcb.png) — aperçu du PCB.

---

<p align="center">
  <strong>🧡 Bon assemblage et bon amusement !</strong><br>
  Un doute ? Vérifiez d’abord la BOM et la sérigraphie avant de mettre la carte sous tension.
</p>
