#include <VirtualWire.h>

int len = 0;

/* Constantes pour les broches */
const byte TX_PIN = 12;
const byte TRIGGER_PIN = 8; // Broche TRIGGER
const byte ECHO_PIN = 9;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/** Fonction setup() */
void setup() {
   
  /* Initialise le port série */
  Serial.begin(9600);
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);

  vw_set_tx_pin(12);
  vw_setup(2000); 
}
 
/** Fonction loop() */
void loop() {
   byte message[VW_MAX_MESSAGE_LEN]; 
   // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire
  
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
   
  /* Affiche les résultats en mm, cm et m */
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));

int distance_mm_r = distance_mm;
sprintf(message, "%4d mm", distance_mm_r); 
   len = strlen(message);
Serial.println("send"); 
  vw_send(message, len + 1); // On envoie le message (len + 1 pour tenir compte du caractère de fin de string)
  Serial.println("wait"); 
  vw_wait_tx(); // On attend la fin de l'envoi
Serial.println("sent"); 
  
  /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
  delay(2000);
}
