/* Demo project pingpongtoren + hoogtesensor 
 * 
 *  
 * Author: Cedric Baijot
 */

#include "mcc_generated_files/mcc.h"
#include <stdlib.h>

#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "PI.h"
#include "UART.h"
#include "adcMultiplexer.h"
#include "ledstrip.h"

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    initAdcMultiplexer();
    initLedStrip();
    
    printf("Hello :) \r\n");
    
    
    while (1) {
        
        uartHandler();
        
        // PI moet op een vaste frequentie (elke 33ms) lopen voor de integrator
        
        if (TMR0_HasOverflowOccured()) {
            TMR0_Initialize();
            
            PI();
            static uint8_t printCycle = 0; //door static toe te voegen wordt "printCycle" niet elke keer ge-herinitialiseerd maar behoudt het zijn vorige waarde
            if (printCycle++ > 30) {
                //printLogs();
                printCycle = 0;
            }
        }
        if (TMR6_HasOverflowOccured()) {
            TMR6_Initialize();
            LED_Toggle();
            
             printf("%u ", getHartslag(), 3);
             printf("\r\n");
        }
            
        /*
        // De ledstrip wordt om de 10ms geupdated
        if (TMR4_HasOverflowOccured()) {
            TMR4_Initialize();
            updateLedstripAnimation();
        }
        */
    }
    
}

/**
 End of File
 */