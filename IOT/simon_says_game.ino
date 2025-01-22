int button[] = {2,4,6,8};   // Definição dos pinos dos botões.
int led[] = {3,5,7,9};      // Definição dos pinos dos LEDs.
int tones[] = {262, 330, 392, 494};  // Notas associadas a cada LED/botão.

int roundsToWin = 10;       // Número de rodadas que o jogador deve vencer para ganhar.
int buttonSequence[16];     // Array que armazenará a sequência de botões que o jogador deve seguir.

int buzzerPin = 10;         // Pino conectado ao buzzer.
int pressedButton = 4;      // Variável para armazenar o botão pressionado (4 indica que nenhum botão foi pressionado).
int roundCounter = 1;       // Contador de rodadas.

long startTime = 0;         // Armazena o tempo de início para controle de tempo.
long timeLimit = 2000;      // Tempo limite em milissegundos para pressionar um botão.

boolean gameStarted = false;  // Indica se o jogo já começou.

void setup(){
  // Configura os pinos dos botões como entrada com resistores de pull-up.
  pinMode(button[0], INPUT_PULLUP);
  pinMode(button[1], INPUT_PULLUP);
  pinMode(button[2], INPUT_PULLUP);
  pinMode(button[3], INPUT_PULLUP);

  // Configura os pinos dos LEDs como saída.
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  pinMode(led[2], OUTPUT);
  pinMode(led[3], OUTPUT);

  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer como saída.
}

void loop(){
  if (gameStarted == false){    // Se o jogo ainda não começou.
    startSequence();            // Executa a sequência de início.
    roundCounter = 0;           // Reseta o contador de rodadas.
    delay(1500);                // Aguarda um segundo e meio.
    gameStarted = true;         // Marca o início do jogo.
  }

  // Cada rodada, começa piscando a sequência de botões a ser repetida.
  for(int i=0; i <= roundCounter; i++){   // Percorre o array até a rodada atual.
    flashLED(buttonSequence[i]);          // Acende o LED correspondente e toca o som.
    delay(200);                           // Aguarda.
    allLEDoff();                          // Apaga todos os LEDs.
    delay(200);
  }

  // O jogador deve repetir a sequência pressionando os botões corretos.
  for(int i=0; i <= roundCounter; i++){   // Para cada botão da sequência.
    startTime = millis();                 // Armazena o tempo de início.

    while(true){  // Loop até o jogador pressionar um botão ou o tempo acabar.

      pressedButton = buttonCheck();      // Verifica qual botão foi pressionado.

      if (pressedButton < 4){             // Se algum botão foi pressionado.
        flashLED(pressedButton);          // Acende o LED correspondente.

        if(pressedButton == buttonSequence[i]){   // Se o botão pressionado está correto.
          delay(250);                   // Mantém o LED aceso por um momento.
          allLEDoff();                  // Apaga todos os LEDs.
          break;                        // Sai do loop while e vai para o próximo botão.
        } else {                        // Se o botão pressionado está incorreto.
          loseSequence();               // Executa a sequência de perda.
          break;                        // Sai do loop while e reinicia o jogo.
        }
      } else {                          // Se nenhum botão foi pressionado.
        allLEDoff();                    // Apaga todos os LEDs.
      }

      // Verifica se o tempo limite foi excedido.
      if(millis() - startTime > timeLimit){   // Se o tempo limite for ultrapassado.
        loseSequence();                       // Executa a sequência de perda.
        break;                                // Sai do loop e reinicia o jogo.
      }
    }    
  }

  roundCounter++;  // Aumenta o contador de rodadas.

  if (roundCounter >= roundsToWin){  // Se o jogador vencer todas as rodadas.
    winSequence();  // Executa a sequência de vitória.
  }

  delay(500);  // Espera meio segundo entre as rodadas.
}

//----------FUNÇÕES AUXILIARES------------

// Função que acende o LED e toca o som correspondente.
void flashLED (int ledNumber){
  digitalWrite(led[ledNumber], HIGH);  // Acende o LED.
  tone(buzzerPin, tones[ledNumber]);   // Toca o som.
}

// Função que apaga todos os LEDs.
void allLEDoff (){
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  digitalWrite(led[3], LOW);
  noTone(buzzerPin);  // Desliga o som.
}

// Função que verifica qual botão foi pressionado.
int buttonCheck(){
  if(digitalRead(button[0]) == LOW) return 0;  // Se o botão 0 foi pressionado.
  else if(digitalRead(button[1]) == LOW) return 1;  // Se o botão 1 foi pressionado.
  else if(digitalRead(button[2]) == LOW) return 2;  // Se o botão 2 foi pressionado.
  else if(digitalRead(button[3]) == LOW) return 3;  // Se o botão 3 foi pressionado.
  else return 4;  // Se nenhum botão foi pressionado.
}

// Função que executa a sequência de início.
void startSequence(){
  randomSeed(analogRead(A0));  // Inicializa o gerador de números aleatórios.

  for (int i=0;i<=roundsToWin;i++){
    buttonSequence[i] = round(random(0, 4));  // Gera uma sequência de números aleatórios de 0 a 3.
  }

  for(int i=0; i<=3; i++){
    tone(buzzerPin, tones[i], 200);  // Toca uma nota.

    // Acende todos os LEDs.
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], HIGH);
    digitalWrite(led[3], HIGH);

    delay(100);  // Aguarda um momento.

    // Apaga todos os LEDs.
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);

    delay(100);  // Aguarda um momento.
  }
}

// Função que executa a sequência de vitória.
void winSequence(){
  // Acende todos os LEDs.
  for(int j=0; j<=3; j++){
    digitalWrite(led[j], HIGH);
  }

  // Toca uma melodia de vitória.
  tone(buzzerPin, 1318, 150);
  delay(175);
  tone(buzzerPin, 1567, 150);
  delay(175);
  tone(buzzerPin, 2637, 150);
  delay(175);
  tone(buzzerPin, 2093, 150);
  delay(175);
  tone(buzzerPin, 2349, 150);
  delay(175);
  tone(buzzerPin, 3135, 500);
  delay(500);

  // Espera até que um botão seja pressionado.
  do {         
    pressedButton = buttonCheck();
  } while(pressedButton > 3);
  delay(100);

  gameStarted = false;  // Reseta o jogo para começar novamente.
}

// Função que executa a sequência de perda.
void loseSequence(){
  // Acende todos os LEDs.
  for(int j=0; j<=3; j++){
    digitalWrite(led[j], HIGH);
  }

  // Toca uma melodia de derrota.
  tone(buzzerPin, 130, 250);
  delay(275);
  tone(buzzerPin, 73, 250);
  delay(275);
  tone(buzzerPin, 65, 150);
  delay(175);
  tone(buzzerPin, 98, 500);
  delay(500);

  // Espera até que um botão seja pressionado.
  do {         
    pressedButton = buttonCheck();
  } while(pressedButton > 3);
  delay(200);

  gameStarted = false;  // Reseta o jogo para começar novamente.
}