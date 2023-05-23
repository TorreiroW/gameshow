//Start of script

#include <RCSwitch.h>

// Initialise 433MHz receiver
RCSwitch mySwitch = RCSwitch();
// Initialise script variables
long int button1=7133128;  // player 1
long int button2=2779944;  // player 2
long int button3=12072904; // player 3
long int button4=232232;   // player 4
long int buttonA=3418209;  // Host button A
long int buttonB=3418210;  // Host button B
long int buttonC=3418212;  // Host button C
long int buttonD=3418216;  // Host button D
long int gameStatus = 0;     
long int disablePlayer1 = 0;
long int disablePlayer2 = 0;
long int disablePlayer3 = 0;
long int disablePlayer4 = 0;
long int playersTurn = 0;
long int playerName = "";

void setup() {
  // Zet de seriële communicatie op 9600 baud
  Serial.begin(9600);

  // Configureer de ontvanger
  mySwitch.enableReceive(0);
  // Definieer groene led
    pinMode(13, OUTPUT);  // Stel de ingebouwde LED-pin (pin 13) in als uitgang
    pinMode(12, OUTPUT); 
    digitalWrite(12, LOW);  // Zet pin 8 hoog (5V) om het lampje aan te zetten

}
  
long int lookupPlayer(long int buttonID){
  if (buttonID==button1){
    Serial.println("Speler 1");
    playerName="Speler um";
  }
  if (buttonID==button2){
    Serial.println("Speler 2");
    playerName="Speler dois";
  }
  if (buttonID==button3){
    Serial.println("Speler 3");
    playerName="Speler três";
  }
  if (buttonID==button4){
    Serial.println("Speler 4");
    playerName="Speler quatro";
  }
  return playerName;
}

void loop() {
  // Waiting for RF-signal
  while (mySwitch.available() == false) {
    delay(100); // delay of 100ms to prevent multiple signals
  
  }

  // Process signal received
  long int receivedValue = mySwitch.getReceivedValue();
  Serial.println(receivedValue);
  Serial.print("gameStatus: ");
  Serial.println(gameStatus);

  if(receivedValue==buttonA){
    Serial.println("Host started the game");
    digitalWrite(12, LOW);
    for (int i = 0; i < 10; i++) {
    // Code to repeat 10 times
    // Place your desired code here that you want to repeat
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50); // Optional delay between iterations
    }
   
    gameStatus = 1;     // game is set
    // reset disable players
    disablePlayer1 = 0;
    disablePlayer2 = 0; 
    disablePlayer3 = 0; 
    disablePlayer4 = 0; 
        digitalWrite(LED_BUILTIN, HIGH);  // Zet de ingebouwde LED-pin hoog (5V) om het lampje aan te zetten
  delay(1000);  // Wacht 1 seconde
  digitalWrite(LED_BUILTIN, LOW);  // Zet de ingebouwde LED-pin laag (0V) om het lampje uit te zetten
  delay(1000);  // Wacht 1 seconde
  }

  if(receivedValue==buttonB && gameStatus==1){
    Serial.println("Host activated question.");
    gameStatus = 2; // answer is set
  }

  
  if (gameStatus==2){
    digitalWrite(12, LOW);  // Zet pin 8 hoog (5V) om het lampje aan te zetten
    digitalWrite(13, HIGH);  // Zet pin 8 hoog (5V) om het lampje aan te zetten
    Serial.println("Ready to receive buzz in");

    // Check for player buzz-in
    if(gameStatus==2 && receivedValue != buttonA && receivedValue != buttonB && receivedValue != buttonC && receivedValue != buttonD){
     if(receivedValue == button1 && disablePlayer1 !=1){
        playersTurn=button1;
        Serial.println("Player 1 can guess");
        gameStatus=3;
      }
      if(receivedValue == button2 && disablePlayer2 !=1){
        playersTurn=button2;
        Serial.println("Player 2 can guess");
        gameStatus=3;
      }
      if(receivedValue == button3 && disablePlayer3 !=1){
        playersTurn=button3;
        Serial.println("Player 3 can guess");
        gameStatus=3;
      }
      if(receivedValue == button4 && disablePlayer4 !=1){
        playersTurn=button4;
        Serial.println("Player 4 can guess");
        gameStatus=3;
      }
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
    }

  }

  
  if(gameStatus==1)  {
    Serial.println("Game is set");
    if(receivedValue==button1) {
      Serial.println("Player 1 pressed too early. out!");
      disablePlayer1=1;
    }
    if(receivedValue==button2) {
      Serial.println("Player 2 pressed too early. out!");
      disablePlayer2=1;
    }
    if(receivedValue==button3) {
      Serial.println("Player 3 pressed too early. out!");
      disablePlayer3=1;
    }
    if(receivedValue==button4) {
      Serial.println("Player 4 pressed too early. out!");
      disablePlayer4=1;
    }
  }
    if (receivedValue==buttonD && gameStatus==3){
      for (int i = 0; i < 20; i++) {
    // Code to repeat 10 times
    // Place your desired code here that you want to repeat
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    delay(50); // Optional delay between iterations
    }

      Serial.print("We've got a winner: ");
      //Serial.println(playersTurn);
      long int winnersName = lookupPlayer(playersTurn);
      Serial.println("And the winner is:");
      Serial.print(winnersName);
      gameStatus=0;
    }
  if(receivedValue==buttonC && gameStatus==3){
    Serial.print("Answer wrong of player:");
    Serial.println(playersTurn);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);  // Zet pin 8 hoog (5V) om het lampje aan te zetten
    delay(5000);
    for (int i = 0; i < 10; i++) {
    // Code to repeat 10 times
    // Place your desired code here that you want to repeat
    digitalWrite(12, LOW);
    delay(50);
    digitalWrite(12, HIGH);
    delay(50); // Optional delay between iterations
    digitalWrite(13, HIGH);
    }
    
    if (playersTurn==button1){disablePlayer1=1;}
    if (playersTurn==button2){disablePlayer2=1;}
    if (playersTurn==button3){disablePlayer3=1;}
    if (playersTurn==button4){disablePlayer4=1;}
    Serial.println("Other players can guess again.");
    gameStatus=2;
  }



  // Reset de ontvanger zodat deze klaar is om opnieuw te ontvangen
  mySwitch.resetAvailable();

  delay(50);

  // Schakel de ontvanger weer in om nieuwe signalen te ontvangen
  mySwitch.enableReceive(0);
}