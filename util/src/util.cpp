#include "nin-io/util/util.h"

int convertHexCharArrayToInt(char* array, bool isBigEndian, int len) {
    int result = 0;
    if (isBigEndian) {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * (len - 1 - i));
        }
    }
    else {
        for (int i = 0; i < len; ++i) {
            result |= ((uint8_t)array[i]) << (8 * i);
        }
    }
    return result;
}

void displayCharArrayAsHex(char* arr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (static_cast<int>(arr[i]) & 0xFF) << " ";
    }
    std::cout << std::dec << std::endl;
}

void readIntFromStream(std::istream& stream, bool isBigEndian, int size, int& result)
{
	char* buffer = new char[size];
	stream.read(buffer, size);
	result = convertHexCharArrayToInt(buffer, isBigEndian, size);
	delete[] buffer;
}
