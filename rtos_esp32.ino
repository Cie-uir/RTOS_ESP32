/**
 * TUTORIEL: MULTITÂCHE AVEC FREERTOS SUR ESP32
 * ============================================
 * 
 * Ce programme sert de tutoriel progressif pour apprendre
 * à utiliser les fonctionnalités multitâche de FreeRTOS sur ESP32.
 * 
 * Author: Claude
 * Date: Mai 2025
 */

#include <Arduino.h>

// CHAPITRE 1: INTRODUCTION AUX TÂCHES
// ====================================

// Une tâche (task) est une fonction qui s'exécute indépendamment
// et parallèlement à d'autres tâches. Chaque tâche a sa propre pile
// et sa propre priorité.

// Structure du tutoriel:
// 1. Création et gestion de tâches simples
// 2. Communication entre tâches (files d'attente)
// 3. Synchronisation entre tâches (sémaphores)
// 4. Notifications de tâches
// 5. Mutexes et sections critiques
// 6. Minuteries logicielles
// 7. Projet pratique

// Configuration des broches (pour les exemples)
#define LED1_PIN 2  // LED embarquée sur la plupart des cartes ESP32
#define LED2_PIN 4  
#define BUTTON_PIN 15

// CHAPITRE 1: CRÉATION ET GESTION DE TÂCHES SIMPLES
// =================================================

// Déclaration des handles (poignées) de tâches
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

// Prototype des fonctions de tâches
void task1Function(void *parameter);
void task2Function(void *parameter);

void setup() {
  // Initialisation de la communication série
  Serial.begin(115200);
  delay(1000); // Temps pour ouvrir le moniteur série
  
  Serial.println("==============================================");
  Serial.println("TUTORIEL MULTITÂCHE AVEC FREERTOS SUR ESP32");
  Serial.println("==============================================");
  Serial.println();
  
  // Configuration des broches
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("CHAPITRE 1: CRÉATION ET GESTION DE TÂCHES");
  Serial.println("-----------------------------------------");
  
  // Création de tâches
  Serial.println("Création de deux tâches indépendantes...");
  
  /*
   * xTaskCreate(
   *    TaskFunction_t pvTaskCode,       // Fonction qui implémente la tâche
   *    const char * const pcName,       // Nom de la tâche (pour débogage)
   *    configSTACK_DEPTH,              // Taille de la pile (en mots)
   *    void *pvParameters,              // Paramètres passés à la tâche
   *    UBaseType_t uxPriority,          // Priorité de la tâche
   *    TaskHandle_t *pxCreatedTask      // Handle de la tâche (optionnel)
   * );
   */
  
  // Création de la première tâche (priorité 1)
  xTaskCreate(
    task1Function,        // Fonction de la tâche
    "Task1",              // Nom pour le débogage
    2048,                 // Taille de pile en mots (2048 est souvent suffisant)
    NULL,                 // Pas de paramètre
    1,                    // Priorité 1 (basse priorité)
    &task1Handle          // Handle de la tâche
  );
  
  // Création de la deuxième tâche (priorité 2)
  xTaskCreate(
    task2Function,        // Fonction de la tâche
    "Task2",              // Nom pour le débogage
    2048,                 // Taille de pile
    NULL,                 // Pas de paramètre
    2,                    // Priorité 2 (plus haute priorité que task1)
    &task2Handle          // Handle de la tâche
  );
  
  Serial.println("Tâches créées avec succès!");
  Serial.println("- Task1: Priorité 1, clignote la LED1 lentement");
  Serial.println("- Task2: Priorité 2, clignote la LED2 rapidement");
  Serial.println();
  
  Serial.println("La fonction setup() est maintenant terminée.");
  Serial.println("Les tâches continuent à s'exécuter indépendamment.");
  Serial.println();
}

// Fonction principale de la première tâche
void task1Function(void *parameter) {
  Serial.println("Task1: Démarrage");
  
  // Boucle infinie (comme loop())
  for(;;) {
    digitalWrite(LED1_PIN, HIGH);
    Serial.println("Task1: LED1 allumée");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Délai de 1 seconde
    
    digitalWrite(LED1_PIN, LOW);
    Serial.println("Task1: LED1 éteinte");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Délai de 1 seconde
  }
  
  // Une tâche ne devrait jamais retourner, mais si cela arrivait:
  vTaskDelete(NULL); // Supprime la tâche actuelle
}

// Fonction principale de la deuxième tâche
void task2Function(void *parameter) {
  Serial.println("Task2: Démarrage");
  
  // Boucle infinie
  for(;;) {
    digitalWrite(LED2_PIN, HIGH);
    Serial.println("Task2: LED2 allumée");
    vTaskDelay(200 / portTICK_PERIOD_MS); // Délai de 200ms
    
    digitalWrite(LED2_PIN, LOW);
    Serial.println("Task2: LED2 éteinte");
    vTaskDelay(200 / portTICK_PERIOD_MS); // Délai de 200ms
  }
  
  vTaskDelete(NULL); // Supprime la tâche actuelle (ne devrait jamais être exécuté)
}

// La fonction loop n'est pas utilisée car notre code s'exécute dans les tâches
// Elle est exécutée par la boucle principale Arduino, qui est une tâche FreeRTOS
void loop() {
  // La boucle loop est exécutée en parallèle des tâches que nous avons créées
  // Nous pouvons l'utiliser pour surveiller ou contrôler les tâches
  
  // Exemple: Suspendre/reprendre task1 avec un bouton
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);
  
  // Détection d'un appui sur le bouton (front descendant)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Si task1 est en cours d'exécution
    if (eTaskGetState(task1Handle) != eSuspended) {
      Serial.println("Main: Suspension de Task1");
      vTaskSuspend(task1Handle);
    } else {
      Serial.println("Main: Reprise de Task1");
      vTaskResume(task1Handle);
    }
    delay(50); // Debounce
  }
  
  lastButtonState = buttonState;
  delay(10); // Courte pause pour ne pas surcharger le CPU
}

// CHAPITRE 2: COMMUNICATION ENTRE TÂCHES (FILES D'ATTENTE)
// ========================================================

// Cette section sera décommentée pour le chapitre 2
/*
#include <queue.h>

// Déclaration d'une file d'attente
QueueHandle_t dataQueue;

// Nouvelle tâche pour envoyer des données
void senderTask(void *parameter) {
  int counter = 0;
  
  for(;;) {
    // Envoyer la valeur counter à la file d'attente
    if (xQueueSend(dataQueue, &counter, portMAX_DELAY) == pdPASS) {
      Serial.printf("Sender: Envoi de la valeur %d\n", counter);
      counter++;
    } else {
      Serial.println("Sender: Échec de l'envoi");
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Nouvelle tâche pour recevoir des données
void receiverTask(void *parameter) {
  int receivedValue;
  
  for(;;) {
    // Recevoir une valeur de la file d'attente
    if (xQueueReceive(dataQueue, &receivedValue, portMAX_DELAY) == pdPASS) {
      Serial.printf("Receiver: Réception de la valeur %d\n", receivedValue);
      
      // Afficher la valeur sur la LED en binaire
      digitalWrite(LED1_PIN, receivedValue & 1);
      digitalWrite(LED2_PIN, (receivedValue >> 1) & 1);
    }
  }
}

// POUR CHAPITRE 2: Décommentez ce bloc pour remplacer setup()
void setupCh2() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("CHAPITRE 2: COMMUNICATION ENTRE TÂCHES (FILES D'ATTENTE)");
  Serial.println("--------------------------------------------------------");
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Création d'une file d'attente qui peut contenir 10 entiers
  dataQueue = xQueueCreate(10, sizeof(int));
  
  if (dataQueue != NULL) {
    Serial.println("File d'attente créée avec succès");
    
    // Créer les tâches d'envoi et de réception
    xTaskCreate(
      senderTask,
      "Sender",
      2048,
      NULL,
      1,
      NULL
    );
    
    xTaskCreate(
      receiverTask,
      "Receiver",
      2048,
      NULL,
      1,
      NULL
    );
    
    Serial.println("Tâches créées");
    Serial.println("La tâche Sender enverra des valeurs croissantes");
    Serial.println("La tâche Receiver affichera ces valeurs sur les LEDs en binaire");
  } else {
    Serial.println("Échec de la création de la file d'attente");
  }
}
*/

// CHAPITRE 3: SYNCHRONISATION AVEC SÉMAPHORES
// ===========================================

// Cette section sera décommentée pour le chapitre 3
/*
#include <semphr.h>

// Déclaration des sémaphores
SemaphoreHandle_t binarySemaphore;
SemaphoreHandle_t countingSemaphore;

// Tâche qui utilise un sémaphore binaire
void binarySemaphoreTask(void *parameter) {
  for(;;) {
    // Attendre que le sémaphore soit disponible
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE) {
      // Section protégée par le sémaphore
      Serial.println("BSTask: Sémaphore obtenu");
      digitalWrite(LED1_PIN, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED1_PIN, LOW);
      
      // Libérer le sémaphore
      xSemaphoreGive(binarySemaphore);
      Serial.println("BSTask: Sémaphore libéré");
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tâche qui utilise un sémaphore de comptage
void countingSemaphoreTask(void *parameter) {
  int taskId = (int)parameter;
  
  for(;;) {
    // Attendre que le sémaphore soit disponible
    if (xSemaphoreTake(countingSemaphore, portMAX_DELAY) == pdTRUE) {
      // Section protégée par le sémaphore
      Serial.printf("CSTask %d: Entrée dans la section critique\n", taskId);
      digitalWrite(LED2_PIN, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED2_PIN, LOW);
      
      // Libérer le sémaphore
      xSemaphoreGive(countingSemaphore);
      Serial.printf("CSTask %d: Sortie de la section critique\n", taskId);
    }
    
    vTaskDelay((taskId * 200) / portTICK_PERIOD_MS);
  }
}

// POUR CHAPITRE 3: Décommentez ce bloc pour remplacer setup()
void setupCh3() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("CHAPITRE 3: SYNCHRONISATION AVEC SÉMAPHORES");
  Serial.println("-----------------------------------------");
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Création d'un sémaphore binaire
  binarySemaphore = xSemaphoreCreateBinary();
  
  // Initialiser le sémaphore comme disponible
  xSemaphoreGive(binarySemaphore);
  
  // Création d'un sémaphore de comptage avec une valeur maximale de 3
  countingSemaphore = xSemaphoreCreateCounting(3, 3);
  
  if (binarySemaphore != NULL && countingSemaphore != NULL) {
    Serial.println("Sémaphores créés avec succès");
    
    // Créer la tâche qui utilise le sémaphore binaire
    xTaskCreate(
      binarySemaphoreTask,
      "BinarySem",
      2048,
      NULL,
      1,
      NULL
    );
    
    // Créer plusieurs tâches qui utilisent le sémaphore de comptage
    for (int i = 0; i < 5; i++) {
      xTaskCreate(
        countingSemaphoreTask,
        "CountSem",
        2048,
        (void*)i,  // Passer l'id de la tâche comme paramètre
        1,
        NULL
      );
    }
    
    Serial.println("Tâches créées");
    Serial.println("Le sémaphore binaire permet à une seule tâche d'accéder à une ressource");
    Serial.println("Le sémaphore de comptage permet à plusieurs tâches (3 max) d'accéder à une ressource");
  } else {
    Serial.println("Échec de la création des sémaphores");
  }
}
*/

// CHAPITRE 4: NOTIFICATIONS DE TÂCHES
// ===================================

// Cette section sera décommentée pour le chapitre 4
/*
// Handles des tâches pour les notifications
TaskHandle_t notifiedTaskHandle = NULL;
TaskHandle_t notifierTaskHandle = NULL;

// Tâche qui attend une notification
void notifiedTask(void *parameter) {
  uint32_t notificationValue;
  
  for(;;) {
    // Attendre une notification avec un délai maximum de 5 secondes
    if (xTaskNotifyWait(
          0,                    // N'efface aucun bit lors de l'entrée
          ULONG_MAX,            // Efface tous les bits lors de la sortie
          &notificationValue,   // Stocke la valeur de notification ici
          pdMS_TO_TICKS(5000)   // Attendre au maximum 5 secondes
        ) == pdPASS) {
      
      // Notification reçue
      Serial.printf("NotifiedTask: Notification reçue avec valeur %lu\n", notificationValue);
      
      // Interprétation des différentes commandes dans la notification
      if (notificationValue & (1 << 0)) {
        // Bit 0 activé: clignoter rapidement
        for (int i = 0; i < 5; i++) {
          digitalWrite(LED1_PIN, HIGH);
          vTaskDelay(100 / portTICK_PERIOD_MS);
          digitalWrite(LED1_PIN, LOW);
          vTaskDelay(100 / portTICK_PERIOD_MS);
        }
      }
      
      if (notificationValue & (1 << 1)) {
        // Bit 1 activé: allumer fixe
        digitalWrite(LED1_PIN, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(LED1_PIN, LOW);
      }
      
      if (notificationValue & (1 << 2)) {
        // Bit 2 activé: clignoter lentement
        for (int i = 0; i < 2; i++) {
          digitalWrite(LED1_PIN, HIGH);
          vTaskDelay(500 / portTICK_PERIOD_MS);
          digitalWrite(LED1_PIN, LOW);
          vTaskDelay(500 / portTICK_PERIOD_MS);
        }
      }
    } else {
      // Délai expiré sans notification
      Serial.println("NotifiedTask: Aucune notification reçue pendant 5 secondes");
      // Clignoter la LED pour indiquer l'absence de notification
      digitalWrite(LED1_PIN, HIGH);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalWrite(LED1_PIN, LOW);
    }
  }
}

// Tâche qui envoie des notifications
void notifierTask(void *parameter) {
  int command = 0;
  
  for(;;) {
    // Envoi d'une notification avec une valeur différente à chaque fois
    command = (command + 1) % 8;
    
    Serial.printf("NotifierTask: Envoi de la commande %d\n", command);
    
    if (command > 0) {
      xTaskNotify(
        notifiedTaskHandle,    // Handle de la tâche à notifier
        1 << (command - 1),    // Valeur de notification (puissance de 2)
        eSetBits               // Opération: définir les bits
      );
    } else {
      // Commande 0: ne rien faire (pause)
      Serial.println("NotifierTask: Pause (aucune notification envoyée)");
    }
    
    vTaskDelay(6000 / portTICK_PERIOD_MS);  // Attendre 6 secondes
  }
}

// POUR CHAPITRE 4: Décommentez ce bloc pour remplacer setup()
void setupCh4() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("CHAPITRE 4: NOTIFICATIONS DE TÂCHES");
  Serial.println("----------------------------------");
  
  pinMode(LED1_PIN, OUTPUT);
  
  // Créer la tâche qui attend des notifications
  xTaskCreate(
    notifiedTask,
    "Notified",
    2048,
    NULL,
    1,
    &notifiedTaskHandle
  );
  
  // Créer la tâche qui envoie des notifications
  xTaskCreate(
    notifierTask,
    "Notifier",
    2048,
    NULL,
    2,  // Priorité plus élevée
    &notifierTaskHandle
  );
  
  Serial.println("Tâches créées");
  Serial.println("NotifierTask: Envoie des notifications avec différentes commandes");
  Serial.println("NotifiedTask: Reçoit les notifications et agit en conséquence");
}
*/

// CHAPITRE 5: MUTEXES ET SECTIONS CRITIQUES
// =========================================

// Cette section sera décommentée pour le chapitre 5
/*
#include <semphr.h>

// Déclaration d'un mutex
SemaphoreHandle_t xMutex;

// Variable partagée entre les tâches
volatile int sharedCounter = 0;

// Tâche qui incrémente le compteur partagé
void incrementTask(void *parameter) {
  int taskId = (int)parameter;
  
  for(;;) {
    // Méthode 1: Utilisation d'un mutex
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
      // Section critique protégée par mutex
      Serial.printf("IncrementTask %d: Entrée en section critique (Mutex)\n", taskId);
      
      // Simuler une opération qui prend du temps
      int tempValue = sharedCounter;
      vTaskDelay(random(10, 100) / portTICK_PERIOD_MS);
      tempValue++;
      sharedCounter = tempValue;
      
      Serial.printf("IncrementTask %d: Compteur = %d\n", taskId, sharedCounter);
      
      xSemaphoreGive(xMutex);
      Serial.printf("IncrementTask %d: Sortie de section critique (Mutex)\n", taskId);
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Méthode 2: Utilisation d'une section critique
    Serial.printf("IncrementTask %d: Entrée en section critique (portENTER_CRITICAL)\n", taskId);
    
    // Désactiver les interruptions pendant cette section
    portENTER_CRITICAL(&spinlock);
    
    // Section critique (très courte!)
    sharedCounter++;
    digitalWrite(LED1_PIN, sharedCounter % 2);
    
    portEXIT_CRITICAL(&spinlock);
    
    Serial.printf("IncrementTask %d: Sortie de section critique (portEXIT_CRITICAL), Compteur = %d\n", taskId, sharedCounter);
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// POUR CHAPITRE 5: Décommentez ce bloc pour remplacer setup()
void setupCh5() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("CHAPITRE 5: MUTEXES ET SECTIONS CRITIQUES");
  Serial.println("----------------------------------------");
  
  pinMode(LED1_PIN, OUTPUT);
  
  // Initialisation du mutex
  xMutex = xSemaphoreCreateMutex();
  
  if (xMutex != NULL) {
    Serial.println("Mutex créé avec succès");
    
    // Créer plusieurs tâches qui essaient d'incrémenter le compteur
    for (int i = 0; i < 3; i++) {
      xTaskCreate(
        incrementTask,
        "Increment",
        2048,
        (void*)i,  // Passer l'id de la tâche comme paramètre
        1,
        NULL
      );
    }
    
    Serial.println("Tâches créées");
    Serial.println("Chaque tâche incrémente une variable partagée");
    Serial.println("1. En utilisant un mutex (permet des sections critiques longues)");
    Serial.println("2. En utilisant portENTER_CRITICAL (pour des sections très courtes)");
  } else {
    Serial.println("Échec de la création du mutex");
  }
}
*/

// CHAPITRE 6: MINUTERIES LOGICIELLES
// ==================================

// Cette section sera décommentée pour le chapitre 6
/*
#include <timers.h>

// Handles des minuteries
TimerHandle_t xOneShotTimer;
TimerHandle_t xAutoReloadTimer;

// ID des minuteries pour les identifier dans le callback
#define ONESHOT_TIMER_ID   1
#define AUTORELOAD_TIMER_ID 2

// Fonction de callback commune pour toutes les minuteries
void timerCallback(TimerHandle_t xTimer) {
  // Identifier quelle minuterie a lancé le callback
  uint32_t timerId = (uint32_t)pvTimerGetTimerID(xTimer);
  
  if (timerId == ONESHOT_TIMER_ID) {
    Serial.println("Callback: Minuterie one-shot déclenchée!");
    digitalWrite(LED1_PIN, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(LED1_PIN, LOW);
  } else if (timerId == AUTORELOAD_TIMER_ID) {
    Serial.println("Callback: Minuterie auto-reload déclenchée!");
    digitalWrite(LED2_PIN, !digitalRead(LED2_PIN)); // Inverser l'état
  }
}

// POUR CHAPITRE 6: Décommentez ce bloc pour remplacer setup()
void setupCh6() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("CHAPITRE 6: MINUTERIES LOGICIELLES");
  Serial.println("--------------------------------");
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Création d'une minuterie à déclenchement unique (one-shot)
  xOneShotTimer = xTimerCreate(
    "OneShotTimer",               // Nom pour débogage
    pdMS_TO_TICKS(10000),         // Période de 10 secondes
    pdFALSE,                      // Auto-reload désactivé (one-shot)
    (void*)ONESHOT_TIMER_ID,      // ID du timer
    timerCallback                 // Fonction callback
  );
  
  // Création d'une minuterie à rechargement automatique (auto-reload)
  xAutoReloadTimer = xTimerCreate(
    "AutoReloadTimer",            // Nom pour débogage
    pdMS_TO_TICKS(2000),          // Période de 2 secondes
    pdTRUE,                       // Auto-reload activé
    (void*)AUTORELOAD_TIMER_ID,   // ID du timer
    timerCallback                 // Fonction callback
  );
  
  if (xOneShotTimer != NULL && xAutoReloadTimer != NULL) {
    Serial.println("Minuteries créées avec succès");
    
    // Démarrer les minuteries
    xTimerStart(xOneShotTimer, 0);
    xTimerStart(xAutoReloadTimer, 0);
    
    Serial.println("Minuteries démarrées");
    Serial.println("Minuterie one-shot: Se déclenchera une fois après 10 secondes");
    Serial.println("Minuterie auto-reload: Se déclenchera toutes les 2 secondes");
  } else {
    Serial.println("Échec de la création des minuteries");
  }
}
*/

// CHAPITRE 7: PROJET PRATIQUE - SYSTÈME DE SURVEILLANCE DE TEMPÉRATURE
// ====================================================================

// Cette section sera décommentée pour le chapitre 7
/*
#include <queue.h>
#include <semphr.h>
#include <timers.h>

// Broches supplémentaires
#define TEMP_SENSOR_PIN 35  // Capteur de température (simulé avec potentiomètre)
#define ALARM_LED_PIN 5     // LED d'alarme

// Seuil d'alarme de température (valeur simulée)
#define TEMP_THRESHOLD 30

// Handles des files et sémaphores
QueueHandle_t tempQueue;
SemaphoreHandle_t displayMutex;
TimerHandle_t sampleTimer;

// Structure pour les données de température
typedef struct {
  float temperature;
  unsigned long timestamp;
} TempData_t;

// Tâche de lecture du capteur
void tempSensorTask(void *parameter) {
  TempData_t tempData;
  
  for(;;) {
    // Lecture analogique (simulate temperature with potentiometer)
    int rawValue = analogRead(TEMP_SENSOR_PIN);
    
    // Convertir en température simulée (0-50°C)
    tempData.temperature = map(rawValue, 0, 4095, 0, 50);
    tempData.timestamp = millis();
    
    // Envoyer à la file d'attente
    if (xQueueSend(tempQueue, &tempData, 0) != pdPASS) {
      Serial.println("TempSensor: File d'attente pleine!");
    }
    
    // Attendre avant la prochaine lecture
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Tâche de traitement des données
void tempProcessTask(void *parameter) {
  TempData_t tempData;
  float avgTemp = 0;
  const int windowSize = 10;
  int sampleCount = 0;
  
  for(;;) {
    // Récupérer les données de température
    if (xQueueReceive(tempQueue, &tempData, portMAX_DELAY) == pdPASS) {
      // Calculer une moyenne mobile
      avgTemp = (avgTemp * sampleCount + tempData.temperature) / (sampleCount + 1);
      sampleCount = min(sampleCount + 1, windowSize);
      
      // Prendre le mutex pour l'affichage
      if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE) {
        Serial.printf("Process: Temp=%.1f°C, Avg=%.1f°C, Time=%lu ms\n", 
                      tempData.temperature, avgTemp, tempData.timestamp);
        xSemaphoreGive(displayMutex);
      }
      
      // Vérifier le seuil d'alarme
      if (tempData.temperature > TEMP_THRESHOLD) {
        // Allumer la LED d'alarme
        digitalWrite(ALARM_LED_PIN, HIGH);
        
        // Afficher un message d'alarme
        if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE) {
          Serial.printf("!!! ALARME !!! Température %.1f°C au-dessus du seuil de %.1f°C\n", 
                        tempData.temperature, (float)TEMP_THRESHOLD);
          xSemaphoreGive(displayMutex);
        }
      } else {
        // Éteindre la LED d'alarme
        digitalWrite(ALARM_LED_PIN, LOW);
      }
    }
  }
}

// Fonction de callback pour la minuterie d'échantillonnage
void sampleTimerCallback(TimerHandle_t xTimer) {
  // Cette fonction est appelée périodiquement
  static int sampleCount = 0;
  sampleCount++;
  
  // Prendre le mutex pour l'affichage
  if (xSemaphoreTake(displayMutex, 0) == pdTRUE) {
    Serial.printf("Timer: Échantillon #%d pris\n", sampleCount);
    xSemaphoreGive(displayMutex);
  }
  
  // Clignoter la LED1 pour indiquer l'échantillonnage
  digitalWrite(LED1_PIN, HIGH);
  vTaskDelay(50 / portTICK_PERIOD_MS);
  digitalWrite(LED1_PIN, LOW);
}

// Tâche de contrôle par l'utilisateur
void userControlTask(void *parameter) {
  char cmd;
  
  for(;;) {
    // Vérifier si des commandes sont disponibles sur le port série
    if (Serial.available() > 0) {
      cmd = Serial.read();
      
      // Prendre le mutex pour l'affichage
      xSemaphoreTake(displayMutex, portMAX_DELAY);
      
      switch (cmd) {
        case 'r': // Reset
          Serial.println("Commande: Reset des données");
          break;
        case 'i': // Info
          Serial.println("Commande: Affichage des infos");
          Serial.println("-- Statistiques Système --");
          Serial.printf("Mémoire libre: %lu octets\n", esp_get_free_heap_size());
          Serial.printf("Pile libre Min Task1: %lu mots\n", uxTaskGetStackHighWaterMark(NULL));
          break;
        case 'h': // Help
          Serial.println("Commandes disponibles:");
          Serial.println("r - Reset des données");
          Serial.println("i - Afficher les infos système");
          Serial.println("h - Aide");
          break;
        default:
          Serial.println("Commande inconnue. Tapez 'h' pour l'aide.");
      }
      
      // Libérer le mutex
      xSemaphoreGive(displayMutex);
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// POUR CHAPITRE 7: Décommentez ce bloc pour remplacer setup()
void setupCh7() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("============================================================");
  Serial.println("CHAPITRE 7: PROJET PRATIQUE - SYSTÈME DE SURVEILLANCE");
  Serial.println("============================================================");
  
  // Configuration des broches
  pinMode(LED1_PIN, OUTPUT);
  pinMode(ALARM_LED_PIN, OUTPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  
  // Création de la file d'attente pour les données de température
  tempQueue = xQueueCreate(20, sizeof(TempData_t));
  
  // Création du mutex pour l'affichage
  displayMutex = xSemaphoreCreateMutex();
  
  // Création de la minuterie d'échantillonnage
  sampleTimer = xTimerCreate(
    "SampleTimer",
    pdMS_TO_TICKS(5000),  // Toutes les 5 secondes
    pdTRUE,               // Auto-reload
    0,                    // ID
    sampleTimerCallback   // Fonction callback
  );
  
  if (tempQueue != NULL && displayMutex != NULL && sampleTimer != NULL) {
    Serial.println("Ressources créées avec succès");
    
    // Créer les tâches
    xTaskCreate(
      tempSensorTask,
      "TempSensor",
      2048,
      NULL,
      2,  // Priorité plus élevée pour la lecture du capteur
      NULL
    );
    
    xTaskCreate(
      tempProcessTask,
      "TempProcess",
      2048,
      NULL,
      1,
      NULL
    );
    
    xTaskCreate(
      userControlTask,
      "UserControl",
      2048,
      NULL,
      1,
      NULL
    );
    
    // Démarrer la minuterie
    xTimerStart(sampleTimer, 0);
    
    Serial.println("Système de surveillance démarré");
    Serial.println("Tapez 'h' pour voir les commandes disponibles");
  } else {
    Serial.println("Échec de la création des ressources");
  }
}
*/

// ========================================================================
// BONUS: EXERCICES PRATIQUES AVEC SOLUTIONS
// ========================================================================

/*
EXERCICE 1: CRÉATION DE TÂCHES
------------------------------
Objectif: Créer une tâche qui affiche un message périodique

Solution:
void exerciceTask(void *parameter) {
  const char* message = (const char*)parameter;
  for(;;) {
    Serial.println(message);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setupExercice1() {
  Serial.begin(115200);
  
  // Créer la tâche avec un message en paramètre
  xTaskCreate(
    exerciceTask,
    "ExTask",
    2048,
    (void*)"Message périodique!",
    1,
    NULL
  );
}

EXERCICE 2: COMMUNICATION ENTRE TÂCHES
--------------------------------------
Objectif: Créer une tâche qui envoie des nombres à une autre tâche via une file d'attente

Solution:
QueueHandle_t numberQueue;

void senderTaskEx2(void *parameter) {
  int number = 0;
  for(;;) {
    if (xQueueSend(numberQueue, &number, 0) == pdPASS) {
      Serial.printf("Envoi: %d\n", number);
      number++;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void receiverTaskEx2(void *parameter) {
  int receivedNumber;
  for(;;) {
    if (xQueueReceive(numberQueue, &receivedNumber, portMAX_DELAY) == pdPASS) {
      Serial.printf("Réception: %d\n", receivedNumber);
    }
  }
}

void setupExercice2() {
  Serial.begin(115200);
  
  numberQueue = xQueueCreate(5, sizeof(int));
  
  xTaskCreate(senderTaskEx2, "Sender", 2048, NULL, 1, NULL);
  xTaskCreate(receiverTaskEx2, "Receiver", 2048, NULL, 1, NULL);
}

EXERCICE 3: UTILISATION DE SÉMAPHORES
-------------------------------------
Objectif: Utiliser un sémaphore binaire pour synchroniser deux tâches

Solution:
SemaphoreHandle_t syncSemaphore;

void firstTaskEx3(void *parameter) {
  for(;;) {
    Serial.println("Tâche 1: Travail en cours...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    Serial.println("Tâche 1: Travail terminé, signalant à la tâche 2");
    xSemaphoreGive(syncSemaphore);
  }
}

void secondTaskEx3(void *parameter) {
  for(;;) {
    Serial.println("Tâche 2: En attente du signal...");
    
    if (xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.println("Tâche 2: Signal reçu, exécution");
      
      // Simuler un traitement
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

void setupExercice3() {
  Serial.begin(115200);
  
  syncSemaphore = xSemaphoreCreateBinary();
  
  xTaskCreate(firstTaskEx3, "First", 2048, NULL, 1, NULL);
  xTaskCreate(secondTaskEx3, "Second", 2048, NULL, 1, NULL);
}
*/

// ========================================================================
// COMMENT UTILISER CE TUTORIEL
// ========================================================================

/*
Pour explorer les différents chapitres de ce tutoriel:

1. Maintenez le code du CHAPITRE 1 (sections setup(), task1Function(), task2Function(), loop())
   tel quel - c'est le chapitre actif par défaut.

2. Pour passer à un chapitre ultérieur:
   a. Commentez les fonctions setup() et loop() actuelles
   b. Décommentez le bloc de code du chapitre que vous souhaitez explorer
   c. Renommez la fonction setupChX() en setup() (par exemple setupCh2() -> setup())
   d. Téléversez le code modifié sur votre ESP32

3. N'hésitez pas à expérimenter et à modifier le code pour mieux comprendre
   comment fonctionne FreeRTOS sur l'ESP32.

Remarques importantes:
- Chaque chapitre est indépendant et illustre un concept spécifique
- Les exercices à la fin vous permettent de pratiquer ce que vous avez appris
- Consultez la documentation officielle de FreeRTOS et d'ESP32 pour plus de détails
*/
