#include <VirtualWire.h>

const byte TX_PIN = 12;
const byte TRIGGER_PIN = 8; // Broche TRIGGER
const byte ECHO_PIN = 9;    // Broche ECHO

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
  
  vw_set_tx_pin(TX_PIN);
  vw_setup(2000); 
  Serial.println("Go sending !"); 
}

void loop() {
  byte message[VW_MAX_MESSAGE_LEN]; 
  // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire
  
  // Lit un message de maximum 26 caractères depuis le port série
//  int len = Serial.readBytesUntil('\n', (char*) message, VW_MAX_MESSAGE_LEN - 1);
//  if (!len) {
//    return; // Pas de message
//  }
//  message[len] = '\0'; // Ferme la chaine de caractères
strcpy(message, "Hello");
message[5] = '\0';
  Serial.println("send"); 
  vw_send(message, 5); // On envoie le message
  Serial.println("wait"); 
  vw_wait_tx(); // On attend la fin de l'envoi
Serial.println("sent"); 
delay(1000);

}
