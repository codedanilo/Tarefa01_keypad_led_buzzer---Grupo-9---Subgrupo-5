#include <stdio.h>
#include "pico/stdlib.h"

// Definições dos pinos do Raspberry Pi Pico para LEDs RGB, buzzer e teclado
#define LED1_PIN_RED   13
#define LED2_PIN_GREEN 12
#define LED3_PIN_BLUE  11

#define TECLA_BLUE   'A'
#define TECLA_GREEN  'B'
#define TECLA_RED    'C'
#define TECLA_BUZZER '#'

#define BUZZER_PIN   21

// Pinos do teclado 4x4
#define ROW1_PIN 8
#define ROW2_PIN 7
#define ROW3_PIN 6
#define ROW4_PIN 5

#define COL1_PIN 4
#define COL2_PIN 3
#define COL3_PIN 2
#define COL4_PIN 28

// Mapeamento das teclas do teclado 4x4
char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Variáveis de estado para LEDs
bool led1_state = false; // LED vermelho
bool led2_state = false; // LED verde
bool led3_state = false; // LED azul

// Função para configurar os pinos do teclado
void setup_keypad() {
    // Configura as colunas como saída
    gpio_init(COL1_PIN); gpio_set_dir(COL1_PIN, GPIO_OUT);
    gpio_init(COL2_PIN); gpio_set_dir(COL2_PIN, GPIO_OUT);
    gpio_init(COL3_PIN); gpio_set_dir(COL3_PIN, GPIO_OUT);
    gpio_init(COL4_PIN); gpio_set_dir(COL4_PIN, GPIO_OUT);

    // Configura as linhas como entrada com pull-up
    gpio_init(ROW1_PIN); gpio_set_dir(ROW1_PIN, GPIO_IN); gpio_pull_up(ROW1_PIN);
    gpio_init(ROW2_PIN); gpio_set_dir(ROW2_PIN, GPIO_IN); gpio_pull_up(ROW2_PIN);
    gpio_init(ROW3_PIN); gpio_set_dir(ROW3_PIN, GPIO_IN); gpio_pull_up(ROW3_PIN);
    gpio_init(ROW4_PIN); gpio_set_dir(ROW4_PIN, GPIO_IN); gpio_pull_up(ROW4_PIN);
}

// Função para ler o teclado matricial 4x4
char get_keypad_key() {
    
        // Ativa uma coluna por vez e verifica se alguma linha foi pressionada
        for (int col = 0; col < 4; col++) {
        // Coloca todas as colunas em estado alto (não pressionadas)
        gpio_put(COL1_PIN, 1);
        gpio_put(COL2_PIN, 1);
        gpio_put(COL3_PIN, 1);
        gpio_put(COL4_PIN, 1);

        // Ativa uma coluna por vez (coloca uma coluna em estado baixo)
        if (col == 0) gpio_put(COL1_PIN, 0);
        if (col == 1) gpio_put(COL2_PIN, 0);
        if (col == 2) gpio_put(COL3_PIN, 0);
        if (col == 3) gpio_put(COL4_PIN, 0);

        // Verifica se alguma linha foi pressionada
        if (gpio_get(ROW1_PIN) == 0) return keypad[0][col];
        if (gpio_get(ROW2_PIN) == 0) return keypad[1][col];
        if (gpio_get(ROW3_PIN) == 0) return keypad[2][col];
        if (gpio_get(ROW4_PIN) == 0) return keypad[3][col];
    }

    return '\0'; // Nenhuma tecla pressionada
}

// Função para controlar os LEDs e o buzzer com base na tecla pressionada
void control_devices(char key) {
    // Controle dos LEDs baseado na tecla pressionada
    if (key == TECLA_BLUE) {
        led3_state = !led3_state; // Alterna o estado do LED azul
        gpio_put(LED3_PIN_BLUE, led3_state); // Acende ou apaga o LED azul
        printf("LED Azul (Estado: %d)\n", led3_state);
    } else if (key == TECLA_GREEN) {
        led2_state = !led2_state; // Alterna o estado do LED verde
        gpio_put(LED2_PIN_GREEN, led2_state); // Acende ou apaga o LED verde
        printf("LED Verde (Estado: %d)\n", led2_state);
    } else if (key == TECLA_RED) {
        led1_state = !led1_state; // Alterna o estado do LED vermelho
        gpio_put(LED1_PIN_RED, led1_state); // Acende ou apaga o LED vermelho
        printf("LED Vermelho (Estado: %d)\n", led1_state);
    }

    // Controle do buzzer
    if (key == TECLA_BUZZER) {
        gpio_put(BUZZER_PIN, 1); // Acende o buzzer
        sleep_ms(250);           // Mantém o buzzer ligado por 250 milissegundos
        gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
    }
}

// Função de inicialização
void setup() {
    // Inicializando pinos dos LEDs (RGB) como saídas
    gpio_init(LED1_PIN_RED); gpio_set_dir(LED1_PIN_RED, GPIO_OUT);
    gpio_init(LED2_PIN_GREEN); gpio_set_dir(LED2_PIN_GREEN, GPIO_OUT);
    gpio_init(LED3_PIN_BLUE); gpio_set_dir(LED3_PIN_BLUE, GPIO_OUT);

    // Inicializando pino do buzzer
    gpio_init(BUZZER_PIN); gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    // Configuração do teclado
    setup_keypad();
}

// Função principal
int main() {
    stdio_init_all();  // Inicializa a comunicação serial
    setup();           // Inicializa os pinos e configurações

    while (true) {
        char key = get_keypad_key(); // Lê a tecla pressionada
        if (key != '\0') {
            printf("Tecla pressionada: %c\n", key); // Mostra a tecla pressionada no console
            control_devices(key); // Controla os LEDs e o buzzer com base na tecla pressionada
        }
        sleep_ms(200); // Aguarda 200 ms antes de verificar novamente
    }

    return 0;
}
