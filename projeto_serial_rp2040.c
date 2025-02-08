#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#include "numeros.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definir os pinos GPIO para LEDs e Buzzer
#define LED_RED_PIN 13
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define BUZZER_PIN 21

// Definição dos pinos de LED e botões
#define LED_PIN 7
#define BUTTON_A 5
#define BUTTON_B 6

// Valor padrão para indicar que nenhum botão foi pressionado
int AUXBUTON1 = 0; // Variável auxiliar para armazenar qual botão foi pressionado
int AUXBUTON2 = 0;
static int numero_atual = 5; // Variável que armazena o número atualmente exibido na matriz de LEDs
char buffer[20];
static volatile uint32_t last_time = 0; // Variável auxiliar para controle de debounce

void initialize_gpio()
{
    // Configuração do LED vermelho
    gpio_init(LED_RED_PIN);              // Inicializa o pino do LED vermelho
    gpio_set_dir(LED_RED_PIN, GPIO_OUT); // Define o pino como saída

    // Configuração do LED vermelho
    gpio_init(LED_GREEN_PIN);              // Inicializa o pino do LED vermelho
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT); // Define o pino como saída
                                           // Configuração do LED vermelho
    gpio_init(LED_BLUE_PIN);               // Inicializa o pino do LED vermelho
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);  // Define o pino como saída
}

void blink_red(u_int32_t LED_PIN_blik);                   // Protótipo da função que faz o LED vermelho piscar
static void gpio_irq_handler(uint gpio, uint32_t events); // Protótipo da função de interrupção dos botões

// Loop principal do programa

ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;
int main()
{
    stdio_init_all();
    initialize_gpio();
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_A);          // Habilita o pull-up interno

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_B);          // Habilita o pull-up interno
    npInit(LED_PIN);                 // Inicializa a matriz de LEDs (função externa)

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                     // Pull up the data line
    gpio_pull_up(I2C_SCL);                     // Pull up the clock line

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Configuração das interrupções dos botões para detecção de borda de descida
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    char comando[50];

    while (1)
    {
        // blink_red(LED_RED_PIN); // Faz o LED vermelho piscar continuamente

        printf("Digite um comando: ");
        scanf("%s", comando);

        // ESCREVE NA MATRIZ DE LED
        if (comando[0] >= '0' && comando[0] <= '9' && comando[1] == '\0') // Verifica se é um único dígito de 0 a 9
        {
            int num = comando[0] - '0'; // Converte caractere para inteiro
            // ESCREVE NA MATRIZ DE LED
            Num(num);
        }

        if (strcmp(comando, "off") == 0)
        {
            Num(999); // MANDA UM VALOR INBALIDO PRA DESLIGAR OS LEDS
        }

        // ESCREVE NO DISPLAY
        ssd1306_fill(&ssd, !cor);                  // Limpa o display
        ssd1306_draw_string(&ssd, comando, 8, 10); // Desenha a string (número)
        ssd1306_send_data(&ssd);

        // Atualiza o display

        sleep_ms(1000);
    }

    return 0;
}

// Função que faz o LED vermelho piscar continuamente
void blink_red(u_int32_t LED_PIN_blik)
{
    gpio_put(LED_PIN_blik, !gpio_get(LED_PIN_blik)); // Alterna o estado do LED
    sleep_ms(200);                                   // Aguarda 200ms antes da próxima mudança
}

// Função de interrupção chamada quando um botão é pressionado
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente desde o último evento (200ms de debounce)
    if (current_time - last_time > 200000)
    {
        printf(" Interrupção ocorreu no pino %d, no evento %d\n ", gpio, events);
        last_time = current_time; // Atualiza o tempo do último evento

        // Verifica qual botão foi pressionado e define o valor de AUXBUTON
        if (gpio == BUTTON_A)
        {
            gpio_put(LED_GREEN_PIN, !gpio_get(LED_GREEN_PIN));
            AUXBUTON1 = (AUXBUTON1 + 1) % 2;                                     // Alterna entre 0 (OFF) e 1 (ON)
            sprintf(buffer, "%s", AUXBUTON1 ? "LED VERDE ON" : "LED VERDE OFF"); // Formata a string corretamente

            ssd1306_fill(&ssd, !cor);                 // Limpa o display
            ssd1306_draw_string(&ssd, buffer, 8, 10); // Desenha a string formatada
            ssd1306_send_data(&ssd);
            printf("%s", buffer);
        }
        else if (gpio == BUTTON_B)
        {
            gpio_put(LED_BLUE_PIN, !gpio_get(LED_BLUE_PIN));

            AUXBUTON2 = (AUXBUTON2 + 1) % 2; // Alterna entre 0 (OFF) e 1 (ON)

            // Buffer para armazenar a string formatada
            sprintf(buffer, " %s", AUXBUTON2 ? "LED AZUL ON" : "LED AZUL OFF"); // Formata a string corretamente
                                                                                // ESCREVE NO DISPLAY

            ssd1306_fill(&ssd, !cor);                 // Limpa o display
            ssd1306_draw_string(&ssd, buffer, 8, 10); // Desenha a string formatada
            ssd1306_send_data(&ssd);
            printf("%s", buffer);
        }
    }
}