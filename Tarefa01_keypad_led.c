#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos GPIO utilizados para LEDs, botões e buzzer
#define LED_VERMELHO 13
#define LED_AZUL 12
#define LED_VERDE 11
#define BOTAO_A 5
#define BOTAO_B 6
#define BUZZER_A 21
#define BUZZER_B 10

// Definição dos pinos GPIO para teclado matricial
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

const uint8_t pinos_linhas[LINHAS] = {L1, L2, L3, L4};
const uint8_t pinos_colunas[COLUNAS] = {C1, C2, C3, C4};
const char teclas[LINHAS][COLUNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Inicialização do teclado matricial
void iniciar_keypad() {
    for (uint8_t i = 0; i < LINHAS; i++) {
        gpio_init(pinos_linhas[i]);
        gpio_set_dir(pinos_linhas[i], GPIO_OUT);
        gpio_put(pinos_linhas[i], 0);
    }
    for (uint8_t i = 0; i < COLUNAS; i++) {
        gpio_init(pinos_colunas[i]);
        gpio_set_dir(pinos_colunas[i], GPIO_IN);
        gpio_pull_down(pinos_colunas[i]);
    }
}

// Leitura de tecla no teclado matricial
char ler_keypad() {
    for (uint8_t linha = 0; linha < LINHAS; linha++) {
        gpio_put(pinos_linhas[linha], 1);
        for (uint8_t coluna = 0; coluna < COLUNAS; coluna++) {
            if (gpio_get(pinos_colunas[coluna])) {
                gpio_put(pinos_linhas[linha], 0);
                return teclas[linha][coluna];
            }
        }
        gpio_put(pinos_linhas[linha], 0);
    }
    return '\0';
}

// Inicialização dos LEDs e buzzers
void iniciar_saidas() {
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    gpio_init(BUZZER_A);
    gpio_set_dir(BUZZER_A, GPIO_OUT);
    gpio_init(BUZZER_B);
    gpio_set_dir(BUZZER_B, GPIO_OUT);

    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AZUL, 0);
    gpio_put(LED_VERDE, 0);
    gpio_put(BUZZER_A, 0);
    gpio_put(BUZZER_B, 0);
}

// Função principal
int main() {
    iniciar_keypad();
    iniciar_saidas();

    while (true) {
        // Botões físicos
        if (!gpio_get(BOTAO_A)) {
            gpio_put(LED_VERMELHO, 1);
            gpio_put(BUZZER_A, 1);
        } else {
            gpio_put(LED_VERMELHO, 0);
            gpio_put(BUZZER_A, 0);
        }

        if (!gpio_get(BOTAO_B)) {
            gpio_put(LED_AZUL, 1);
            gpio_put(BUZZER_B, 1);
        } else {
            gpio_put(LED_AZUL, 0);
            gpio_put(BUZZER_B, 0);
        }

        // LED verde piscando
        gpio_put(LED_VERDE, 1);
        sleep_ms(500);
        gpio_put(LED_VERDE, 0);
        sleep_ms(500);

        // Leitura do teclado matricial
        char tecla = ler_keypad();
        if (tecla != '\0') {
            printf("Tecla pressionada: %c\n", tecla);
            // Adicionar ações específicas para as teclas aqui, se necessário
        }
    }

    return 0;
}

