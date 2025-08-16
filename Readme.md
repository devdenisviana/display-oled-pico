# Display OLED - Desenho de Formas Geométricas

## Atividade: Desenhando Formas Geométricas no Display OLED da BitDogLab

Esta atividade demonstra como criar um programa interativo que desenha formas geométricas no Display OLED da BitDogLab utilizando a linguagem C. O programa desenha um quadrado inicialmente e permite ao usuário alternar entre diferentes formas usando botões físicos.

### Objetivo:

Desenvolver um programa interativo que:
- Configure e inicialize o Display OLED SSD1306
- Desenhe um quadrado na tela inicialmente
- Permita ao usuário apagar a tela com um botão
- Permita ao usuário desenhar um triângulo com outro botão
- Demonstre o uso de funções de desenho de linhas e texto

### Desenvolvimento:

**Passo 1:** Configuração do Ambiente

Antes de iniciar a programação, certifique-se de que o ambiente de desenvolvimento está configurado:

- Conecte a BitDogLab ao computador utilizando um cabo USB
- Abra o VS Code e carregue o projeto com a estrutura de pastas para compilar e executar código em C na BitDogLab
- Verifique se as bibliotecas necessárias estão instaladas:
  - [ssd1306.h](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306.h) para reconhecer as funções do código C
  - [ssd1306_font.h](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_font.h) para obter os desenhos no display para cada caractere
  - [ssd1306_i2c.h](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_i2c.h) para controlar o Display OLED
  - [ssd1306_i2c.c](https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/inc/ssd1306_i2c.c) para declarar as funções ativas do código C
  - Certifique-se de incluir os arquivos numa pasta separada (de nome "inc") dentro do projeto
  - Drivers para comunicação I2C

**Passo 2:** Entendimento da Configuração do Hardware

- O Display OLED está conectado ao barramento I2C da BitDogLab através dos seguintes pinos:
  - SDA: GPIO14
  - SCL: GPIO15
- Dois botões são configurados para controle:
  - Botão A: GPIO5 (desenha triângulo)
  - Botão B: GPIO6 (apaga tela)
- O endereço do Display OLED é 0x3C

**Passo 3:** Escrevendo o Código

O programa principal inclui:

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

int main()
{
    stdio_init_all();

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do display
    ssd1306_init();

    // Área de renderização
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    // Buffer do display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    // Configura os botões
    gpio_init(6); // Botão B
    gpio_set_dir(6, false); // false = entrada
    gpio_pull_up(6);
    gpio_init(5); // Botão A
    gpio_set_dir(5, false); // false = entrada
    gpio_pull_up(5);

    // Exibe quadrado inicialmente
    ssd1306_draw_string(ssd, 32, 0, "QUADRADO");
    int square_size = 40;
    int x0 = (ssd1306_width - square_size) / 2;
    int y0 = 16;
    int x1 = x0 + square_size;
    int y1 = y0 + square_size;
    ssd1306_draw_line(ssd, x0, y0, x1, y0, true);
    ssd1306_draw_line(ssd, x1, y0, x1, y1, true);
    ssd1306_draw_line(ssd, x1, y1, x0, y1, true);
    ssd1306_draw_line(ssd, x0, y1, x0, y0, true);
    render_on_display(ssd, &frame_area);

    while (true) {
        // Apaga tela com botão B
        if (gpio_get(6) == 0) {
            memset(ssd, 0, ssd1306_buffer_length);
            render_on_display(ssd, &frame_area);
            sleep_ms(200);
        }
        // Exibe triângulo com botão A
        if (gpio_get(5) == 0) {
            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 28, 0, "TRIANGULO");
            int tx0 = ssd1306_width / 2;
            int ty0 = 16;
            int tx1 = tx0 - 20;
            int ty1 = ty0 + 40;
            int tx2 = tx0 + 20;
            int ty2 = ty0 + 40;
            ssd1306_draw_line(ssd, tx0, ty0, tx1, ty1, true);
            ssd1306_draw_line(ssd, tx1, ty1, tx2, ty2, true);
            ssd1306_draw_line(ssd, tx2, ty2, tx0, ty0, true);
            render_on_display(ssd, &frame_area);
            sleep_ms(200);
        }
    }
    return 0;
}
```

**Passo 4:** Funcionalidades do Programa

O programa implementa as seguintes funcionalidades:

1. **Inicialização**: Configura o I2C e inicializa o display OLED
2. **Desenho Inicial**: Exibe um quadrado centralizado com o texto "QUADRADO"
3. **Controle por Botões**:
   - **Botão B (GPIO 6)**: Apaga completamente a tela
   - **Botão A (GPIO 5)**: Desenha um triângulo com o texto "TRIANGULO"
4. **Loop Principal**: Monitora continuamente os botões para responder às interações do usuário

**Passo 5:** Compilação e Execução

- Compile o código no VS Code
- Carregue o programa na BitDogLab
- Observe o quadrado inicial no Display OLED
- Use os botões para interagir com o display:
  - Pressione o Botão B para apagar a tela
  - Pressione o Botão A para desenhar o triângulo

### Exploração Adicional:

- Modifique as dimensões das formas geométricas
- Adicione mais formas (círculo, retângulo, etc.)
- Implemente animações simples
- Adicione cores ou padrões diferentes
- Crie uma sequência de formas que se alternam automaticamente

### Dicas Importantes:

- Sempre chame `render_on_display()` após modificar o buffer do display
- Use `sleep_ms()` para evitar leituras muito rápidas dos botões
- O buffer deve ser limpo (`memset`) antes de desenhar novas formas
- As coordenadas do display começam no canto superior esquerdo (0,0)

### Conceitos Aprendidos:

- Configuração de comunicação I2C
- Inicialização de displays OLED
- Desenho de formas geométricas usando linhas
- Controle de entrada/saída digital (GPIO)
- Manipulação de buffers gráficos
- Programação de loops de controle interativo

Este projeto demonstra como criar interfaces gráficas interativas simples em sistemas embarcados, combinando entrada de usuário com saída visual no display OLED.
