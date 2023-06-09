#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>

int convertHexCharArrayToInt(char* array, bool isBigEndian, int len);
void displayCharArrayAsHex(std::ostream& os, char* array, int len);
void readIntFromStream(std::istream& stream, bool isBigEndian, int size, int& result);

