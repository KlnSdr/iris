#include <iostream>
#include <b15f/b15f.h>
#include <iomanip>

using namespace std;

int main() {
	B15F& drv = B15F::getInstance();
	drv.setRegister(&DDRA, 0x00);

	int buffer = 0;
	int offset = 0;
	bool wasNull = true;
	int pause = 0;

	while(1) {
		int value = (int) drv.getRegister(&PINA);
		std::cout << std::hex << value << std::endl;
		if (value != 0 && wasNull) {
			pause = 0;

			buffer <<= offset;
			buffer += value;
			offset += 4;
			wasNull = false;

			if (offset > 4) {
				std::cout << std::hex << buffer << " => " << static_cast<char>(buffer) << std::endl;
				offset = 0;
				buffer = 0;
			}
		} else if (value == 0 && !wasNull) {
			wasNull = true;
		} else if (value == 0 && wasNull) {
			pause++;
		}

		if (pause >= 6) {
			buffer = 0;
			offset = 0;
			pause = 0;
		}
		drv.delay_ms(250);
	}
}
