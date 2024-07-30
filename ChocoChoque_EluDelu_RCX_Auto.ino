#include "SharpIR.h"
#include <IRremote.h>

//======= Define porta dos componentes =========================================
int led = 12;

int PWMA = 3;
const int AIN1 = 5;
const int AIN2 = 4;

const int STBY = 6;

const int BIN1 = 7;
const int BIN2 = 8;
int PWMB = 9;

const int SenInfra = A5;
decode_results results;
int valorRecebido;

// Define as portas dos sensores e o modelo
#define ir4 A7
#define ir3 A6
#define ir2 A1
#define ir1 A0
#define model 20150

SharpIR sharp4(ir4, 25, 93, model); // FRENTE ESQUERDA
SharpIR sharp3(ir3, 25, 93, model); // FRENTE DIREITA
SharpIR sharp2(ir2, 25, 93, model); // DIREITA
SharpIR sharp1(ir1, 25, 93, model); // ESQUERDA

IRrecv irrecv(SenInfra); // PASSA O PARÂMETRO PARA A FUNÇÃO irrecv
int cont = 0; // Variável que libera qual if o botão vai receber
bool robotActive = false; // Variável de estado do robô

//=============================================================================

//======= Inicializa as variáveis que recebe as distâncias dos sensores =========
unsigned int dist4 = 0;
unsigned int dist3 = 0;
unsigned int dist2 = 0;
unsigned int dist1 = 0;
//=============================================================================

//======= Define o tamanho máximo que o robô vai enxergar========================
int range1 = 35;
int range2 = 35;
int range3 = 35;
int range4 = 35;
//==============================================================================

void lerSensores()
{
  dist1 = sharp1.distance(); // leitura da distância dos sensores
  dist2 = sharp2.distance();
  dist3 = sharp3.distance();
  dist4 = sharp4.distance();
}

void stopp()
{
  digitalWrite(PWMA, 0);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMB, 0);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void receptor()
{
  if (irrecv.decode(&results)) // Executa caso receba algum código
  {
    Serial.print("Valor lido : ");
    Serial.println(results.value, HEX);
    valorRecebido = (results.value);

    if (valorRecebido == 0x10) // Código do botão 1
    {
      digitalWrite(led, HIGH);
      stopp();
      robotActive = false; // Desativa o robô
    }

    if (valorRecebido == 0x810) // Código do botão 2
    {
      digitalWrite(led, HIGH);
      robotActive = true; // Ativa o robô
    }

    if (valorRecebido == 0x410) // Código do botão 3
    {
      digitalWrite(led, LOW);
      stopp();
      robotActive = false; // Desativa o robô
    }
    
    irrecv.resume();
  }
}

void motor()
{
  if (dist3 <= range3 && dist4 <= range4) // se os sensores da FRENTE identificarem algo nada ocorre
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, 255);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    digitalWrite(PWMB, 255);
  }
  else if (dist1 <= range1) // se o sensor da ESQUERDA identificar algo, robô vira pra esquerda
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(PWMA, 200);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    digitalWrite(PWMB, 200);
  }
  else if (dist2 <= range2) // se o sensor da DIREITA identificar algo, robô vira pra direita
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, 200);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    digitalWrite(PWMB, 200);
  }
  else if (dist3 <= range3 && dist4 >= range4) // se o sensor da FRENTE DIREITA identificar algo nada ocorre
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, 100);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    digitalWrite(PWMB, 0);
  }
  else if (dist3 >= range3 && dist4 <= range4) // se o sensor da FRENTE ESQUERDA identificar algo nada ocorre
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, 0);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    digitalWrite(PWMB, 100);
  }
  else if (dist1 >= range1 && dist2 >= range2 && dist3 >= range3 && dist4 >= range4) // se os sensores NÃO VÊEM NADA
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, 200);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    digitalWrite(PWMB, 200);
  }
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // INICIALIZA A RECEPÇÃO DE SINAIS IR

  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);

  pinMode(led, OUTPUT);

  digitalWrite(led, LOW);
  digitalWrite(STBY, HIGH);

  //======= DRIVERS ========================
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  //==============================================================================

  pinMode(SenInfra, INPUT);
}

void loop()
{
  receptor();
  if (robotActive) { // Apenas ler sensores e motor quando o robô estiver ativo
    lerSensores(); // Leitura contínua dos sensores
    motor(); // Atualiza o movimento do robô com base nas leituras dos sensores
  }
}
