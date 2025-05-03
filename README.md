# Tutoriel Multitâche FreeRTOS pour ESP32

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![Licence](https://img.shields.io/badge/licence-MIT-green.svg)
![ESP32](https://img.shields.io/badge/plateforme-ESP32-red.svg)
![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-orange.svg)

Un tutoriel complet et progressif pour apprendre à utiliser les fonctionnalités multitâche de FreeRTOS sur l'ESP32. Ce projet unique combine théorie et pratique à travers un programme évolutif qui couvre tous les concepts fondamentaux.

![ESP32 FreeRTOS](https://via.placeholder.com/800x400?text=ESP32+FreeRTOS+Tutorial)

## 📋 Contenu du tutoriel

Le tutoriel est structuré en chapitres progressifs, chacun se concentrant sur un aspect spécifique du multitâche:

1. **Introduction aux tâches** - Création et gestion de tâches simples
2. **Communication entre tâches** - Utilisation des files d'attente (Queues)
3. **Synchronisation avec sémaphores** - Sémaphores binaires et de comptage
4. **Notifications de tâches** - Alternative légère aux sémaphores
5. **Mutexes et sections critiques** - Protection des ressources partagées
6. **Minuteries logicielles** - Actions périodiques et temporisées
7. **Projet pratique** - Système de surveillance de température

## 🛠️ Prérequis

### Matériel
- Carte de développement ESP32 (toutes variantes)
- LEDs (2-3) ou module LED
- Potentiomètre (pour simuler un capteur analogique)
- Bouton-poussoir (optionnel)
- Breadboard et fils de connexion

### Logiciels
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Support ESP32 pour Arduino](https://github.com/espressif/arduino-esp32)

## 🔌 Schéma de câblage

Connexions minimales pour suivre le tutoriel:

| Composant | Broche ESP32 | Description |
|-----------|--------------|-------------|
| LED1      | GPIO2        | LED principale (intégrée sur la plupart des cartes) |
| LED2      | GPIO4        | LED secondaire |
| LED Alarme| GPIO5        | LED d'alarme (pour le projet final) |
| Bouton    | GPIO15       | Entrée bouton-poussoir |
| Potentiomètre | GPIO34   | Entrée analogique (simuler un capteur) |

## 📥 Installation et utilisation

1. Clonez ce dépôt:
   ```bash
   git clone https://github.com/votre-utilisateur/esp32-freertos-tutorial.git
   ```

2. Ouvrez le fichier `ESP32_FreeRTOS_Tutorial.ino` dans l'IDE Arduino

3. Assurez-vous que le support ESP32 est installé (via le gestionnaire de cartes)

4. Sélectionnez votre carte ESP32 dans le menu Outils > Type de carte

5. **Navigation entre les chapitres:**
   - Par défaut, le CHAPITRE 1 est actif (fonctions `setup()`, `loop()`, `task1Function()`, `task2Function()`)
   - Pour passer à un autre chapitre:
     - Commentez les fonctions `setup()` et `loop()` actuelles
     - Décommentez le bloc de code du chapitre souhaité
     - Renommez `setupChX()` en `setup()` (par ex. `setupCh2()` → `setup()`)

6. Téléversez le code sur votre ESP32

7. Ouvrez le moniteur série (115200 bauds) pour voir les sorties

## 📊 Concepts clés FreeRTOS couverts

- **Tâches**: Création, priorités, suspension/reprise
- **Files d'attente**: Communication entre tâches
- **Sémaphores**: Binaires et de comptage pour synchronisation
- **Mutexes**: Protection des ressources partagées
- **Sections critiques**: Désactivation temporaire des interruptions
- **Notifications de tâches**: Alternative légère aux sémaphores
- **Minuteries logicielles**: Actions périodiques sans tâches dédiées

## 🎓 Exercices pratiques

Le tutoriel inclut des exercices pratiques à la fin du fichier pour renforcer vos connaissances:

1. **Exercice 1**: Création de tâches avec paramètres
2. **Exercice 2**: Communication entre tâches via files d'attente
3. **Exercice 3**: Synchronisation avec sémaphores binaires

## 🔧 Personnalisation

Chaque chapitre peut être facilement modifié pour:
- Utiliser différentes broches GPIO
- Ajouter des fonctionnalités supplémentaires
- Intégrer d'autres capteurs
- Expérimenter avec différentes priorités de tâches

## 🚀 Extensions possibles

- Connectivité WiFi/Bluetooth avec tâches dédiées
- Interface web pour le projet de surveillance
- Stockage de données sur carte SD
- Communication MQTT avec serveurs cloud
- Gestion avancée d'énergie avec modes veille

## 📚 Ressources additionnelles

- [Documentation officielle FreeRTOS](https://www.freertos.org/Documentation/RTOS_book.html)
- [Documentation ESP-IDF (FreeRTOS)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
- [API Arduino pour ESP32](https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/)
- [Tutoriels vidéo recommandés](https://www.youtube.com/results?search_query=freertos+esp32+tutorial)

## 🤝 Contributions

Les contributions sont les bienvenues! Voici comment vous pouvez contribuer:

1. Fork du projet
2. Création d'une branche pour votre fonctionnalité (`git checkout -b feature/amazing-feature`)
3. Commit de vos changements (`git commit -m 'Add some amazing feature'`)
4. Push vers la branche (`git push origin feature/amazing-feature`)
5. Ouverture d'une Pull Request

## 📜 Licence

Ce projet est distribué sous licence MIT. Voir le fichier `LICENSE` pour plus d'informations.

## 📞 Contact

Votre Nom - [@twitter_handle](https://twitter.com/twitter_handle) - email@example.com

Lien du projet: [https://github.com/votre-utilisateur/esp32-freertos-tutorial](https://github.com/votre-utilisateur/esp32-freertos-tutorial)Enter file contents here
