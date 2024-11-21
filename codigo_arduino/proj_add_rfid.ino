#include <SPI.h>
#include <MFRC522.h>


const int RED = 2;    // Pino para a cor vermelha
const int GREEN = 3;  // Pino para a cor verde
const int BLUE = 4;   // Pino para a cor azul
const int FAN = 5;    // Pino para a fan
const int BUZZER = 6; // Pino para o buzzer



// Configuração do módulo RFID
#define RST_PIN 9   // Pino RST do RC522
#define SS_PIN 10   // Pino SDA do RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Chaves permitidas
String chavesPermitidas[] = {"76F69ABB"};



bool statusLED = false; // Estado atual do LED (false = desligado, true = ligado)
bool statusFAN = false; // Estado atual da fan (false = desligada, true = ligada)

void setup() {
  Serial.begin(9600);

  // Configuração dos pinos como saída
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Inicializa o LED apagado e a fan desligada
  setColor(0, 0, 0); // Desliga o LED
  digitalWrite(FAN, LOW); // Desliga a fan

  // Inicializa o módulo RFID
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Aguardando leitura do cartão...");
}

void loop() {
  // Verifica se há dados disponíveis na Serial
  if (Serial.available()) {
    char comando = Serial.read(); // Lê o comando de um caractere

    // Lógica para controlar o LED e a fan
    if (comando == '1') {  // Comando para controlar o LED
      if (!statusLED) {
        setColor(255, 255, 255); // Liga o LED com luz branca
        statusLED = true;
        Serial.println("LED on");
      } else {
        setColor(0, 0, 0); // Desliga o LED
        statusLED = false;
        Serial.println("LED off");
      }
    } 
    else if (comando == '2') {  // Comando para controlar a fan
      if (!statusFAN) {
        digitalWrite(FAN, HIGH); // Liga a fan
        statusFAN = true;
        Serial.println("Fan on");
      } else {
        digitalWrite(FAN, LOW); // Desliga a fan
        statusFAN = false;
        Serial.println("Fan off");
      }
    } 
  }


  // Lógica para leitura do RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String codigoLido = "";

    // Obtém o UID do cartão
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      codigoLido += String(mfrc522.uid.uidByte[i], HEX);
    }
    
    codigoLido.toUpperCase(); // Converte o código para letras maiúsculas

    // Verifica se o código lido é permitido
    if (verificaAcesso(codigoLido)) {
      Serial.println("ACESSO_PERMITIDO");
      setColor(0, 255, 0);   // LED verde
      digitalWrite(BUZZER, HIGH); // Toca o buzzer
      delay(500); // Buzzer ativo por 500ms
      digitalWrite(BUZZER, LOW);
      delay(1000); // Delay pra LED ficar acesa
      setColor(0, 0, 0);   // Desliga a LED
    } else {
      Serial.println("ACESSO_NEGADO");
      setColor(255, 0, 0);   // LED vermelho
      digitalWrite(BUZZER, HIGH); // Toca o buzzer
      delay(1250); // Buzzer ativo por 1,25 segundos
      digitalWrite(BUZZER, LOW);
      delay(1000); // Delay pra LED ficar acesa
      setColor(0, 0, 0);   // Desliga a LED
      }

    // Para leitura do cartão atual
    mfrc522.PICC_HaltA();
  }


  delay(10); // Pequeno delay para melhorar a performance
}



// Função para verificar se o código lido está na lista de chaves permitidas
bool verificaAcesso(String codigo) {
  for (int i = 0; i < sizeof(chavesPermitidas) / sizeof(chavesPermitidas[0]); i++) {
    if (codigo == chavesPermitidas[i]) {
      return true;
    }
  }
  return false;
}


// Função para definir a cor do LED RGB
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
}