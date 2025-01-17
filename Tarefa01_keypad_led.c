#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos GPIO utilizados para os LEDs e o buzzer
#define PINO_VERMELHO 15
#define PINO_VERDE 14
#define PINO_AZUL 13
#define TECLA_VERMELHO 'A'
#define TECLA_VERDE 'B'
#define TECLA_AZUL 'C'
#define PINO_BUZZER 12
#define TECLA_BUZZER 'D'

// Definição do tamanho do teclado matricial (4x4)
#define LINHAS 4
#define COLUNAS 4

// Definição dos pinos GPIO das linhas e colunas do teclado matricial
#define L1 11
#define L2 10
#define L3 9
#define L4 8
#define C1 7
#define C2 6
#define C3 5
#define C4 4

// Arrays com os pinos correspondentes às linhas e colunas
const uint8_t pinos_linhas[LINHAS] = {L1,L2,L3,L4};
const uint8_t pinos_colunas[COLUNAS] = {C1,C2,C3,C4};

// Mapeamento das teclas do teclado matricial
const char teclas[LINHAS][COLUNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Função para inicializar os pinos das linhas e colunas do teclado
void iniciar_keypad() {
  uint8_t i;
  // Configura os pinos das linhas como saída e define o estado inicial como 0
  for(i=0;i<LINHAS;i++) {
    gpio_init(pinos_linhas[i]);
    gpio_set_dir(pinos_linhas[i], GPIO_OUT);
    gpio_put(pinos_linhas[i], 0);
  }
  // Configura os pinos das colunas como entrada e ativa o pull-down interno
  for(i=0;i<COLUNAS;i++) {
    gpio_init(pinos_colunas[i]);
    gpio_set_dir(pinos_colunas[i], GPIO_IN);
    gpio_pull_down(pinos_colunas[i]);
  }
}

// Função para ler a tecla pressionada no teclado matricial
char ler_keypad() {
  uint8_t linha;
  uint8_t coluna;
  // Itera pelas linhas e verifica se há sinal em alguma coluna
  for(linha=0;linha<LINHAS;linha++) {
    gpio_put(pinos_linhas[linha], 1); // Ativa a linha atual
    for(coluna=0;coluna<COLUNAS;coluna++) {
      if( gpio_get(pinos_colunas[coluna]) ) { // Verifica se há sinal na coluna
        gpio_put(pinos_linhas[linha], 0); // Desativa a linha atual
        return teclas[linha][coluna]; // Retorna a tecla pressionada
      }
    }
    gpio_put(pinos_linhas[linha], 0); // Desativa a linha atual
  }
  return '\0'; // Retorna '\0' se nenhuma tecla foi pressionada
}

// Função para inicializar os pinos dos LEDs e do buzzer
void iniciar_saidas() {
  gpio_init(PINO_VERMELHO);
  gpio_init(PINO_VERDE);
  gpio_init(PINO_AZUL);
  gpio_init(PINO_BUZZER);
  gpio_set_dir(PINO_VERMELHO, GPIO_OUT);
  gpio_set_dir(PINO_VERDE, GPIO_OUT);
  gpio_set_dir(PINO_AZUL, GPIO_OUT);
  gpio_set_dir(PINO_BUZZER, GPIO_OUT);
  // Inicializa os pinos com estado desligado
  gpio_put(PINO_VERMELHO, 0);
  gpio_put(PINO_VERDE, 0);
  gpio_put(PINO_AZUL, 0);
  gpio_put(PINO_BUZZER, 0);
}

// Função principal
int main() {
  char tecla;
  stdio_init_all();
  iniciar_keypad(); // Configura o teclado matricial
  iniciar_saidas(); // Configura os LEDs e o buzzer
  while (true) {
    sleep_ms(20); // Aguarda 20 milissegundos para melhor funcionamento do simulador 
    tecla = ler_keypad(); // Lê as teclas do keypad
    if( tecla != '\0' ) { // Verifica se uma tecla foi pressionada
      printf("Tecla pressionada: %c\n", tecla); // Exibe a tecla no terminal
      // Alterna o estado dos LEDs ou ativa o buzzer com base na tecla pressionada
      if(tecla == TECLA_VERMELHO) {
        gpio_put(PINO_VERMELHO, !gpio_get(PINO_VERMELHO));
      }else
      if(tecla == TECLA_VERDE) {
        gpio_put(PINO_VERDE, !gpio_get(PINO_VERDE));
      }else
      if(tecla == TECLA_AZUL) {
        gpio_put(PINO_AZUL, !gpio_get(PINO_AZUL));
      }else
      if(tecla == TECLA_BUZZER) {
        gpio_put(PINO_BUZZER, 1); // Liga o buzzer
        sleep_ms(250); // Mantém o buzzer ligado por 250 milissegundos
        gpio_put(PINO_BUZZER, 0); // Desliga o buzzer
        continue; // Volta ao início do loop
      }
      sleep_ms(180); // Aguarda antes de processar a próxima tecla para evitar leituras instáveis
    }
  }
}