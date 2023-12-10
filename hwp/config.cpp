#include "config.hpp"

bool Config::a_isWrite = Config::a_primarySend;
bool Config::b_isWrite = !Config::a_primarySend;

bool Config::checkSumIsFOCKINGtheSame = false;
bool Config::everythingIsOkiDoki = false;
