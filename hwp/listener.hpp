#include <iostream>
#include <b15f/b15f.h>
#include <iomanip>
#include <vector>
#include <string>
#include "helper.hpp"
#include "config.hpp"

class Reader{
public:
    static void read(B15F& drv, int channel, bool isPrimaryRead);
private:
    static const char esc;
    static bool escbool;
    static const char esc2;
    static bool esc2bool;
    static int beginzaehler;
    static const char begin;
    static bool  beginbool;
    static const char end;
    static bool endbool;
    static const char OK;
    static const char RESEND;
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
