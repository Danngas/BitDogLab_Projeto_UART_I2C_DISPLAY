#include <stdio.h>        // Biblioteca para funções de entrada e saída padrão
#include "pico/stdlib.h"  // Biblioteca para funções específicas do Raspberry Pi Pico
#include "hardware/pwm.h" // Biblioteca para controle de PWM (Pulse Width Modulation)
#include "notas.h"        // Biblioteca externa com as notas musicais (definidas na matriz)
#include <stdlib.h>
#define NO_SOUND 0 // Definição para uma nota que significa "sem som" (frequência 0)

#define PWM_PIN 21 // Definindo o pino GPIO 21 para saída de PWM (som)

float divisor_frequency = 125; // Divisor de frequência para ajustar o tom do PWM

// Função que configura e toca uma nota com base na frequência e duração fornecida
void play_note(uint frequency, float duration, float clkdiv)
{
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN); // Obtém o número do slice (canal PWM) para o pino
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);       // Define a função do pino para PWM

    pwm_config config = pwm_get_default_config(); // Obtém a configuração padrão do PWM
    pwm_config_set_clkdiv(&config, clkdiv);       // Ajusta o divisor de frequência do PWM

    uint wrap = 125000000 / (frequency * clkdiv) - 1; // Calcula o valor de wrap baseado na frequência e divisor
    pwm_config_set_wrap(&config, wrap);               // Define o wrap para o PWM

    pwm_init(slice_num, &config, true);    // Inicializa o PWM no slice e começa a geração de sinal
    pwm_set_gpio_level(PWM_PIN, wrap / 2); // Define o duty cycle de 50% (meia altura)

    sleep_ms(duration); // Aguarda a duração da nota (em milissegundos)

    pwm_set_gpio_level(PWM_PIN, 0);    // Desliga o sinal PWM (tira o som)
    pwm_set_enabled(slice_num, false); // Desativa o PWM
}

int melodia[] = {
    NOTE_AS5,                                                                                                            // 1
    0, NOTE_A5, NOTE_AS5, NOTE_C5, NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_C5, NOTE_AS5, NOTE_A5, NOTE_AS5, // 2
    0, NOTE_A5, NOTE_GS5, NOTE_G5,                                                                                       // 3
    0, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_G5, NOTE_FS5, NOTE_G5,  // 4
    0, NOTE_DS5, NOTE_C5, NOTE_C5,                                                                                       // 5
    0, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_FS5,                                                            // 6
    0, NOTE_C5, NOTE_C5,                                                                                                 // 7
    0, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_F5, NOTE_FS5, NOTE_G5,                                                         // 8
    0, NOTE_AS5, NOTE_AS5,                                                                                               // 9
    0, NOTE_GS5, NOTE_G5, NOTE_F5, NOTE_G5,                                                                              // 10
    0, NOTE_DS5,                                                                                                         // 11
                 ////PARTE CANTADA
    NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_G5, NOTE_AS5, NOTE_F5,                         // 1
    0, NOTE_F5, NOTE_AS5, NOTE_G5, NOTE_F5, NOTE_DS5,                               // 2
    0, NOTE_DS5, NOTE_F5, NOTE_DS5, NOTE_C5, NOTE_GS5, NOTE_DS5, NOTE_C5, NOTE_AS5, // 3
    0, NOTE_DS5, NOTE_F5, NOTE_DS5, NOTE_C5, NOTE_GS5, NOTE_DS5,                    // 4
    0, NOTE_D5, NOTE_AS5, NOTE_G5, NOTE_D5,                                         // 5
    0, NOTE_C5, NOTE_G5, NOTE_F5, NOTE_C5, NOTE_C5,                                 // 6
    0, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_C5,                                        // 7
    0, NOTE_G5, NOTE_AS5, NOTE_GS5, NOTE_DS5, NOTE_F5, NOTE_G5,                     // 8
    0, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_G5, NOTE_AS5, NOTE_F5,                      // 9
    0, NOTE_GS5, NOTE_G5, NOTE_F5, NOTE_AS5, NOTE_G5, NOTE_DS5,                     // 10
    0, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_A5,                                         // 11
    0, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_A5,                                         // 12
    0, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_A5,                                         // 13
    0, NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_A5                                          // 14

};

int tempoNotas[] = {
    2,                                                 // 1
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 4, // 2
    16, 16, 16, 4,                                     // 3
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 4, // 4
    16, 16, 16, 4,                                     // 5
    16, 16, 16, 16, 16, 16, 16,                        // 6
    16, 16, 8,                                         // 7
    16, 16, 16, 16, 16, 16, 8,                         // 8
    16, 16, 4,                                         // 9
    16, 16, 16, 16, 16,                                // 10
    16, 4,                                             // 11
    // ----------------------
    16, 16, 16, 10, 8, 16,          // 1
    8, 16, 8, 16, 8, 16,            // 2
    8, 16, 16, 16, 4, 4, 4, 16, 16, // 3 ---ok
    8, 16, 16, 16, 8, 16, 4,        // 4 --- ok
    8, 16, 16, 16, 8,               // 5 --OK
    8, 16, 16, 16, 8, 16,           // 6 ---- OK
    8, 16, 16, 16, 4,               // 7
    8, 4, 4, 4, 4, 16, 16,          // 8
    8, 16, 16, 4, 16, 4, 8,         // 9
    8, 16, 16, 16, 8, 16, 4,        // 10
    16, 16, 16, 16, 16,             // 11
    16, 16, 16, 16, 16,             // 12
    16, 16, 16, 16, 16,             // 11
    16, 16, 16, 16, 16,             // 12
};

// Compasso da música (determina o tempo da música)
const int compasso = 1200; // Compasso que altera a duração das notas

// Função principal que executa a reprodução das notas