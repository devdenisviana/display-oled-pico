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