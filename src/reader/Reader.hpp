#include <iostream>
#include <b15f/b15f.h>
#include <iomanip>
#include <vector>
#include <string>
#include "../helper/Helper.hpp"
#include "../config/Config.hpp"
#include "../controlChars/ControlCharDef.hpp"

class Reader{
public:
    static void read(B15F& drv, int channel, bool isPrimaryRead);
private:
    static bool escbool;
    static bool esc2bool;
    static int beginzaehler;
    static bool  beginbool;
    static bool endbool;
    static int buffer;
    static int offset;
    static bool wasNull;
    static int pause;
    static int compareWert;
    static char normalizeReading(char rawRead);
    static int checkSumsize;
    static std::vector<char> dataBuffer;
    static bool calculateCheckSumAndPrint();
};
