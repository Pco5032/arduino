#include <VirtualWire.h>

void setup() {
  Serial.begin(9600);
  vw_set_tx_pin(10);
  vw_setup(2000); 
  Serial.println("Go sending !"); 
}

void loop() {
  byte message[VW_MAX_MESSAGE_LEN]; 
  // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire
  
  // Lit un message de maximum 26 caractères depuis le port série
  int len = Serial.readBytesUntil('\n', (char*) message, VW_MAX_MESSAGE_LEN - 1);
  if (!len) {
    return; // Pas de message
  }
  message[len] = '\0'; // Ferme la chaine de caractères

  Serial.println("send"); 
  vw_send(message, len + 1); // On envoie le message
  Serial.println("wait"); 
  vw_wait_tx(); // On attend la fin de l'envoi
Serial.println("sent"); 

}
