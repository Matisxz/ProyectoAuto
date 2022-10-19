#include "LCD.h"

void LCD_inicio(void) {
    PORTD = PORTD | 0x20; // Bus de 4 bits
    E = 1;
    __delay_ms(100);
    E = 0; // Dato sin validar
    RS = 0; // Comando al LCD
    LCD_OUT(0x28); // Bus de 4 bits
    LCD_OUT(0x06); // Display quieto
    LCD_OUT(0x0C); // Dispaly ON - Cursor OFF - Sin parpadeo
    __delay_ms(100);
    LCD_CLR(); // BORRO EL LCD
}
// Borra la pantalla del display LCD

void LCD_CLR(void) {
    RS = 0; // Comando al LCD
    LCD_OUT(0x01); // Borra display
    LCD_OUT(0x02); // Cursor a Home
}
// Envia un caracter al display LCD

void LCD_OUT(char dato) {
    __delay_ms(10);
    PORTD = PORTD | (dato & 0xF0);
    E = 1; // El LCD toma el dato que esta en el puerto
    __delay_ms(10);
    E = 0; // Dato sin validar
    PORTD = PORTD & 0x0F;
    PORTD = PORTD | ((dato << 4) & 0xF0);
    E = 1; // El LCD toma el dato que esta en el puerto
    __delay_ms(10);
    E = 0; // Dato sin validar
    PORTD = PORTD & 0x0F;
}
// Combia de linea en el display LCD

void LCD_NL(void) {
    RS = 0; // Comando al LCD
    LCD_OUT(0xC0); // Comando para cambiar de linea
}
// Envia una cadena de caracteres al display LCD

void LCD_STR(const char *pun) {
    RS = 1; // Dato al LCD
    while (*pun) { // Mientras el caracter no sea nulo....
        LCD_OUT(*pun); // Envio el caracter al LCD
        pun++; // Cambio al siguiente caracter
    }
    //RS=0;
}
// ENVIA UN ENTERO AL LCD

void LCD_INT(int n) {
    int m, c, d, u; // Declaro variables
    m = n / 1000;
    int m, c, d, u; // Miles
    c = (n - m * 1000) / 100; // Centenas
    d = (n - (m * 1000 + c * 100)) / 10; // Decenas
    u = n - (d * 10 + c * 100 + m * 1000); // Unidades
    //LCD_STR(texto5);            // Imprimo en el LCD: "Medicion:"
    RS = 1; // Dato al LCD
    LCD_OUT(m + 48); // Imprimo los miles
    LCD_OUT(c + 48); // Imprimo las centenas
    LCD_OUT(d + 48); // Imprimo las decenas
    LCD_OUT(u + 48); // Imprimo las unidades
    //RS=0;
}