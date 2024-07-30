#include <SoftwareSerial.h>

//Referências
//https://www.youtube.com/watch?v=MMsRlr28C6w&t=55s 
//https://electropeak.com/learn/interfacing-tb6612fng-dual-motor-driver-module-with-arduino/ 
//https://www.mcucity.com/product/2314/dual-motor-driver-tb6612fng-better-than-l298n 
//https://www.instructables.com/Driving-Small-Motors-With-the-TB6612FNG/ 

// Configuração dos pinos do drive TB6612FNG para 2 motores
int PWMA = 3;
int AIN1 = 5;
int AIN2 = 4;//motor esquerdo

int STBY = 6;

int PWMB = 9;
int BIN1 = 8;
int BIN2 = 7;//motor direito

// Configuração dos pinos do módulo Bluetooth
#define bluetoothTx 11// Pino de transmissão (TX) do módulo Bluetooth conectado ao pino 2 (RX no Arduino)
#define bluetoothRx 2 // Pino de recepção (RX) do módulo Bluetooth conectado ao pino 3 (TX no Arduino) e esse precisa do divisor de tensão de 5V para 3.3V

SoftwareSerial bluetoothSerial(bluetoothRx, bluetoothTx); // Inicializa a comunicação serial do Bluetooth

char command; // Variável para armazenar o comando Bluetooth recebido

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial para depuração
  bluetoothSerial.begin(9600); // Inicializa a comunicação serial do Bluetooth
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);// colocar o Stby como HIGH e conectar VM no 5V

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop() {
  // Verifica se há dados disponíveis no módulo Bluetooth
  if (bluetoothSerial.available() > 0) {
    command = bluetoothSerial.read(); // Lê o comando enviado pelo Bluetooth
    Serial.println(command);// Só pra testar se o módulo está enviando a mensagem pro arduino
    executeCommand(command); // Executa o comando recebido
  }
}

// Função para executar o comando recebido do Bluetooth
void executeCommand(char command) {
  switch (command) {//talvez configurar algum botão pra ele meio que rotacionar
    case 'f': // Frente
      Serial.println("Frente");//Só pra testar se tá chegando até aqui
      analogWrite(PWMA, 255); // Define a velocidade desejada (de 0 a 255) para o motor
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 255); // Define a velocidade desejada (de 0 a 255) para o motor
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      break;
    case 'b': // Trás
      Serial.println("Trás");
      analogWrite(PWMA, 255); // Define a velocidade desejada (de 0 a 255) para o motor
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      analogWrite(PWMB, 255); // Define a velocidade desejada (de 0 a 255) para o motor
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
      break;
    case 'r': // Direita
      Serial.println("Direita"); //O motor direito para e o motor esquerdo funciona, para ir somente para direita
      analogWrite(PWMA, 220); // esquerdo
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 0); //direito
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
      break;
    case 'l': // Esquerda
      Serial.println("Esquerda"); //O motor esquerdo para e o motor direito funciona, para ir somente para esquerda
      analogWrite(PWMA, 0); 
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 220); 
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      break;
    case 'i': // Frente Direita --> motor direito vai mais devagar que o esquerdo
      analogWrite(PWMA, 255); 
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 220); 
      digitalWrite(BIN1, HIGH);//ambos os motores vão pra frente
      digitalWrite(BIN2, LOW);
      break;
    case 'g': // Frente Esquerda --> motor esquerdo vai mais devagar que o direito
      analogWrite(PWMA, 220); 
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 255); 
      digitalWrite(BIN1, HIGH);//ambos os motores vão pra frente
      digitalWrite(BIN2, LOW);
      break;
    case 'h': // Trás Esquerda --> motor esquerdo vai mais devagar que o direito
      analogWrite(PWMA, 220); 
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      analogWrite(PWMB, 255); 
      digitalWrite(BIN1, LOW);//ambos os motores vão pra trás
      digitalWrite(BIN2, HIGH);
      break;
    case 'j': // Trás Direita --> motor direito vai mais devagar que o esquerdo
      analogWrite(PWMA, 255); 
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      analogWrite(PWMB, 220); 
      digitalWrite(BIN1, LOW);//ambos os motores vão pra trás
      digitalWrite(BIN2, HIGH);
      break;
    case 's': // Parar
      analogWrite(PWMA, 0); // Para o motor esquerdo
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMB, 0); // Para o motor direito
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
      break;
    default:
      break;
  }
}
