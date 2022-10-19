#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define RS PORTDbits.RD2 
#define E PORTDbits.RD3
#define _XTAL_FREQ 4000000

void LCD_inicio(void); // Inicializa el display LCD
void LCD_CLR(void); // Borra el LCD
void LCD_OUT(char dato); // Caracter al LCD
void LCD_NL(void); // Cambia de l?nea en el LCD
void LCD_STR(const char *pun); // Cadena de caracteres al LCD
void LCD_INT(int n); // Entero al LCD