//Seteamos los "Configuration bits"
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

//Incluimos librerias necesarias
#include <xc.h>
#include "LCD.h"

//Definimos frecuencia del cristal
#define _XTAL_FREQ 4000000

//Definimos el nombre de los botones
#define adelante RB1
#define atras RB2
#define izquierda RB3
#define derecha RB5

volatile int d; //Variable en la que guardaremos el dato de la distancia

void interrupt echo() {
    if (RBIF == 1) //Nos aseguramos de que haya una interrupcion por el PORTB
    {
        RBIE = 0;
        if (RB4 == 1) { //Si ECHO es 1
            TMR1ON = 1;
        } //Comienza el temporizador
        if (RB4 == 0) //Si ECHO es 0
        {
            TMR1ON = 0; //Se detiene el temporizador
            d = (TMR1L | (TMR1H << 8)) / 58.82; //Se calcula la distancia

            if (d < 20) { //En caso de que la distancia sea menor a 20 el auto se detiene y luego retrocede
                RC4 = RC5 = RC6 = RC7 = 0;

                __delay_ms(1000);
                //primer motor
                RC4 = 0;
                RC5 = 1;

                //segundo motor
                RC6 = 0;
                RC7 = 1;

                __delay_ms(1000);
                RC4 = RC5 = RC6 = RC7 = 0;
            }

        }
    }
    RBIF = 0; //Clear PORTB On-Change Interrupt flag
    RBIE = 1; //Enable PORTB On-Change Interrupt
}

void main() {
    TRISB = 0b11111110; //RB0 como salida para TRIGGER del sensor ultrasonico
    TRISC = 0b00001111; // MOTORES (SALIDA) y primeros pines entrada
    TRISD = 0; // LCD (SALIDA)

    PORTB = PORTC = PORTD = 0;

    LCD_inicio();
    LCD_CLR();
    LCD_STR("Ultrasonico");
    __delay_ms(1000);
    LCD_CLR();

    GIE = 1; //Interrupciones globales habilitadas
    RBIF = 0; //Clear PORTB On-ChaGIE = 1; //Global Interrupt Enablenge Interrupt Flag
    RBIE = 1; //Interrupciones por PORTB habilitadas

    T1CON = 0x00; //Inicializacion del modulo del timer

    while (1) {

        TMR1H = 0; //Setea el valor inicial del timer
        TMR1L = 0; //Setea el valor inicial del timer

        RB0 = 1; //TRIGGER en 1
        __delay_us(10); //10uS Delay
        RB0 = 0; //TRIGGER en 0 

        __delay_ms(100); //Esperando por el "ECHO"

        d = d + 1; //Correccion de error

        if (d >= 2 && d <= 400) {//Chequea si el resultado es correcto o no

            //Imprimimos la distancia en un LCD
            LCD_CLR();
            LCD_STR("Dist:");
            LCD_INT(d);
            LCD_STR("CM");

        } else {
            LCD_CLR();
            LCD_STR("Fuera de rango");

        }

        //Si tocamos el boton configurado hacia adelante y la distancia es mayor a 20 enviaremos la señal para que avanze
        if (adelante == 1 && d > 20) {
            if (d > 20) {
                //Primer motor
                RC4 = 1;
                RC5 = 0;

                //Segundo motor
                RC6 = 1;
                RC7 = 0;

            }
        }

        //Si tocamos el boton configurado hacia la izquierda el auto se movera en esa direccion
        if (izquierda == 1) {
            //Primer motor
            RC4 = 1;
            RC5 = 0;

            //Segundo motor
            RC6 = 0;
            RC7 = 01;
        }

        //Si tocamos el boton configurado hacia la derecha el auto se movera en esa direccion
        if (derecha == 1) {
            //Primer motor
            RC4 = 0;
            RC5 = 1;

            //Segundo motor
            RC6 = 1;
            RC7 = 0;
        }

        //Si tocamos el boton configurado hacia la atras el auto retrocedera
        if (atras == 1) {
            //primer motor
            RC4 = 0;
            RC5 = 1;

            //segundo motor
            RC6 = 0;
            RC7 = 1;
        }

        //Cuando ninguno de los botones esta presionados se ponen todos los pines con los que se comanda el PuenteH en 0
        if (adelante == 0 && atras == 0 && derecha == 0 && izquierda == 0) {
            //Primer motor
            RC4 = 0;
            RC5 = 0;

            //Segundo motor
            RC6 = 0;
            RC7 = 0;
        }
    }
}