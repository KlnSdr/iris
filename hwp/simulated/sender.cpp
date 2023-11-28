#include <b15f/b15f.h>
#include <iostream>
#include <string> 
#include <array>

int main(){

B15F& drv = B15F::getInstance();
drv.setRegister(&DDRA, 0x0f);

drv.setRegister(&PORTA, 0);
drv.delay_ms(5000);
int a = 0xFF1;
char b = 'O';
char let[5] = {'A', 'B', 'C','D','E'};


int c = (int) (b);



int x = b & 0xF0;
int y = b & 0x0F;
int z= x>>4;
std::cout << z << std::endl;





int i= 0;


while(i<=4){

int m = (int)let[i] & 0xF0;
int n = (int)let[i] & 0x0F;
int o= m>>4;
i++;

drv.setRegister(&PORTA,o);
drv.delay_ms(500);
drv.setRegister(&PORTA, 0);
drv.delay_ms(500);
drv.setRegister(&PORTA,n);
drv.delay_ms(500);
drv.setRegister(&PORTA,0);
drv.delay_ms(1500);
}





}
