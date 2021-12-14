#include <proc/pic18f4520.h>
#include "config.h"
#include "lcd.h"
#include "adc.h"
#include "bits.h"
#include "keypad.h"
#include "rgb.h"
#include "pwm.h"

void main(void) {
    unsigned char temp = 25;
    unsigned int tecla = 16;
    char slot;
    int valor;
    int v;

    //inicialização das funções
    lcdInit();
    adcInit();
    kpInit();
    pwmInit();
    rgbInit();

    for (;;) {
        switch (slot) {
            case 0:
                lcdPosition(0, 1);
                //temperatura aparecendo no LCD
                lcdString("Temperatura:");

                lcdChar((temp / 10) + 48);
                lcdChar((temp % 10) + 48);
                
                lcdChar('C');
                //temperatura não passa acima de 35 ou abaixo de 25
                if (temp > 35) {
                    temp = 35;
                }
                if (temp < 25) {
                    temp = 25;
                }
                kpDebounce();
                lcdPosition(1, 0);
                
                if (kpRead() != tecla) {
                    tecla = kpRead();
                    //teclas para aumentar ou diminuir a temperatura
                    if (bitTst(tecla, 0))temp++;
                    if (bitTst(tecla, 2))temp--;
                }
                //dependendo da temperatura ele acende um led colorido diferente
                //verde para temperatura baixa, amarelo para média e vermelha
                //para quente
                if((temp>=25)&&(temp<30)){
                    rgbColor(650);//verde
                }
                if((temp>=30)&&(temp<35)){
                    rgbColor(627);//amarelo
                }
                if(temp==35){
                    rgbColor(457);//vermelho
                }
                
                slot = 1;
                break;
            case 1:
                v = adcRead(0);
                lcdPosition(1, 0);
                lcdString("Nivel Agua:");
                //potenciometro define a quantidade de água que sai do chuveiro
                if((v>=0)&&(v<=350)){
                    lcdString("Baixo");
                }
                if((v>=351)&&(v<=650)){
                    lcdString("Medio");
                }
                if((v>=651)&&(v<=1100)){
                    lcdString(" Alto");
                }
                        
                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }

    }
}