/*
 * File:   adcMultiplexer.c
 * Author: Cedric Baijot
 *
 * Created on June 28, 2023, 2:08 PM
 */



#include "mcc_generated_files/mcc.h"

/* Deze variables moeten volatile zijn omdat deze kunnen wijzigen outside of de 
 * main program flow. Deze worden namelijk gewijzigd in een interrupt service 
 * handler (ISR).
 */
volatile uint16_t hoogteSensor, potentiometer=0, hartslagsensor;

void adcConversionDoneHandler() {
    static adc_channel_t channel = Potentiometer;
    switch (channel) {
        case Potentiometer:
            potentiometer = ADC_GetConversionResult();
            channel = Hoogtesensor;

            break;
        case Hoogtesensor:
            hoogteSensor = ADC_GetConversionResult();
            channel = Hartslagsensor;

            break;
        case Hartslagsensor:
            hartslagsensor = ADC_GetConversionResult();
            channel = Hoogtesensor;
            
            break;
    }
    
    ADC_SelectChannel(channel);
}

void initAdcMultiplexer() {
    ADC_SelectChannel(Potentiometer);
    /* De ADC genereert een interrupt wanneer hij een analoge value omzet naar
     * een digitale value. Wanneer deze interrupt gegenereerd is zal de ADC van 
     * channel wisselen (potentiometer naar hoogtesensor). Op deze manier heb
     * je maar één ADC nodig voor twee inputs.
     */
    ADC_SetInterruptHandler(adcConversionDoneHandler);
}

uint8_t getHoogteSensor() {
    /* De adc heeft een resolutie van 10 bits. 
     * Om dit getal te laten passen in een byte moeten we de 2 LSB verwijderen
     */
    return hoogteSensor >> 2;
}

uint8_t getPotentiometer() {
    return potentiometer >> 2;
}

uint8_t getHartslag(){
    return hartslagsensor >> 2;
}