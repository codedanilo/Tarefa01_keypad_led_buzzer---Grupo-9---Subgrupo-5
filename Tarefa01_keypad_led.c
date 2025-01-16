#include <stdio.h>
#include "pico/stdlib.h"

#define PINO_VERMELHO 15
#define PINO_VERDE 14
#define PINO_AZUL 13
#define TECLA_VERMELHO 'A'
#define TECLA_VERDE 'B'
#define TECLA_AZUL 'C'

#define PINO_BUZZER 12
#define TECLA_BUZZER 'D'

#define LINHAS 4
#define COLUNAS 4

#define L1 11
#define L2 10
#define L3 9
#define L4 8
#define C1 7
#define C2 6
#define C3 5
#define C4 4

const uint8_t pinos_linhas[LINHAS] = {L1,L2,L3,L4};
const uint8_t pinos_colunas[COLUNAS] = {C1,C2,C3,C4};

const char teclas[LINHAS][COLUNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

void iniciar_keypad() {
  uint8_t i;
  for(i=0;i<LINHAS;i++) {
    gpio_init(pinos_linhas[i]);
    gpio_set_dir(pinos_linhas[i], GPIO_OUT);
    gpio_put(pinos_linhas[i], 0);
  }
  for(i=0;i<COLUNAS;i++) {
    gpio_init(pinos_colunas[i]);
    gpio_set_dir(pinos_colunas[i], GPIO_IN);
    gpio_pull_down(pinos_colunas[i]);
  }
}

char ler_keypad() {
  uint8_t linha;
  uint8_t coluna;
  for(linha=0;linha<LINHAS;linha++) {
    gpio_put(pinos_linhas[linha], 1);
    for(coluna=0;coluna<COLUNAS;coluna++) {
      if( gpio_get(pinos_colunas[coluna]) ) {
        gpio_put(pinos_linhas[linha], 0);
        return teclas[linha][coluna];
      }
    }
    gpio_put(pinos_linhas[linha], 0);
  }
  return '\0';
}

void iniciar_saidas() {
  gpio_init(PINO_VERMELHO);
  gpio_init(PINO_VERDE);
  gpio_init(PINO_AZUL);
  gpio_init(PINO_BUZZER);
  gpio_set_dir(PINO_VERMELHO, GPIO_OUT);
  gpio_set_dir(PINO_VERDE, GPIO_OUT);
  gpio_set_dir(PINO_AZUL, GPIO_OUT);
  gpio_set_dir(PINO_BUZZER, GPIO_OUT);
  gpio_put(PINO_VERMELHO, 0);
  gpio_put(PINO_VERDE, 0);
  gpio_put(PINO_AZUL, 0);
  gpio_put(PINO_BUZZER, 0);
}

int main() {
  char tecla;
  stdio_init_all();
  iniciar_keypad();
  iniciar_saidas();
  while (true) {
    sleep_ms(20);
    tecla = ler_keypad();
    if( tecla != '\0' ) {
      printf("Tecla pressionada: %c\n", tecla);
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
        gpio_put(PINO_BUZZER, 1);
        sleep_ms(250);
        gpio_put(PINO_BUZZER, 0);
        continue;
      }
      sleep_ms(180);
    }
  }
}