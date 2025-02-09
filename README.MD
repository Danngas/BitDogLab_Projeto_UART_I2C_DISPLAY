# 🎛️ Projeto: BitDogLab - Comunicação Serial e Controle de LEDs

## 📝 Descrição do Projeto  

Este projeto foi desenvolvido para consolidar os conhecimentos sobre a **comunicação serial (UART e I2C)** no microcontrolador **RP2040** utilizando a placa **BitDogLab**. O objetivo é controlar diversos dispositivos, como **matriz de LEDs WS2812**, **LED RGB**, **botões físicos** e um **display OLED SSD1306** com as seguintes funcionalidades:

- Modificação da biblioteca `font.h` para adicionar caracteres minúsculos.
- Exibição de caracteres digitados via Serial Monitor no display SSD1306.
- Controle de LEDs e interação com botões físicos para alternar estados.

### 🛠 Construído com  
- **Microcontrolador:** RP2040 (Placa BitDogLab)  
- **Linguagem:** C/C++  
- **Frameworks:** Pico SDK  
- **Componentes:**  
  - **Matriz 5x5 de LEDs WS2812**  
  - **LED RGB** (pinos GPIO 11, 12, 13)  
  - **Botão A** (GPIO 5)  
  - **Botão B** (GPIO 6)  
  - **Display SSD1306** (conectado via I2C: GPIO 14 e GPIO 15)

---

## 🎯 Objetivo do Projeto

O objetivo do projeto é implementar e testar as funcionalidades de **comunicação serial** com **UART**, controle de LEDs (comuns e endereçáveis WS2812), e exibição de caracteres no **display OLED SSD1306**. Além disso, o projeto explora o uso de **interrupções** (IRQ) e **debouncing** para o tratamento de botões físicos.

---

## 📌 Funcionalidades do Projeto

### 1. **Modificação da Biblioteca font.h**  
A biblioteca `font.h` foi modificada para adicionar **caracteres minúsculos**, permitindo a exibição de letras maiúsculas e minúsculas no display SSD1306.

### 2. **Entrada de Caracteres via PC (Serial Monitor)**  
Através do **Serial Monitor** no VS Code, o usuário pode digitar um **caractere por vez**.  
- O caractere digitado será exibido no **display SSD1306**.  
- Se um número de 0 a 9 for digitado, um **símbolo correspondente** ao número será exibido também na **matriz 5x5 de LEDs WS2812**.

### 3. **Interação com o Botão A**  
- Ao pressionar o **botão A**, o estado do **LED RGB verde** será alternado (ligado/desligado).  
- Uma mensagem sobre o estado do LED será exibida no **display SSD1306**.  
- Um texto descritivo sobre a operação também será enviado ao **Serial Monitor**.

### 4. **Interação com o Botão B**  
- Ao pressionar o **botão B**, o estado do **LED RGB azul** será alternado (ligado/desligado).  
- Uma mensagem sobre o estado do LED será exibida no **display SSD1306**.  
- Um texto descritivo sobre a operação será enviado ao **Serial Monitor**.


### 5. **Funções Adicionais para Exibição de Caracteres Maiores**  
   - Duas funções foram adicionadas para permitir a exibição de **caracteres maiores** no display SSD1306:
     - `void ssd1306_draw_char_large(ssd1306_t *ssd, char c, uint8_t x, uint8_t y);`
     - `void ssd1306_draw_string_large(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y);`
   - Essas funções são responsáveis por desenhar caracteres e strings maiores no display, melhorando a legibilidade.

---

## 📁 Estrutura do Projeto

### 🔹 Arquivos principais

- `projeto_serial_rp2040.c` – Programa principal que implementa a lógica do sistema, controle dos botões e exibição de caracteres no display.
- `matrizled.c` – Funções de controle para a matriz de LEDs WS2812.
- `numeros.h` – Modelos de números e caracteres a serem exibidos na matriz de LEDs e no display SSD1306.

### 🛠 Arquivos auxiliares

- **inc/font.h**  
  Mapemaneto das letras **a-z**, **A-Z** e **0-9** para exibição no display SSD1306.

- **inc/ssd1306.c**  
  Implementa a funcionalidade de controle do display OLED SSD1306. Foram adicionadas duas funções para suportar a exibição de caracteres maiores.

- **inc/ssd1306.h**  
  Definições e declarações necessárias para o funcionamento do display SSD1306.
---

## 🛠️ Estrutura do Código

O código está estruturado da seguinte forma:

### main()

- Inicializa a matriz de LEDs, o display SSD1306, e os botões físicos.
- Configura as interrupções para capturar eventos dos botões.
- Controla a exibição dos caracteres, com base no que for digitado no Serial Monitor.

### Interrupções de Botões

- O código utiliza **interrupções (IRQ)** para detectar quando o botão A ou B é pressionado e alternar o estado dos LEDs RGB correspondentes.
- O **debouncing** dos botões foi implementado via software para evitar múltiplos acionamentos.

---

## 🚀 Instruções para Execução do Projeto

### 📌 Requisitos

- **Placa:** BitDogLab (RP2040)
- **Ambiente de desenvolvimento:**  
  - [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado e configurado.
  - Compilador **ARM GCC**.
  - Ferramenta **CMake**.
  - Interface **Serial** para upload do código.

### 🚀 Passos para Compilação e Upload

1. **Clonar o repositório**  
   ```sh
   git clone https://github.com/Danngas/BitDogLab_Projeto_UART_I2C_DISPLAY.git
   cd BitDogLab_Projeto_UART_I2C_DISPLAY
