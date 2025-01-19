#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos
#define LED_VERMELHO 13
#define LED_AZUL 12
#define LED_VERDE 11
#define BUZZER_A 21
#define BUZZER_B 10

// Definição dos pinos do teclado matricial
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

// Função para inicializar o teclado matricial
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

// Função para ler tecla pressionada no teclado matricial
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

// Função para inicializar LEDs e buzzers
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

int main() {
    iniciar_keypad();
    iniciar_saidas();

    while (true) {
        char tecla = ler_keypad();
        if (tecla != '\0') {
            printf("Tecla pressionada: %c\n", tecla);

            switch (tecla) {
                case '1': // Liga o LED vermelho
                    gpio_put(LED_VERMELHO, 1);
                    break;
                case '2': // Desliga o LED vermelho
                    gpio_put(LED_VERMELHO, 0);
                    break;
                case '3': // Liga o buzzer A
                    gpio_put(BUZZER_A, 1);
                    break;
                case '4': // Desliga o buzzer A
                    gpio_put(BUZZER_A, 0);
                    break;
                case '5': // Liga o LED azul
                    gpio_put(LED_AZUL, 1);
                    break;
                case '6': // Desliga o LED azul
                    gpio_put(LED_AZUL, 0);
                    break;
                case '7': // Liga o buzzer B
                    gpio_put(BUZZER_B, 1);
                    break;
                case '8': // Desliga o buzzer B
                    gpio_put(BUZZER_B, 0);
                    break;
                case '9': // Pisca o LED verde
                    gpio_put(LED_VERDE, 1);
                    sleep_ms(750);
                    gpio_put(LED_VERDE, 0);
                    sleep_ms(750);
                    break;
                default:
                    printf("Tecla não atribuída: %c\n", tecla);
                    break;
            }
        }
    }
    return 0;
}




