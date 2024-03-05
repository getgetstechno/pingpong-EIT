/**
 Created by Cedric Baijot
 
  Description:
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.15.0
        Device            :  PIC16F18345
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
 */


#include "mcc_generated_files/mcc.h"

#define NUMBER_OF_LEDS 60
#define STEP 2

enum states {
    GREEN_UP, RED_DOWN, BLUE_UP, GREEN_DOWN, RED_UP, BLUE_DOWN
};

enum count {
    UP, DOWN
};

void Send_LED_Frame(uint8_t intensity, uint8_t blue, uint8_t green, uint8_t red) {
    SPI1_ExchangeByte(0xE0 | intensity);
    SPI1_ExchangeByte(blue);
    SPI1_ExchangeByte(green);
    SPI1_ExchangeByte(red);
}

void Send_LED_StartFrame() {
    for (int i = 0; i < 4; i++) {
        SPI1_ExchangeByte(0x00);
    }
}

void Send_LED_EndFrame() {
    for (int i = 0; i < 4; i++) {
        SPI1_ExchangeByte(0xFF);
    }
}

void initLedStrip() {
    spi_master_open(MASTER0);
}

void updateLedstripAnimation() {
    //door static toe te voegen wordt de variabele niet slechts 1 
    static enum count direction = UP;
    static uint8_t led_run = 0;
    static enum states change_color = GREEN_UP;
    static uint8_t blue = 0x00, green = 0x00, red = 0xFF;


    switch (direction) {
        case UP: if (led_run < NUMBER_OF_LEDS - 1) {
                led_run++;
            } else {
                direction = DOWN;
            }
            break;
        case DOWN: if (led_run > 0) {
                led_run--;
            } else {
                direction = UP;
            }
            break;
    }

    switch (change_color) {
        case GREEN_UP: if (green < 0xFF) {
                green += STEP;
            } else {
                change_color = RED_DOWN;
            }
            break;
        case RED_DOWN: if (red > 0x00) {
                red -= STEP;
            } else {
                change_color = BLUE_UP;
            }
            break;
        case BLUE_UP: if (blue < 0xFF) {
                blue += STEP;
            } else {
                change_color = GREEN_DOWN;
            }
            break;
        case GREEN_DOWN: if (green > 0x00) {
                green -= STEP;
            } else {
                change_color = RED_UP;
            }
            break;
        case RED_UP: if (red < 0xFF) {
                red += STEP;
            } else {
                change_color = BLUE_DOWN;
            }
            break;
        case BLUE_DOWN: if (blue > 0x00) {
                blue -= STEP;
            } else {
                change_color = GREEN_UP;
            }
            break;
    }

    //start frame
    Send_LED_StartFrame();
    for (char led = 0; led < NUMBER_OF_LEDS; led++) {
        if (led == led_run) {
            Send_LED_Frame(0x1F, blue, green, red);
        } else {
            Send_LED_Frame(0x00, 0x00, 0x00, 0x00);
        }
    }
    //stop frame
    Send_LED_EndFrame();
}
/**
 End of File
 */