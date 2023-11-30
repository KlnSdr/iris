#include <iostream>
#include <b15f/b15f.h>
#include <iomanip>

using namespace std;

int main() {
	B15F& drv = B15F::getInstance();
	drv.setRegister(&DDRA, 0x00);
	
	const char esc = 0x0A;
	bool escbool = false;
	const char esc2 = 0x0D;
	bool esc2bool = false;
	int esc2zaehler = 1;
	
	const char begin = 0x05;
	bool  beginbool= false;
	int beginzaehler = 1;
	const char end = 0x07;
	bool endbool = false;

	int buffer = 0;
	int offset = 0;
	bool wasNull = true;
	int pause = 0;

	int compareWert = 0;
	while(1) {
		
		int value = (int) drv.getRegister(&PINA);
		if (compareWert == value){
			drv.delay_ms(250);
			std::cout << std::hex << value << "----------------------------------------------------------------------" << std::endl;	
			continue;
		}
		compareWert = value;

		if(value == esc2){
			esc2bool = true;
			
		}
		if(value == begin){
			beginbool = true;	
		}
		if(value == end ){
			endbool = true;
		}
		
		if(value == end && esc2zaehler  < 1){
			endbool = false;
			
		}
			
		if (beginbool == true && endbool == false && esc2bool == false){
		
		if(beginzaehler < 1){
		
		std::cout << std::hex << value << std::endl;
		esc2zaehler = 1;
		
			pause = 0;

			buffer <<= offset;
			buffer += value;
			offset += 4;
			wasNull = false;

			if (offset > 4){
				std::cout << std::hex << buffer << " => " << static_cast<char>(buffer) << std::endl;
				offset = 0;
				buffer = 0;
			}

		if (pause >= 6) {
			buffer = 0;
			offset = 0;
			pause = 0;
		}
		drv.delay_ms(250);
		}else{beginzaehler--;
			std::cout << std::hex << value << std::endl;
			drv.delay_ms(500);
		}
	}else 
	{
		std::cout << std::hex << value << "----------------------------------------------------------------------" << std::endl;	
		if(esc2bool == true){
			esc2zaehler--;
		}
		esc2bool = false;
		drv.delay_ms(250);
		
		
	}
}

}
